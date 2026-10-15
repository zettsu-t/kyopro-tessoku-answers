#include <limits>
#include <random>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <atcoder/modint.hpp>
#include <gtest/gtest.h>

namespace {
    using Num = int64_t;

    template <typename T=__int128>
    Num power(Num base, Num index, Num mod) {
        const Num x = base % mod;
        if (x == 0) {
            return 0;
        }

        if (x == 1) {
            return 1;
        }

        T acc {1};
        T doubled = x;
        auto i = index;

        while(i > 0) {
            if ((i & 1) != 0) {
                acc *= doubled;
                acc %= mod;
            }

            doubled *= doubled;
            doubled %= mod;
            i /= 2;
        }

        return static_cast<Num>(acc);
    }
}

template <typename T>
class TestAll : public ::testing::Test {};

typedef ::testing::Types<__int128, boost::multiprecision::cpp_int> BigNumTypes;
TYPED_TEST_CASE(TestAll, BigNumTypes);

TYPED_TEST(TestAll, PowerOfTwo) {
    constexpr Num mod {83};
    Num base {1};

    for(Num index{0}; index<62; ++index) {
        const auto actual = power(2, index, mod);
        EXPECT_EQ(base % mod, actual);
        base *= 2;
    }
}

TYPED_TEST(TestAll, ZeroOne) {
    constexpr auto num_max = std::numeric_limits<Num>::max();
    const std::vector<Num> indexes {0, 1, 2, 0x100000001LL, num_max-1, num_max};
    const std::vector<Num> mods {2, 3, 83, 998244353, 1000000007, 0x100000001LL, num_max-1};

    for(const auto& index : indexes) {
        for(const auto& mod : mods) {
            for(Num offset{0}; offset<2; ++offset) {
                const auto max_n = (num_max / mod) - 1;
                if (max_n <= 0) {
                    continue;
                }

                const std::vector<Num> nums {0, mod, mod*2, (max_n-1)*mod, max_n*mod};
                for(const auto& base : nums) {
                    if (base < 0) {
                        continue;
                    }
                    const auto actual = power(base + offset, index, mod);
                    EXPECT_EQ(offset, actual);
                }
            }
        }
    }
}

// https://atcoder.jp/contests/abc228/tasks/abc228_e
TYPED_TEST(TestAll, ABC228E) {
    const auto actual = power(15926535, power(14, 3, 998244352), 998244353);
    EXPECT_EQ(109718301, actual);
}

TYPED_TEST(TestAll, Random) {
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_int_distribution<Num> dist(1000, 1000000000);
    std::uniform_int_distribution<Num> dist_mod(2, 97);

    for(Num trial{0}; trial<20000; ++trial) {
        const Num base = dist(engine);
        const Num index = dist(engine);
        Num mod {0};
        if ((trial & 1) > 0) {
            mod = dist(engine);
        } else {
            mod = dist_mod(engine);
        }

        atcoder::modint::set_mod(mod);
        atcoder::modint x = base;
        const auto expected = x.pow(index).val();

        const auto actual = power(base, index, mod);
        EXPECT_EQ(expected, actual);
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
