#include "semantic_mcl/particle_filter.hpp"

namespace semantic_mcl {
    ParticleFilter::ParticleFilter(size_t particle_count, OccupancyMap ogm, BeamModel beam_model,
            MotionModel motion_model, double ess_threshold, double w_slow, double w_fast, double alpha_slow, double alpha_fast) :
        particle_count_(particle_count), map_(ogm), beam_model_(beam_model), motion_model_(motion_model), gen_(std::random_device {}()),
        z_max_range_(this->beam_model_.get_z_max_value_()), ess_threshold_(ess_threshold),
        w_slow_(w_slow), w_fast_(w_fast), alpha_slow_(alpha_slow), alpha_fast_(alpha_fast) {}
        // random_device gen can vbe initialized under member list constructor

    void ParticleFilter::init() {
        // clear particles
        this->particles_.clear();

        std::uniform_int_distribution<int> col_dist(0, this->map_.width - 1); // generate a distribution of random numbers for column cells 
        std::uniform_int_distribution<int> row_dist(0, this->map_.height - 1); // generate a distribution of random numbers for row cells 
        std::uniform_real_distribution<double> dist(-M_PI, M_PI); // generate a random distribution of random numbers for theta
        while (this->particles_.size() < this->particle_count_) {
            int random_column_number = col_dist(this->gen_);
            int random_row_number = row_dist(this->gen_);

            if (this->map_.is_free_space(random_column_number, random_row_number)) {
                // grid to world, do take note that these are particles in terms of x and y
                // why we add 0.5 is because it is centered at half the distance of the cell so like exactly half
                double wx = (random_column_number + 0.5) * this->map_.resolution + this->map_.origin_x;
                double wy = (random_row_number + 0.5) * this->map_.resolution  + this->map_.origin_y;
                double theta = dist(this->gen_);
                Particle p {wx, wy, theta, 1.0/this->particle_count_}; // create a particle and then 
                this->particles_.push_back(p);
            }
        }
    }

    void ParticleFilter::predict(const Pose& u_old, const Pose& u_new) {
        for (auto& p: this->particles_) {
            Pose old_pose = {p.x, p.y, p.theta};
            Pose new_pose = this->motion_model_.sample(u_old, u_new, old_pose);
            p.x = new_pose.x;
            p.y = new_pose.y;
            p.theta = new_pose.theta; 
        }
    }

    void ParticleFilter::update(const std::vector<double>& z_obs, const std::vector<double>& beam_angles) {
        // 0. init the first wei
        // 1. init the weight of the particle to be 1.0 so that it is equal probability
        for (auto & p: this->particles_) {
            // 2. for each beam, ray cast from each particle to get expected beam distance, then calculate the probability and then accumulate into the weight
            // since we are focusing on global localization, we will use log-likelihood rather than raw multiplication
            double log_weight = 0.0;
            // p.weight = 1.0 // raw multplication method
            for (size_t i = 0; i < beam_angles.size(); i++) {
                double z_exp = this->ray_cast(p.x, p.y, p.theta, beam_angles[i]);
                log_weight += std::log(std::max(beam_model_.calculate_probability(z_obs[i], z_exp), 1e-10));
                // p.weight *= this->beam_model_.calculate_probability(z_obs[i], z_exp); // raw multplication method
            }
            p.weight *= std::exp(log_weight);
        }

        // 3. normalize weights accross all particles
        // first sum all the particle weights in one loop
        double total_weight = 0.0;
        for (const auto &p : this->particles_)
            total_weight += p.weight;

        // Augmented MCL: track running average of mean weight
        double w_avg = total_weight / this->particle_count_;
        if (!w_initialized_) {
            w_slow_ = w_avg;
            w_fast_ = w_avg;
            w_initialized_ = true;
        } else {
            w_slow_ += alpha_slow_ * (w_avg - w_slow_);
            w_fast_ += alpha_fast_ * (w_avg - w_fast_);
        }

        // then divide each particle weight by the total weight to get normalized weights
        for (auto &p : this->particles_)
            p.weight = p.weight / total_weight;
    }

