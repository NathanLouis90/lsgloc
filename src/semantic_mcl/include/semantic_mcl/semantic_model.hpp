#ifndef SEMANTIC_MODEL_HPP
#define SEMANTIC_MODEL_HPP

#include <string>
#include "semantic_mcl/particle_filter.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <iostream>

namespace semantic_mcl {
    /**
     * @brief What the robot/particle sees in terms of category, displacement and angle from robot/particle to landmark
     *        as well as the confidence value  
     */
    struct SemanticDetection {
        std::string label;
        double bearing;
        double range;
        double confidence;
    };

    class SemanticModel {
    public:
        /**
         * @brief inputs the map, which tells you what landmarks are occluded, as well as where each individual landmarks are.
         *        also includes the variance to sprinkle some gaussian noise 
         */
        SemanticModel(semantic_mcl::OccupancyMap map, const std::string& landmark_json_path, double sigma_bearing, double sigma_range, bool use_range);
        /**
         * @brief inputs the particle pose and the objects the robot detects
         */
        double compute_weight(double x, double y, double theta, const std::vector<SemanticDetection>& detections);
    private:
        /**
         * @brief checks to see if landmark is visible based on particle pose 
         */
        bool is_visible(double px, double py, double lx, double ly);
        semantic_mcl::OccupancyMap map_;
        std::vector<Landmark> landmarks_;
        double sigma_bearing_;
        double sigma_range_;
        bool use_range_;
    };
} // namespace semantic_mcl

#endif