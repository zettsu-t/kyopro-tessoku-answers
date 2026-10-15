#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

namespace {
    using Num = int;
}

std::pair<long long int, long long int>
str_to_fixed_point(const std::string& s, size_t n_frac_digits) {
    using Int = long long int;
    const auto pos = s.find('.');
    if (pos == std::string::npos) {
        std::pair<Int, Int> num {std::atoll(s.c_str()), 0};
        return num;
    }

    const auto int_part_str = s.substr(0, pos);
    const Int int_part = std::atoll(int_part_str.c_str());

    const auto len = std::min(s.size() - pos - 1, n_frac_digits);
    const auto frac_part_str = s.substr(pos + 1, len);
    Int frac_part = std::atoll(frac_part_str.c_str());

    for(size_t i = frac_part_str.size(); i < n_frac_digits; ++i) {
        frac_part *= 10;
    }

    std::pair<Int, Int> num {int_part, frac_part};
    return num;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Zero) {
    const std::vector<std::string> input {
        "", ".", "0", "0.", "0.0", "0.00", "0.000",
        "00", "00.", "00.0", "00.00", "00.000"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(0LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestAll, One) {
    const std::vector<std::string> input {
        "1", "1.", "1.0", "1.00", "01", "01.", "01.0", "01.00"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(1LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestAll, Int) {
    const std::vector<std::string> input {
        "987", "987.", "987.0", "987.00", "0987", "0987.", "0987.0", "0987.00"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(987LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestAll, Frac) {
    const std::string input {"76.543"};
    const long long int expected_int = 76;

    const auto [int_part0, frac_part0] = str_to_fixed_point(input, 0);
    EXPECT_EQ(expected_int, int_part0);
    EXPECT_EQ(0LL, frac_part0);

    const auto [int_part1, frac_part1] = str_to_fixed_point(input, 1);
    EXPECT_EQ(expected_int, int_part1);
    EXPECT_EQ(5LL, frac_part1);

    const auto [int_part2, frac_part2] = str_to_fixed_point(input, 2);
    EXPECT_EQ(expected_int, int_part2);
    EXPECT_EQ(54LL, frac_part2);

    long long int expected_frac = 543;
    for(size_t n_frac_digits {3}; n_frac_digits < 7; ++n_frac_digits) {
        const auto [int_part, frac_part] = str_to_fixed_point(input, n_frac_digits);
        EXPECT_EQ(expected_int, int_part);
        EXPECT_EQ(expected_frac, frac_part);
        expected_frac *= 10;
    }
}

TEST_F(TestAll, Sample) {
    const auto [int_part, frac_part] = str_to_fixed_point("65.4321", 4);
    EXPECT_EQ(65LL, int_part);
    EXPECT_EQ(4321LL, frac_part);
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
