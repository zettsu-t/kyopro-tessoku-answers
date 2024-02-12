#include <numeric>
#include <gtest/gtest.h>

using Num = long long int;

// 各種実装を参考にした
Num extended_gcd(Num a, Num b, Num& x, Num& y) {
    Num d = a;
    if (b != 0) {
        d = extended_gcd(b, a % b, y, x);
        y -= (a / b) * x;
    } else {
        x = 1;
        y = 0;
    }

    return d;
}

const std::vector<std::pair<Num, Num>> signs {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

Num abs_gcd(Num a, Num b) {
    return std::gcd(std::abs(a), std::abs(b));
}

Num inner_product(Num a, Num b, Num x, Num y) {
    return a * x + b * y;
}

class TestAbsGcd : public ::testing::Test {};

TEST_F(TestAbsGcd, MutuallyPrime) {
    for(const auto& [sign_a, sign_b] : signs) {
        Num a {3};
        Num b {5};
        EXPECT_EQ(1, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(1, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Even) {
    for(const auto& [sign_a, sign_b] : signs) {
        Num a {6};
        Num b {10};
        EXPECT_EQ(2, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(2, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Power) {
    for(const auto& [sign_a, sign_b] : signs) {
        constexpr Num expected {3};
        Num a {expected};
        Num b = a * a * a;
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Zero) {
    for(const auto& [sign_a, sign_b] : signs) {
        constexpr Num expected {4};
        Num a {expected};
        Num b {0};
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
    }
}

class TestExtendedGcd : public ::testing::Test {};

TEST_F(TestExtendedGcd, MutuallyPrime) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const std::vector<Num> expected_y_signs {-1, 1, 1, -1};
    const Num base_a {3};
    const Num base_b {5};

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        ASSERT_GT(expected_y_signs.size(), trial);

        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const auto& y_sign = expected_y_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(2, x);
        EXPECT_EQ(1 * y_sign, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Even) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const std::vector<Num> expected_y_signs {-1, 1, 1, -1};
    const Num base_a {6};
    const Num base_b {10};

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        ASSERT_GT(expected_y_signs.size(), trial);

        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const auto& y_sign = expected_y_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(2, x);
        EXPECT_EQ(1 * y_sign, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Power) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const Num base_a {3};
    const Num base_b = base_a * base_a * base_a;

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, x);
        EXPECT_EQ(0, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Zero) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const Num base_a {4};
    const Num base_b = 0;

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, x);
        EXPECT_EQ(0, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
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
