#include "semantic_mcl/semantic_model.hpp"


namespace semantic_mcl {
    SemanticModel::SemanticModel(semantic_mcl::OccupancyMap map, const std::string& landmark_json_path, double sigma_bearing, double sigma_range, bool use_range) :
        map_(map), sigma_bearing_(sigma_bearing), sigma_range_(sigma_range), use_range_(use_range) {
        // open up the landmark json path
        std::ifstream file(landmark_json_path);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << landmark_json_path << "\n";
        }

        nlohmann::json data;
        try {
            file >> data;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
        }

        for (const auto& item: data["landmarks"]) {
            if (!item.contains("label") || !item.contains("x") || !item.contains("y")) { // if any of the item doesnt contain the label, x or y
                std::cerr << "Skipping malformed landmark entry\n";
                continue;
            }

            Landmark lm;
            lm.label = item["label"].get<std::string>();
            lm.x = item["x"].get<double>();
            lm.y = item["y"].get<double>();

            this->landmarks_.push_back(lm);
        }
    }

    double SemanticModel::compute_weight(double x, double y, double theta, const std::vector<SemanticDetection>& detections) {
        // multiplication of weights across detections, tracking of the best score in p
        double p = 1.0; 
        // go through each detection in the vector
        for (const auto& det: detections) {
            bool label_in_db = false; // label not in database
            double best_score = 1e-3;
            for (const Landmark& lm : this->landmarks_) { // first phase to check if he object vlm detected is in the landmark json file
                if (lm.label == det.label) {
                    label_in_db = true;
                    break;
                }
            }
            if (!label_in_db)
                continue;

            for (const Landmark& lm : this->landmarks_) { // second phase to find the best visible match
                // check if the detection matches the landmark and if the landmark is visible from robot pose
                if (lm.label == det.label && this->is_visible(x, y, lm.x, lm.y)) {
                    // compute the bearing and range
                    double expected_bearing = atan2(lm.y - y, lm.x - x) - theta;
                    double expected_range = sqrt(std::pow(lm.x - x, 2) + std::pow(lm.y - y, 2));
                    // get the bearing error and range error
                    double bearing_error = det.bearing - expected_bearing;
                    double range_error = det.range - expected_range;
                    // wrap the bearing error between -pi and pi
                    bearing_error = atan2(sin(bearing_error), cos(bearing_error));
                    // score the match using gaussian
                    double p_bearing = exp(-0.5 * std::pow((bearing_error/this->sigma_bearing_), 2));
                    double current_p = p_bearing;
                    if (this->use_range_) {
                        double p_range = exp(-0.5 * std::pow((range_error/this->sigma_range_), 2));
                        current_p *= p_range;
                    }
                    if (current_p > best_score)
                        best_score = current_p;
                }
            }
            p *= best_score;
        }
        return p;
    }

    bool SemanticModel::is_visible(double px, double py, double lx, double ly) {
        double angle = atan2(ly - py, lx - px); // direction from particle to landmark
        double distance = sqrt(std::pow(lx - px, 2) + std::pow(ly - py, 2)); // displacement from particle to landmark

        double t = this->map_.resolution * 0.5; // skip own particle cell
        double step = this->map_.resolution * 0.5;
        // step along the angle 
        // hit an occupied cell, return false as landmark can't be reached
        // return true if landmark can vbe reached without obstruction
        while (t <= distance) {
            double rx = px + (t * cos(angle));
            double ry = py + (t * sin(angle));
            // convert rx and ry from world to grid
            int row, column;
            bool world_to_grid_success = this->map_.world_to_grid(rx, ry, row, column);
            
            // if the ray leaves the map, landmark is definitely not visible
            if (!world_to_grid_success)
                return false;
            // if the ray hits a wall
            if (this->map_.is_occupied(column, row))
                return false;

            t += step;
        }
        return true;
    }
} // namespace semantic_mcl