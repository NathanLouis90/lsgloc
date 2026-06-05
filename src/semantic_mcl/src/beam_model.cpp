#include "semantic_mcl/beam_model.hpp"

namespace semantic_mcl {
        BeamModel::BeamModel(double z_hit, double z_short, double z_max, double z_rand, double sigma_hit, double lambda_short, double z_max_val) 
            : z_hit_(z_hit), z_short_(z_short), z_max_(z_max), z_rand_(z_rand),
           sigma_hit_(sigma_hit), lambda_short_(lambda_short), z_max_val_(z_max_val)
        {}

        double BeamModel::calculate_probability(double z_obs, double z_exp) const { // this is the formula p(z | m, x) where z is the observation, m is the map and x is the particle pose
            return z_hit_ * p_hit(z_obs, z_exp) + z_short_ * p_short(z_obs, z_exp) + z_max_ * p_max(z_obs) + z_rand_ * p_rand(z_obs);
        }

        double BeamModel::p_hit(double z, double z_exp) const {
            if (z < 0 || z > z_max_val_) return 0.0;
            // center the gaussian at the 0 point 
            double exponent = -0.5 * std::pow((z - z_exp) / sigma_hit_, 2);
            double norm_factor = 1.0 / (std::sqrt(2.0 * M_PI) * sigma_hit_);

            return norm_factor * std::exp(exponent);
        }
        double BeamModel::p_short(double z, double z_exp) const {
            if (z < 0 || z > z_exp) return 0.0;
            // this is for exponential distribution
            if (z_exp < 1e-6) return 0.0; // return early if expected beam is small distance otherwise the denominator would return 0 as exp(0) is 1
            double eta = 1.0 / (1.0 - std::exp(-lambda_short_ * z_exp));
            return eta * lambda_short_ * std::exp(-lambda_short_ * z);
        }

        double BeamModel::p_max(double z) const {
            // narrow spike at z_max — width epsilon so the distribution integrates to ~1.0
            const double epsilon = 0.01;
            return (z >= z_max_val_ - epsilon) ? 1.0 / epsilon : 0.0;
        }

        double BeamModel::p_rand(double z) const {
            if (z < 0 || z > z_max_val_) return 0.0;
            // this is uniform distribution
            return 1.0/z_max_val_;
        }

        double BeamModel::get_z_max_value_() const {
            return this->z_max_val_;
        }
}