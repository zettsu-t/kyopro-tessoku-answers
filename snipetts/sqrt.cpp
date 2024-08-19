#include <cmath>
#include <algorithm>
#include <ranges>
#include <gtest/gtest.h>

template<typename T>
T isqrt(const T& x) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    auto pred = [&](const auto& thr) {
        return (thr <= x) && ((thr * thr) <= x);
    };

    T upper {1};
    upper <<= (sizeof(T) * 4);
    upper -= 1;
    return *std::ranges::partition_point(std::views::iota(T{0}, upper), pred) - 1;
}

template<typename T, typename U>
T inverse_pow(const T& x, const U& n) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    if (n == 0) {
        return (x == 0) ? 0 : 1;
    }

    if (n == 1) {
        return x;
    }

    if (n == 2) {
        return isqrt(x);
    }

    auto pred = [&](const auto& thr) {
        T prod {1};
        for(U i{0}; i<n; ++i) {
            prod *= thr;
            if (prod > x) {
                return false;
            }
        }

        return true;
    };

    double inv_n {1};
    inv_n /= n;
    const T center = std::floor(std::pow(x, inv_n));
    const T lower = std::max(T{0}, center - 2);
    const T upper = std::max(lower, center + 3);
    return *std::ranges::partition_point(std::views::iota(lower, upper + 1), pred) - 1;
}

class TestSqrt : public ::testing::Test {};

TEST_F(TestSqrt, Negative) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, isqrt(Int{-1}));
}

TEST_F(TestSqrt, Low) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, isqrt(Int{0}));
    EXPECT_EQ(Int{1}, isqrt(Int{1}));

    for(Int i{2}; i<=102; ++i) {
        const Int actual = isqrt(i);
        Int expected {0};
        for(Int j{0}; j<=i; ++j) {
            if ((j * j) <= i) {
                expected = j;
            } else {
                break;
            }
        }

        EXPECT_EQ(expected, actual);
    }
}

TEST_F(TestSqrt, Int8) {
    using Int = int8_t;
    EXPECT_EQ(Int{10}, isqrt(Int{120}));

    Int i {121};
    for(;;) {
        EXPECT_EQ(Int{11}, isqrt(Int{i}));
        if (i == 127) {
            break;
        }
        ++i;
    }
}

TEST_F(TestSqrt, Uint8) {
    using Int = uint8_t;
    Int ui{121};
    for(; ui<=128; ++ui) {
        EXPECT_EQ(Int{11}, isqrt(Int{ui}));
    }

    ui = 196;
    for(; ui<225; ++ui) {
        EXPECT_EQ(Int{14}, isqrt(Int{ui}));
    }
}

TEST_F(TestSqrt, IntMax) {
    using Int = int64_t;
    Int expected = 0x7ffffffeLL;
    std::vector<Int> inputs {4611686009837453316LL, 4611686014132420608LL};
    for(const auto& input : inputs) {
        EXPECT_EQ(Int{expected}, isqrt(Int{input}));
        EXPECT_TRUE((expected * expected) <= input);
        EXPECT_TRUE(((expected + 1) * (expected + 1)) > input);
    }
}

TEST_F(TestSqrt, UIntMax) {
    using Int = uint64_t;
    Int expected = 0xfffffffeULL;
    std::vector<Int> inputs {18446744056529682436ULL, 18446744065119617024ULL};
    for(const auto& input : inputs) {
        EXPECT_EQ(Int{expected}, isqrt(Int{input}));
        EXPECT_TRUE((expected * expected) <= input);
        EXPECT_TRUE(((expected + 1) * (expected + 1)) > input);
    }
}

class TestInversePow : public ::testing::Test {};

TEST_F(TestInversePow, Negative) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, inverse_pow(Int{-1}, 0));
}

TEST_F(TestInversePow, Zero) {
    using Int = int64_t;
    EXPECT_EQ(Int{0}, inverse_pow(0, 0));

    for(Int base{1}; base<=100; ++base) {
        EXPECT_EQ(Int{1}, inverse_pow(base, 0));
    }
}

TEST_F(TestInversePow, One) {
    using Int = int64_t;
    for(Int base{0}; base<=100; ++base) {
        EXPECT_EQ(base, inverse_pow(base, 1));
    }
}

TEST_F(TestInversePow, Low) {
    using Int = int64_t;

    for(Int index{2}; index<=10; ++index) {
        for(Int base{0}; base<=10; ++base) {
            Int arg {1};
            for(Int i{0}; i<index; ++i) {
                arg *= base;
            }

            const Int actual = inverse_pow(arg, index);
            EXPECT_EQ(base, actual);
        }
    }
}

TEST_F(TestInversePow, Typical) {
    using Int = int64_t;
    constexpr Int arg = 1000000000000000000LL;
    EXPECT_EQ(Int{1000000000LL}, inverse_pow(arg, 2));
    EXPECT_EQ(Int{1000000LL}, inverse_pow(arg, 3));
    EXPECT_EQ(Int{1000LL}, inverse_pow(arg, 6));
    EXPECT_EQ(Int{100LL}, inverse_pow(arg, 9));
    EXPECT_EQ(Int{10LL}, inverse_pow(arg, 18));
    EXPECT_EQ(Int{31622LL}, inverse_pow(arg, 4));
    EXPECT_EQ(Int{3981LL}, inverse_pow(arg, 5));
}

TEST_F(TestInversePow, Pow3) {
    using Int = int64_t;
    constexpr int n = 3;

    for(Int i{1000000 - 11*100}; i<=(1000000 + 11*100); i+=11) {
        Int arg {1};

        for(int j{0}; j<n; ++j) {
            arg *= i;
        }

        for(Int ofs{-20}; ofs<=20; ++ofs) {
            const Int expected = i - (ofs < 0);
            EXPECT_EQ(expected, inverse_pow(arg + ofs, n));
        }
    }
}

TEST_F(TestInversePow, Pow6) {
    using Int = int64_t;
    constexpr int n = 6;

    for(Int i{1000- 3*100}; i<=(1000 + 3*100); i+=3) {
        Int arg {1};

        for(int j{0}; j<n; ++j) {
            arg *= i;
        }

        for(Int ofs{-20}; ofs<=20; ++ofs) {
            const Int expected = i - (ofs < 0);
            EXPECT_EQ(expected, inverse_pow(arg + ofs, n));
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
