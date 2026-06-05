#include "semantic_mcl/motion_model.hpp"

namespace semantic_mcl {
    MotionModel::MotionModel(double a1, double a2, double a3, double a4, MotionModelType type) : alpha1_(a1), alpha2_(a2), alpha3_(a3), alpha4_(a4), alpha5_(0.0), type_(type) { // differential model
        std::random_device rd;
        gen_.seed(rd());
    }

    MotionModel::MotionModel(double a1, double a2, double a3, double a4, double a5, MotionModelType type) : alpha1_(a1), alpha2_(a2), alpha3_(a3), alpha4_(a4), alpha5_(a5), type_(type) { // omnidirectional model
        std::random_device rd;
        gen_.seed(rd());
    }

    Pose MotionModel::sample(const Pose& u_old, const Pose& u_new, const Pose& x_old) {
        if (type_ == MotionModelType::DIFFERENTIAL_MODEL) {
            // calculate delta motion relative to odom frame
            double d_trans = std::sqrt(std::pow(u_new.x - u_old.x, 2) + std::pow(u_new.y - u_old.y, 2)); // translation distance
            double d_rot1 = (d_trans > 1e-6)
                ? std::atan2(u_new.y - u_old.y, u_new.x - u_old.x) - u_old.theta
                : 0.0; // initial rotation — guard against atan2(0,0) on pure forward motion
            double d_rot2 = u_new.theta - u_old.theta - d_rot1;

            // add gaussian noise to relative motion
            double noisy_rot1  = d_rot1  - sample_normal(std::sqrt(alpha1_ * d_rot1*d_rot1  + alpha2_ * d_trans*d_trans));
            double noisy_trans = d_trans - sample_normal(std::sqrt(alpha3_ * d_trans*d_trans + alpha4_ * (d_rot1*d_rot1 + d_rot2*d_rot2)));
            double noisy_rot2  = d_rot2  - sample_normal(std::sqrt(alpha1_ * d_rot2*d_rot2  + alpha2_ * d_trans*d_trans));

            // apply noisy motion to particle previous pose
            Pose x_new;
            x_new.x = x_old.x + noisy_trans * std::cos(x_old.theta + noisy_rot1);
            x_new.y = x_old.y + noisy_trans * std::sin(x_old.theta + noisy_rot1);
            x_new.theta = x_old.theta + noisy_rot1 + noisy_rot2;

            // normalize theta to [-PI, PI]
            while (x_new.theta >  M_PI) x_new.theta -= 2.0 * M_PI;
            while (x_new.theta < -M_PI) x_new.theta += 2.0 * M_PI;

            return x_new;
        } else { // omnidirectional
            // get the raw delta
            double dx_odom = u_new.x - u_old.x;
            double dy_odom = u_new.y - u_old.y;
            double dtheta_odom = u_new.theta - u_old.theta;

            // tf from old to new frame
            double dx_robot = dx_odom * std::cos(u_old.theta) + dy_odom * std::sin(u_old.theta);
            double dy_robot = -dx_odom * std::sin(u_old.theta) + dy_odom * std::cos(u_old.theta);
            double dtheta_robot = dtheta_odom;

            // add gaussian noise by computing standard deviation
            double std_forward = alpha3_ * std::abs(dx_robot) + alpha4_ * std::abs(dtheta_robot) + alpha5_ * std::abs(dy_robot);
            double std_left = alpha3_ * std::abs(dy_robot) + alpha4_ * std::abs(dtheta_robot) + alpha5_ * std::abs(dx_robot);
            double std_theta = alpha1_ * std::abs(dtheta_robot) + alpha2_ * (std::abs(dx_robot) + std::abs(dy_robot));

            double noisy_dx = dx_robot - sample_normal(std_forward);
            double noisy_dy = dy_robot - sample_normal(std_left);
            double noise_dtheta = dtheta_robot - sample_normal(std_theta);

            // apply to the particle pose
            Pose x_new;
            x_new.x = x_old.x + noisy_dx * std::cos(x_old.theta) - noisy_dy * std::sin(x_old.theta);
            x_new.y = x_old.y + noisy_dx * std::sin(x_old.theta) + noisy_dy * std::cos(x_old.theta);
            x_new.theta = x_old.theta + noise_dtheta;

            // normalize theta to [-PI, PI]
            while (x_new.theta >  M_PI) x_new.theta -= 2.0 * M_PI;
            while (x_new.theta < -M_PI) x_new.theta += 2.0 * M_PI;

            return x_new;
        }
    }

    double MotionModel::sample_normal(double sd) {
        if (sd <= 0.0) return 0.0;
        std::normal_distribution<double> dist(0, sd); // gaussian about the 0
        return dist(gen_);
    }
} // namespace semantic_mcl