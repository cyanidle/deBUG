#include "dorLib/dMatrix.h"
// Херня которую тестируем
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ros/ros.h"

TEST(Matrix, basicOperations)
{
    dMatrix<float, 3, 3> mat{1,1,1,1,1,1,1,1,1};
    mat += 2;
    EXPECT_EQ(mat.at(2,2), 3);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "tester");
    ros::NodeHandle nh;
    return RUN_ALL_TESTS();
}