#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <sstream>
#include <gtest/gtest.h>

using Num = long long int;

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Setw) {
    const long long int input = 123456;
    const std::string base {"123456"};
    for(int width=0; width<100; ++width) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(width) << input;

        if (width<=6) {
            EXPECT_EQ(base, oss.str());
        } else {
            std::string expected (width-6, '0');
            expected += base;
            EXPECT_EQ(expected, oss.str());
        }
    }
}

TEST_F(TestAll, SetPrecision) {
    const double input = 1.123123123123;
    const std::string base {"1.123123123123"};
    for(int width{1}; width<10; ++width) {
        std::ostringstream oss;
        oss << std::setprecision(width) << input;

        auto len = width;
        len += width > 1;
        const auto expected = base.substr(0, len);
        EXPECT_EQ(expected, oss.str());
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
