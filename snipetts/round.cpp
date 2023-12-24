#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
}

std::pair<Num, Num> ifloor(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = p % q;
        quotient = p / q;
    } else {
        remainder = (q - (std::abs(p) % q)) % q;
        quotient = (p - remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iceil(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = (q - p % q) % q;
        quotient = (p + remainder) / q;
    } else {
        remainder = std::abs(p) % q;
        quotient = (p + remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, -rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iround(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
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
    const Num quotient = sign * (abs_p / q);
    const Num remainder = sign * (abs_p % q);
    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}

struct TestCase {
    Num divisor {0};
    const std::vector<std::pair<Num, Num>> expected;
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, IfloorZeros) {
    const auto [quotient, remainder] = ifloor(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
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
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = ifloor(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((divisor * actual_rem) >= 0);
        }
    }
}

TEST_F(TestAll, IceilZeros) {
    const auto [quotient, remainder] = iceil(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
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
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = iceil(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((divisor * actual_rem) <= 0);
        }
    }
}

TEST_F(TestAll, IroundZeros) {
    const auto [quotient, remainder] = iceil(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
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
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = iround(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((dividend * actual_rem) >= 0);
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
