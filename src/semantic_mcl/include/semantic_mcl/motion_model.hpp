#ifndef SEMANTIC_MCL_MOTION_MODEL_HPP
#define SEMANTIC_MCL_MOTION_MODEL_HPP

// import the libraries
#include <random>
#include <cmath>
#include <vector>

namespace semantic_mcl {
   /**
        * @brief Represents a 2D pose (x, y, theta)
   */
   struct Pose { // descibes the particle position and orientation
        double x;
        double y;
        double theta;
   };

   enum class MotionModelType {
        DIFFERENTIAL_MODEL,
        OMNIDIRECTIONAL_MODEL
   };

   class MotionModel {
    public:
        /**
            @param alpha1 rotation error
            @param alpha2 translation error
            @param alpha3 translation error
            @param alpha4 rotation error
        */
        MotionModel(double a1, double a2, double a3, double a4, MotionModelType type = MotionModelType::DIFFERENTIAL_MODEL); // differential model

        /**
            @param alpha1 rotation error
            @param alpha2 translation error
            @param alpha3 translation error
            @param alpha4 rotation error
            @param alpha5 lateral error
        */
        MotionModel(double a1, double a2, double a3, double a4, double a5, MotionModelType type = MotionModelType::OMNIDIRECTIONAL_MODEL); // omidirectional model

        /**
            @brief sample new pose based on initial pose and odometry change
            @param u_old previous odometry pose in time step t
            @param u_new new odometry pose in time step t + 1
            @param x_old particle previous pose
        */
        Pose sample(const Pose& u_old, const Pose& u_new, const Pose& x_old);

    private:
        double alpha1_, alpha2_, alpha3_, alpha4_, alpha5_;
        MotionModelType type_;
        std::mt19937 gen_; // from mersenne twister engine

        // helper function for gaussian distribution where the mean is 0 and sd stands for standard deviation
        double sample_normal(double sd);
   };
} // namespace semantic_mcl

#endif