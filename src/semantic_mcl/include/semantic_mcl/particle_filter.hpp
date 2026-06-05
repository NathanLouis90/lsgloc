#ifndef SEMANTIC_MCL_PARTICLE_FILTER_HPP
#define SEMANTIC_MCL_PARTICLE_FILTER_HPP

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>

// include my own pose and 
#include "semantic_mcl/motion_model.hpp"
#include "semantic_mcl/beam_model.hpp"

namespace semantic_mcl {
/**
 * @brief A single hypothesis about the robot's pose.
 *        The weight is unnormalized during the update step and
 *        normalized before resampling.
 */
struct Particle {
    double x;
    double y;
    double theta;
    double weight; // the sum over all particles should equal 1 as this indicates likelihood i.e. higher weight -> higher likelihood
};

/**
 * @brief One semantic landmark loaded from the JSON map.
 */
struct Landmark {
    double x;
    double y;
    std::string label;
};

/**
 * @brief Minimal OGM interface the particle filter needs.
 *        Your MapLoader should populate this and pass it in.
 */
struct OccupancyMap {
    std::vector<int8_t> data; // row major where -1 is unkniwn, 100 is occupied and 0 is unoccupied
    unsigned int width;
    unsigned int height;
    double resolution; // meters per cell
    double origin_x;
    double origin_y;

    bool world_to_grid(double wx, double wy, int& row, int& col) const {
        col = static_cast<int>((wx - origin_x) / resolution);
        row = static_cast<int>((wy - origin_y) / resolution);

        return col >=0 && col < (int) width && row >= 0 && row < (int) height;
    }

    bool is_free_space(int col, int row) const {
        if (col < 0 || col >= (int) width || row < 0 || row >= (int) height)
            return false;
        return data[row * width + col] == 0;
    }

    bool is_occupied(int col, int row) const {
        if (col < 0 || col >= (int) width || row < 0 || row >= (int) height)
            return false;
        return data[row * width + col] == 100;
    }
};

/**
 * @brief The particle class defines possible hypothesis over the map. It is initialized by a uniform distruibution of partiocles over the map.
 *        The process is simple: motion prediction -> sensor correction -> resampling
 *        Motion prediction: use the odometry to estimate where the robot is
 *        Sensor correction: use measurements from the lidar and camera to update the weights by increasing and decreasing those that do not match the sensor observations
 *        Resampling: Add particles. ideally those that matches the location of the robot and prune particles that is of low weight  
 */
class ParticleFilter {
public:
    ParticleFilter(size_t particle_count, OccupancyMap ogm, BeamModel beam_model, MotionModel motion_model, double ess_threshold, double w_slow, double w_fast, double alpha_slow, double alpha_fast);
    void init();
    void predict(const Pose& u_old, const Pose& u_new);
    void update(const std::vector<double>& z_obs, const std::vector<double>& beam_angles);
    void resample();
    Pose best_estimate() const; // outputs a single pose which represents the best estimate of where it is at and what it is facing
    std::vector<Particle>& get_particle_array(); // getter function for partilce array
private:
    size_t particle_count_; // number of particles
    OccupancyMap map_;
    BeamModel beam_model_;
    MotionModel motion_model_;
    std::mt19937 gen_; 
    double z_max_range_; // the maximum range in which the beam can hit an obstacle, this is the ray_cast output
    std::vector<Particle> particles_;
    double ray_cast(double x, double y, double theta, double beam_angle) const; // private helper
    double ess_threshold_;
    // for confidence estimates of robot, if alpha_fast < alpha_slow, conmfidence drops and particles gets redistirbuted
    double w_slow_;
    double w_fast_;
    double alpha_slow_;
    double alpha_fast_;
    bool w_initialized_ = false;
};

} // namespace semantic_mcl

#endif