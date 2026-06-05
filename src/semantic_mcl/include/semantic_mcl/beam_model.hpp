#ifndef SEMANTIC_MCL_BEAM_MODEL_HPP
#define SEMANTIC_MCL_BEAM_MODEL_HPP

#include <cmath>
#include <algorithm>

namespace semantic_mcl {
    class BeamModel {
    public:
        /**
         * @param z_hit    mixture weight for correct range hits (Gaussian centred at z_exp). Must sum to 1.0 with others.
         * @param z_short  mixture weight for unexpectedly short readings (dynamic obstacles in front of expected surface)
         * @param z_max    mixture weight for max-range / sensor miss returns
         * @param z_rand   mixture weight for random noise (uniform across [0, z_max_val])
         * @param sigma_hit    standard deviation of the Gaussian for z_hit
         * @param lambda_short rate parameter of the exponential distribution for z_short (higher = assumes obstacles are close)
         * @param z_max_val    maximum range of the LiDAR sensor in metres
         */
        BeamModel(double z_hit, double z_short, double z_max, double z_rand, double sigma_hit, double lambda_short, double z_max_val);

        /**
         * @brief Computes the unnormalized beam likelihood p(z_obs | x, m).
         *        This is a 4-mixture model. The return value is used for relative
         *        particle weighting only — it is NOT bounded to [0, 1].
         * @param z_obs observed range from the LiDAR
         * @param z_exp expected range from ray-casting against the OGM at the particle pose
         * @return unnormalized likelihood weight
         */
        double calculate_probability(double z_obs, double z_exp) const;
        /**
         * @brief a getter helper function
         */
        double get_z_max_value_() const;
    private:
        double z_hit_;
        double z_short_;
        double z_max_;
        double z_rand_;
        double sigma_hit_;
        double lambda_short_;
        double z_max_val_;

        // helper functions
        /**
         * @brief add gaussian distribution to observation
         */
        double p_hit(double z, double z_exp) const;

        /**
         * @brief add exponential distribution to observation
         */
        double p_short(double z, double z_exp) const;

        /**
         * @brief add a unimodal distribution
         */
        double p_max(double z) const;

        /**
         * @brief add uniform distribution
         */
        double p_rand(double z) const;
    };
} // namespace semantic_mcl
#endif