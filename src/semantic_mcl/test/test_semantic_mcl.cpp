#include <gtest/gtest.h>
#include "semantic_mcl/beam_model.hpp"

TEST(BeamModelTest, PHitZeroOutOfRange) {
    // test if probability hit is out of range
    double z_hit = 0.7, z_short = 0.1, z_max = 0.1, z_rand = 0.1, sigma_hit = 0.2, lambda_short = 1.0, z_max_val = 10.0; 
    semantic_mcl::BeamModel bm(z_hit, z_short, z_max, z_rand, sigma_hit, lambda_short, z_max_val);
    double result = bm.calculate_probability(-1.0, 5.0);
    EXPECT_NEAR(result, 0.0, 1e-9);
}

TEST(BeamModelTest, ZBeyondMaxZValue) {
    // teST IF Z IS BYOND MAXIMUM Z VALUE ACCEPTED
    double z_hit = 0.7, z_short = 0.1, z_max = 0.1, z_rand = 0.1, sigma_hit = 0.2, lambda_short = 1.0, z_max_val = 10.0; 
    semantic_mcl::BeamModel bm(z_hit, z_short, z_max, z_rand, sigma_hit, lambda_short, z_max_val);
    double result = bm.calculate_probability(11.0, 5.0);
    EXPECT_NEAR(result, 10.0, 1e-9);
}

TEST(BeamModelTest, PShortTest) {
    double z_hit = 0.0, z_short = 1.0, z_max = 0.0, z_rand = 0.0, sigma_hit = 0.2, lambda_short = 1.0, z_max_val = 10.0;
    semantic_mcl::BeamModel bm(z_hit, z_short, z_max, z_rand, sigma_hit, lambda_short, z_max_val);
    double result = bm.calculate_probability(5.0, 1.0); // sensor reads 5 meters but the expected distance from wall to particle pose is 1 meter
    EXPECT_NEAR(result, 0.0, 1e-9);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}