#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp" // for odom callback
#include "sensor_msgs/msg/laser_scan.hpp" // for detection callback, for lidar
#include "nav_msgs/msg/occupancy_grid.hpp" // for map callback
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp" // for rviz visualization to publish best pose estimate
#include "geometry_msgs/msg/pose_array.hpp" // to visualize particles over the map
// my own included packages
#include "semantic_mcl/motion_model.hpp"
#include "semantic_mcl/beam_model.hpp"
#include "semantic_mcl/particle_filter.hpp"
#include "semantic_mcl/semantic_model.hpp" // for our semantic visibility model
#include "semantic_msgs/msg/semantic_detection_array.hpp"
#include <optional> // for defining private objects
#include <cmath> // for atan2

#define DEBUG_MSG

namespace semantic_mcl {
class SemanticMCL : public rclcpp::Node {
public:
    SemanticMCL() : Node("semantic_mcl_node") {
        RCLCPP_INFO(this->get_logger(), "Created semantic mcl node!");
        this->odom_initialized_ = false;

        // subscribe to the lidar scan (and in the future to the semantic model)
        this->det_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("scan", 10, std::bind(&SemanticMCL::detection_callback, this, std::placeholders::_1));

        // subscribe to the odometry messages
        this->odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>("odom", 10, std::bind(&SemanticMCL::odom_callback, this, std::placeholders::_1));

        // susbcribe to the map topic
        // use transient local on rviz2
        auto map_qos = rclcpp::QoS(rclcpp::KeepLast(1)).transient_local().reliable();
        this->map_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>("map", map_qos, std::bind(&SemanticMCL::map_callback, this, std::placeholders::_1));
        
        // publisher that publish best pose estimate for rviz visualization
        this->pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("mcl_pose", 10);

        // publisher thta publishes the robot hypothetahiocal poses over the map
        this->particle_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("particle_cloud", rclcpp::QoS(10).reliable().transient_local());

        // subscrbe to the semantic detections topic
        // first get the parameter to know if we are using vlm or yolo
        this->declare_parameter("semantic_model_type", "vlm");
        std::string semantic_model_type = this->get_parameter("semantic_model_type").as_string();
        std::string semantic_topic_name;
        if (semantic_model_type == "yolo")
            semantic_topic_name = "yolo/semantic_detections";
        else if (semantic_model_type == "vlm")
            semantic_topic_name = "vlm/semantic_detections";
        this->semantic_sub_ = this->create_subscription<semantic_msgs::msg::SemanticDetectionArray>(
            semantic_topic_name, 10, std::bind(&SemanticMCL::semantic_callback, this, std::placeholders::_1)
        );

        // declare parameters for motion model and converting them into usable values
        this->declare_parameter("alpha1", 0.2);
        double alpha1 = this->get_parameter("alpha1").as_double();
        this->declare_parameter("alpha2", 0.2);
        double alpha2 = this->get_parameter("alpha2").as_double();
        this->declare_parameter("alpha3", 0.2);
        double alpha3 = this->get_parameter("alpha3").as_double();
        this->declare_parameter("alpha4", 0.2);
        double alpha4 = this->get_parameter("alpha4").as_double();
        this->declare_parameter("alpha5", 0.0); // only applies to domnidirectional model
        double alpha5 = this->get_parameter("alpha5").as_double();
        this->declare_parameter("motion_type", "differential"); // change this when deploying in real life
        std::string motion_type_str = this->get_parameter("motion_type").as_string();
        MotionModelType motion_type = (motion_type_str == "differential") ? MotionModelType::DIFFERENTIAL_MODEL : MotionModelType::OMNIDIRECTIONAL_MODEL;

        // define the motion model private member
        mm_.emplace(alpha1, alpha2, alpha3, alpha4, alpha5, motion_type);

        // declare parameters for beam model and converting them into usable values
        this->declare_parameter("z_hit", 0.5);
        double z_hit = this->get_parameter("z_hit").as_double();
        this->declare_parameter("z_max", 0.05);
        double z_max = this->get_parameter("z_max").as_double();
        this->declare_parameter("z_rand", 0.5);
        double z_rand = this->get_parameter("z_rand").as_double();
        this->declare_parameter("z_short", 0.05);
        double z_short = this->get_parameter("z_short").as_double();
        this->declare_parameter("sigma_hit", 0.2);
        double sigma_hit = this->get_parameter("sigma_hit").as_double();
        this->declare_parameter("lambda_short", 0.1);
        double lambda_short = this->get_parameter("lambda_short").as_double();
        this->declare_parameter("z_max_val", 12.0 );
        double z_max_val = this->get_parameter("z_max_val").as_double();

        // define the beam model private member
        bm_.emplace(z_hit, z_short,  z_max, z_rand, sigma_hit, lambda_short, z_max_val);

        // declare parameters for the particle filter
        this->declare_parameter("particle_count", 2000);
        this->particle_count_ = this->get_parameter("particle_count").as_int();
        this->declare_parameter("ess_threshold", 3.0);
        this->ess_threshold_ = this->get_parameter("ess_threshold").as_double();
        // added for convergence issues
        this->declare_parameter("w_slow", 0.0);
        this->w_slow_ = this->get_parameter("w_slow").as_double();
        this->declare_parameter("w_fast", 0.0);
        this->w_fast_ = this->get_parameter("w_fast").as_double();
        this->declare_parameter("alpha_slow", 0.001);
        this->alpha_slow_ = this->get_parameter("alpha_slow").as_double();
        this->declare_parameter("alpha_fast", 0.1);
        this->alpha_fast_ = this->get_parameter("alpha_fast").as_double();

        // declare parameters for semantic model
        this->declare_parameter("landmark_json_path", "");
        this->declare_parameter("sigma_bearing", 0.3);
        this->declare_parameter("sigma_range", 1.0);
        this->declare_parameter("use_range", false);
        this->declare_parameter("semantic_min_confidence", 0.5);
        this->semantic_min_confidence_ = this->get_parameter("semantic_min_confidence").as_double();
    }

    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
        // check if pf_ is not init , go out
        if (!pf_.has_value()) return;