        // fires a lidar beam at a specific angle and measures the distance to the obstacke it hits first
        // it simulates what the particle expects to measure if it was the actual pose (z_exp)
    double ParticleFilter::ray_cast(double x, double y, double theta, double beam_angle) const { // return z_exp
        // get the angle and determine the increment lidar beam should take every time step t
        double angle = beam_angle + theta;
        double t = 0.0;
        double step = this->map_.resolution * 0.5; // ensure that the beam dont just skip over thin walls, it meaures twice per cell
        while (t < this->z_max_range_) {
            double rx = x + (t * cos(angle));
            double ry = y + (t * sin(angle));
            // convert rx and ry from world to grid
            int row, column;
            bool world_to_grid_success = this->map_.world_to_grid(rx, ry, row, column);

            if (!world_to_grid_success)
                return this->z_max_range_;
            if (this->map_.is_occupied(column, row))
                return t;

            t += step;
        }
        return this->z_max_range_; // return the max range as the ray does not hit any obstacles
    }

    void ParticleFilter::resample() {
        // Augmented MCL: compute injection probability from w_slow/w_fast ratio
        double injection_prob = (w_slow_ > 0.0) ? std::max(0.0, 1.0 - w_fast_/w_slow_) : 0.0;

        // Distributions for random particle generation (same as init())
        std::uniform_int_distribution<int> col_dist(0, this->map_.width - 1);
        std::uniform_int_distribution<int> row_dist(0, this->map_.height - 1);
        std::uniform_real_distribution<double> theta_dist(-M_PI, M_PI);
        std::uniform_real_distribution<double> coin(0.0, 1.0);

        // Low-variance resampling setup
        std::uniform_real_distribution<double> start_dist(0.0, 1.0 / this->particle_count_);
        double r = start_dist(this->gen_);
        double cumulative_weight = this->particles_[0].weight;
        size_t j = 0; // pointer into the old particle set

        std::vector<Particle> new_set_of_particles;
        for (size_t m = 0; m < this->particle_count_; m++) {
            if (coin(this->gen_) < injection_prob) {
                // inject a random free-space particle (retry until free cell found)
                int col, row;
                do {
                    col = col_dist(this->gen_);
                    row = row_dist(this->gen_);
                } while (!this->map_.is_free_space(col, row)); // keep going till you find a free space

                double wx = (col + 0.5) * this->map_.resolution + this->map_.origin_x;
                double wy = (row + 0.5) * this->map_.resolution + this->map_.origin_y;
                double theta = theta_dist(this->gen_);
                new_set_of_particles.push_back(Particle{wx, wy, theta, 1.0 / this->particle_count_});
            } else {
                // normal low-variance resampling
                double u = r + m * (1.0 / this->particle_count_);
                while (u > cumulative_weight && j < this->particle_count_ - 1) {
                    j++;
                    cumulative_weight += this->particles_[j].weight;
                }
                this->particles_[j].weight = 1.0/this->particle_count_;
                new_set_of_particles.push_back(this->particles_[j]);
            }
        }

        this->particles_ = new_set_of_particles;
    }

    Pose ParticleFilter::best_estimate() const {
        double best_estimate_x = 0.0;
        double best_estimate_y = 0.0;
        double best_estimate_sin = 0.0;
        double best_estimate_cos = 0.0;
        double best_estimate_theta = 0.0;

        for (const Particle& p: this->particles_) {
            best_estimate_x += p.x * p.weight;
            best_estimate_y += p.y * p.weight;

            best_estimate_sin += p.weight * sin(p.theta);
            best_estimate_cos += p.weight * cos(p.theta);
        }
        best_estimate_theta = atan2(best_estimate_sin, best_estimate_cos);

        return Pose{best_estimate_x, best_estimate_y, best_estimate_theta};
    }

    std::vector<Particle>& ParticleFilter::get_particle_array() {
        return this->particles_;
    }
}