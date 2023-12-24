#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
}

std::pair<Num, Num> ifloor(Num denominator, Num divider) {
    Num p = denominator;
    Num q = divider;

    Num rem_sign = 1;
    if (divider < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num rem {0};
    Num div {0};
    if (p >= 0) {
        rem = p % q;
        div = p / q;
    } else {
        rem = (q - (std::abs(p) % q)) % q;
        div = (p - rem) / q;
    }

    const std::pair<Num, Num> answer {div, rem_sign * rem};
    return answer;
}

std::pair<Num, Num> iceil(Num denominator, Num divider) {
    Num p = denominator;
    Num q = divider;

    Num rem_sign = 1;
    if (divider < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num rem {0};
    Num div {0};
    if (p >= 0) {
        rem = (q - p % q) % q;
        div = (p + rem) / q;
    } else {
        rem = std::abs(p) % q;
        div = (p + rem) / q;
    }

    const std::pair<Num, Num> answer {div, -rem_sign * rem};
    return answer;
}

std::pair<Num, Num> iround(Num denominator, Num divider) {
    Num p = denominator;
    Num q = divider;

    Num rem_sign = 1;
    if (divider < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    const Num sign = (p > 0) ? 1 : -1;
    const Num abs_p = std::abs(p);
    const Num div = sign * (abs_p / q);
    const Num rem = sign * (abs_p % q);
    const std::pair<Num, Num> answer {div, rem_sign * rem};
    return answer;
}

struct TestCase {
    Num divider {0};
    const std::vector<std::pair<Num, Num>> expected;
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, IfloorZeros) {
    const auto [div, rem] = ifloor(1LL, 0LL);
    ASSERT_EQ(0, div);
    ASSERT_EQ(0, rem);
}

TEST_F(TestAll, Ifloor) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-5, 1}, {-4, 0}, {-4, 1}, {-3, 0}, {-3, 1}, {-2, 0}, {-2, 1}, {-1, 0}, {-1, 1},
          {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}, {3, 0}, {3, 1}, {4, 0}, {4, 1}, {5, 0}}},
        {3,
         {{-4, 2}, {-3, 0}, {-3, 1}, {-3, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-1, 0}, {-1, 1}, {-1, 2},
          {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}}},
        {-3,
         {{3, -1}, {3, 0}, {2, -2}, {2, -1}, {2, 0}, {1, -2}, {1, -1}, {1, 0}, {0, -2}, {0, -1},
          {0, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-3, -2}, {-3, -1}, {-3, 0}, {-4, -2}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divider = test_case.divider;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num denominator_index {0};
        for(Num denominator{-width}; denominator<=width; ++denominator, ++denominator_index) {
            const auto [actual_div, actual_rem] = ifloor(denominator, divider);
            const auto [expected_div, expected_rem] = expected.at(denominator_index);
            ASSERT_EQ(expected_div, actual_div);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(denominator, expected_div * divider + expected_rem);
            ASSERT_TRUE((expected_div * denominator * divider) >= 0);
            ASSERT_TRUE((divider * actual_rem) >= 0);
        }
    }
}

TEST_F(TestAll, IceilZeros) {
    const auto [div, rem] = iceil(1LL, 0LL);
    ASSERT_EQ(0, div);
    ASSERT_EQ(0, rem);
}

TEST_F(TestAll, Iceil) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-4, -1}, {-4, 0}, {-3, -1}, {-3, 0}, {-2, -1}, {-2, 0}, {-1, -1}, {-1, 0}, {0, -1},
          {0, 0}, {1, -1}, {1, 0}, {2, -1}, {2, 0}, {3, -1}, {3, 0}, {4, -1}, {4, 0}, {5, -1}, {5, 0}}},
        {3,
         {{-3, -1}, {-3, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {0, -2}, {0, -1},
          {0, 0}, {1, -2}, {1, -1}, {1, 0}, {2, -2}, {2, -1}, {2, 0}, {3, -2}, {3, -1}, {3, 0}, {4, -2}}},
        {-3,
         {{4, 2}, {3, 0}, {3, 1}, {3, 2}, {2, 0}, {2, 1}, {2, 2}, {1, 0}, {1, 1}, {1, 2},
          {0, 0}, {0, 1}, {0, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-3, 0}, {-3, 1}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divider = test_case.divider;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num denominator_index {0};
        for(Num denominator{-width}; denominator<=width; ++denominator, ++denominator_index) {
            const auto [actual_div, actual_rem] = iceil(denominator, divider);
            const auto [expected_div, expected_rem] = expected.at(denominator_index);
            ASSERT_EQ(expected_div, actual_div);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(denominator, expected_div * divider + expected_rem);
            ASSERT_TRUE((expected_div * denominator * divider) >= 0);
            ASSERT_TRUE((divider * actual_rem) <= 0);
        }
    }
}

TEST_F(TestAll, IroundZeros) {
    const auto [div, rem] = iceil(1LL, 0LL);
    ASSERT_EQ(0, div);
    ASSERT_EQ(0, rem);
}

TEST_F(TestAll, Iround) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-4, -1}, {-4, 0}, {-3, -1}, {-3, 0}, {-2, -1}, {-2, 0}, {-1, -1}, {-1, 0}, {0, -1},
          {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}, {3, 0}, {3, 1}, {4, 0}, {4, 1}, {5, 0}}},
        {3,
         {{-3, -1}, {-3, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {0, -2}, {0, -1},
          {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}}},
        {-3,
         {{3, -1}, {3, 0}, {2, -2}, {2, -1}, {2, 0}, {1, -2}, {1, -1}, {1, 0}, {0, -2}, {0, -1},
          {0, 0}, {0, 1}, {0, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-3, 0}, {-3, 1}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divider = test_case.divider;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num denominator_index {0};
        for(Num denominator{-width}; denominator<=width; ++denominator, ++denominator_index) {
            const auto [actual_div, actual_rem] = iround(denominator, divider);
            const auto [expected_div, expected_rem] = expected.at(denominator_index);
            ASSERT_EQ(expected_div, actual_div);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(denominator, expected_div * divider + expected_rem);
            ASSERT_TRUE((expected_div * denominator * divider) >= 0);
            ASSERT_TRUE((denominator * actual_rem) >= 0);
        }
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