        // get the current pose
        semantic_mcl::Pose current_pose;
        current_pose.x = msg->pose.pose.position.x;
        current_pose.y = msg->pose.pose.position.y;
        // thata = atan2(2*(w*z + x*y), 1 - 2*(y*y + z*z))
        double x = msg->pose.pose.orientation.x;
        double y = msg->pose.pose.orientation.y;
        double z = msg->pose.pose.orientation.z;
        double w = msg->pose.pose.orientation.w;

        current_pose.theta = std::atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));

        if (!this->odom_initialized_) { // if odom is not initialized yet i.e. first time callback is ran
            this->prev_odom_ = current_pose; // previous odom have a garbage value here
            this->odom_initialized_ = true;
        } else { 
            // perform the prediction where the next pose is calculated with odometry
            pf_->predict(this->prev_odom_, current_pose);
            // set the previous odom pose to the current pose
            this->prev_odom_ = current_pose; 
        }
    }

    void detection_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) { // takes in input messages from /scan topic
        // check if particle filter is not init
        if (!pf_.has_value()) return;

        // build the beams and z_obsv to update the particle filter by iterating through the laser scans
        std::vector<double> z_obs;
        std::vector<double> beam_angles;
        int beam_skip = 20;
        for (size_t i = 0; i < msg->ranges.size(); i += beam_skip) {
            z_obs.push_back((double) msg->ranges[i]);
            beam_angles.push_back(msg->angle_min + i * msg->angle_increment);
        }

        // update the particle filter
        pf_->update(z_obs, beam_angles);

        // only resample when ESS drops below N/2 to prevent particle depletion
        double sum_sq_weights = 0.0;
        for (const Particle& p : pf_->get_particle_array())
            sum_sq_weights += p.weight * p.weight;
        double ess = (sum_sq_weights > 0.0) ? 1.0 / sum_sq_weights : particle_count_;
        if (ess < particle_count_ / this->ess_threshold_)
            pf_->resample();

        publish_particle_cloud();
        // call best estimate and construct the pose with covariance stamped object
        Pose estimate = pf_->best_estimate();
        geometry_msgs::msg::PoseWithCovarianceStamped pose_msg;
        pose_msg.header.stamp = this->now();
        pose_msg.header.frame_id = "map";
        pose_msg.pose.pose.position.x = estimate.x;
        pose_msg.pose.pose.position.y = estimate.y;
        // convert theta to quartenion
        pose_msg.pose.pose.orientation.z = std::sin(estimate.theta / 2.0);
        pose_msg.pose.pose.orientation.w = std::cos(estimate.theta / 2.0);
        // publish the pose with covariance stamped message
        this->pose_pub_->publish(pose_msg);
    }

    void map_callback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
        if (pf_.has_value()) return;
        semantic_mcl::OccupancyMap ogm;
        ogm.data = msg->data;
        ogm.width = msg->info.width;
        ogm.height = msg->info.height;
        ogm.resolution = msg->info.resolution;
        ogm.origin_x = msg->info.origin.position.x;
        ogm.origin_y = msg->info.origin.position.y;

        pf_.emplace(this->particle_count_, ogm, bm_.value(), mm_.value(), this->ess_threshold_,
                    this->w_slow_, this->w_fast_, this->alpha_slow_, this->alpha_fast_);
        pf_->init();
        publish_particle_cloud(); // show uniform distribution before any scan arrives

        // create the semantic model object
        sm_.emplace(ogm,
            this->get_parameter("landmark_json_path").as_string(),
            this->get_parameter("sigma_bearing").as_double(),
            this->get_parameter("sigma_range").as_double(),
            this->get_parameter("use_range").as_bool()
        );
    }

    // for displaying on rviz the particles
    void publish_particle_cloud() {
        geometry_msgs::msg::PoseArray cloud_msg;
        cloud_msg.header.stamp = this->now();
        cloud_msg.header.frame_id = "map";
        for (const Particle& p : pf_->get_particle_array()) {
            geometry_msgs::msg::Pose pose;
            pose.position.x = p.x;
            pose.position.y = p.y;
            pose.orientation.z = std::sin(p.theta / 2.0);
            pose.orientation.w = std::cos(p.theta / 2.0);
            cloud_msg.poses.push_back(pose);
        }
        this->particle_pub_->publish(cloud_msg);
    }

    // for semantic detections, will trigger if the vlm or yolo model detects an object
    void semantic_callback(const semantic_msgs::msg::SemanticDetectionArray::SharedPtr msg) {
        // check if particle filter is init
        if (!pf_.has_value() || !sm_.has_value()) return;

        // clear the latest detections and push the msg receive into latest detections
        this->latest_detections_.clear();
        for (const auto& d: msg->detections) {
            if (d.confidence < this->semantic_min_confidence_) continue;
            semantic_mcl::SemanticDetection sm_object {d.label, d.bearing, d.range, d.confidence};
            this->latest_detections_.push_back(sm_object);
        }

        // no detections after filtering: skip the update entirely so the beam
        // model runs uncontested — hallucinations or empty corridors have no effect
        if (this->latest_detections_.empty()) return;

        // calculate the weight for each particle
        double sem_min = 1e10, sem_max = -1e10;
        for (Particle &p : pf_->get_particle_array()) {
            double sw = sm_->compute_weight(p.x, p.y, p.theta, this->latest_detections_);
            if (sw < sem_min) sem_min = sw;
            if (sw > sem_max) sem_max = sw;
            p.weight *= sw;
        }
        #ifdef DEBUG_MSG
            RCLCPP_INFO(this->get_logger(), "Semantic update: %zu detections, weight range [%.6f, %.6f]",
                        this->latest_detections_.size(), sem_min, sem_max);
        #endif
        // renormalize the weights so that the total weight = 1.0
        double total = 0.0;
        for (Particle &p : pf_->get_particle_array())
            total += p.weight;
        for (Particle &p : pf_->get_particle_array())
            p.weight /= total;

        // resample after semantic update (like Zimmerman's separate correct-normalize-resample cycle)
        // this locks in the semantic discrimination before the beam model can dilute it
        double sum_sq = 0.0;
        for (const Particle& p : pf_->get_particle_array())
            sum_sq += p.weight * p.weight;
        double ess = (sum_sq > 0.0) ? 1.0 / sum_sq : particle_count_;
        if (ess < particle_count_ / this->ess_threshold_) {
            pf_->resample();
            #ifdef DEBUG_MSG
                RCLCPP_INFO(this->get_logger(), "Semantic resample triggered (ESS=%.1f)", ess);
            #endif
        }

        publish_particle_cloud();
    }

private:
    // create the UDT objects
    std::optional<MotionModel> mm_;
    std::optional<BeamModel> bm_;
    std::optional<ParticleFilter> pf_;
    std::optional<OccupancyMap> map_;
    std::optional<SemanticModel> sm_;
    std::vector<SemanticDetection> latest_detections_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr det_sub_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_sub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr pose_pub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr particle_pub_;
    rclcpp::Subscription<semantic_msgs::msg::SemanticDetectionArray>::SharedPtr semantic_sub_;
    Pose prev_odom_;
    int particle_count_;
    bool odom_initialized_;
    double ess_threshold_;
    double w_slow_;
    double w_fast_;
    double alpha_slow_;
    double alpha_fast_;
    double semantic_min_confidence_;
};
} // namespace semantic_mcl

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<semantic_mcl::SemanticMCL>());
    rclcpp::shutdown();
    return 0;
}