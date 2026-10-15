#include <algorithm>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Numeric = double;
    using Func = Numeric(&)(Numeric);
}

Numeric ternary_search(Numeric lower, Numeric upper, Func func) {
    constexpr Numeric tolerance = 1e-15;
    Numeric left {lower};
    Numeric right {upper};

    while(std::abs(left - right) > tolerance) {
        Numeric relative = std::max(std::abs(left), std::abs(right));
        relative = std::abs(left - right) / std::max(relative, tolerance);
        if (relative < tolerance) {
            break;
        }

        Numeric midleft = (left * 2.0 + right) / 3.0;
        Numeric midright = (left + right * 2.0) / 3.0;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    return left;
}

namespace {
    constexpr Numeric x_origin = 1.5;
    constexpr Numeric y_origin = -2.5;
    constexpr Numeric test_tolerance = 1e-6;
}

Numeric func1(Numeric x) {
    const auto w = x - x_origin;
    return w * w + y_origin;
}

Numeric func0(Numeric x) {
    return x * x;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Origin) {
    constexpr auto lower = x_origin;
    constexpr auto upper = x_origin;
    const auto actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);
    EXPECT_NEAR(y_origin, func1(actual), test_tolerance);
}

TEST_F(TestAll, Min) {
    Numeric lower = x_origin - 2.0;
    Numeric upper = x_origin + 2.0;
    auto actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);

    lower = x_origin - 10.0;
    actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);

    upper = x_origin + 20.0;
    actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);
}

TEST_F(TestAll, MonotonicLeft) {
    constexpr auto lower = x_origin - 2.0;
    auto upper = x_origin - 1.0;
    auto actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(upper, actual, test_tolerance);

    upper = x_origin;
    actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(upper, actual, test_tolerance);
}

TEST_F(TestAll, MonotonicRight) {
    auto lower = x_origin + 1.0;
    constexpr auto upper = x_origin + 2.0;
    auto actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(lower, actual, test_tolerance);

    lower = x_origin;
    actual = ternary_search(lower, upper, func1);
    EXPECT_NEAR(lower, actual, test_tolerance);
}

TEST_F(TestAll, Zero) {
    constexpr auto zero = 0.0;
    auto actual = ternary_search(zero, zero, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);
    EXPECT_NEAR(zero, func0(actual), test_tolerance);

    actual = ternary_search(-1.0, 1.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search(-2.0, 3.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search(-2.0, zero, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search(zero, 2.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
Local Variables:
mode: c++
coding: utf-8-dos
tab-width: nil
c-file-style: "stroustrup"
End:
*/
