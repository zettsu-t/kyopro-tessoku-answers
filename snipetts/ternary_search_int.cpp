#include <algorithm>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using FuncResult = long long int;
    using Func = FuncResult(&)(Num);
    using SearchResult = std::pair<FuncResult,Num>;
}

SearchResult ternary_search(Num lower, Num upper, Func func) {
    Num left {lower};
    Num right {upper};

    while(std::abs(left - right) > 3) {
        const Num midleft = (left * 2 + right) / 3;
        const Num midright = (left + right * 2) / 3;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    std::vector<SearchResult> ps;
    for(Num i{left}; i<=right; ++i) {
        ps.emplace_back(func(i), i);
    }

    return *std::ranges::min_element(ps);
}

namespace {
    constexpr Num Xoffset = 567;

    const std::vector<std::pair<Num,Num>> test_case_ranges {
        {-1000000000000000LL, -500000000000000LL},
        {-1000000000000000LL, 500000000000000LL},
        {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {-1, 0}, {0, 0}, {0, 1}
    };

    Num func_increase(Num x) {
        return x * 2;
    }

    Num func_decrease(Num x) {
        return -x * 3;
    }

    Num func_flat(Num x) {
        return -45678;
    }

    Num func_hull(Num x) {
        return std::abs(x - Xoffset);
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Increase) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search(lower, upper, func_increase);
        EXPECT_EQ(lower * 2, value);
        EXPECT_EQ(lower, arg);
    }
}

TEST_F(TestAll, Decrease) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search(lower, upper, func_decrease);
        EXPECT_EQ(-upper * 3, value);
        EXPECT_EQ(upper, arg);
    }
}

TEST_F(TestAll, Flat) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search(lower, upper, func_flat);
        EXPECT_EQ(-45678, value);
        EXPECT_EQ(lower, arg);
    }
}

TEST_F(TestAll, Hull) {
    const std::vector<std::tuple<Num,Num,Num,Num>> test_case {
        {Xoffset - 2, Xoffset + 3, 0, Xoffset},
        {Xoffset - 2, Xoffset + 2, 0, Xoffset},
        {Xoffset - 2, Xoffset + 1, 0, Xoffset},
        {Xoffset - 2, Xoffset, 0, Xoffset},
        {Xoffset - 3, Xoffset, 0, Xoffset},
        {Xoffset - 4, Xoffset, 0, Xoffset},
        {Xoffset - 1, Xoffset - 1, 1, Xoffset - 1},
        {Xoffset - 1, Xoffset, 0, Xoffset},
        {Xoffset - 1, Xoffset + 1, 0, Xoffset},
        {Xoffset + 1, Xoffset + 1, 1, Xoffset + 1},
        {Xoffset, Xoffset, 0, Xoffset},
        {Xoffset, Xoffset + 1, 0, Xoffset},
        {Xoffset, Xoffset + 2, 0, Xoffset},
        {Xoffset, Xoffset + 3, 0, Xoffset},
    };

    for(const auto& [lower, upper, expected_value, expected_arg] : test_case) {
        const auto [value, arg] = ternary_search(lower, upper, func_hull);
        EXPECT_EQ(expected_value, value);
        EXPECT_EQ(expected_arg, arg);
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
