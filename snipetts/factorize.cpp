#include <algorithm>
#include <iostream>
#include <map>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using Factors = std::vector<Num>;
    using FactorMap = std::map<Num, Num>;
}

FactorMap prime_factorize(Num x) {
    FactorMap factors;
    auto r = x;

    for(Num p{2}; (p*p)<=x; ++p) {
        while((r % p) == 0) {
            factors[p] += 1;
            r /= p;
        }

        if (r == 1) {
            break;
        }
    }

    if (r > 1) {
        factors[r] = 1;
    }

    return factors;
}

Factors factorize(Num x) {
    Factors factors;
    for(Num p{1}; (p*p)<=x; ++p) {
        if ((x % p) != 0) {
            continue;
        }

        factors.push_back(p);
        const auto q = x / p;
        if (q != p) {
            factors.push_back(q);
        }
    }

    std::sort(factors.begin(), factors.end());
    auto it = std::unique(factors.begin(), factors.end());
    factors.erase(it, factors.end());
    return factors;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, PrimeFactorize1) {
    auto ps = prime_factorize(1);
    ASSERT_EQ(0LL, ps.size());
}

// https://t5k.org/lists/2small/0bit.html
TEST_F(TestAll, PrimeFactorizePrimes) {
    auto ps2 = prime_factorize(2);
    ASSERT_EQ(1LL, ps2.size());
    ASSERT_EQ(1LL, ps2[2]);

    auto ps3 = prime_factorize(3);
    ASSERT_EQ(1LL, ps3.size());
    ASSERT_EQ(1LL, ps3[3]);

    Num p {1};
    p <<= 31;
    p -= 1;
    auto ps = prime_factorize(p);
    ASSERT_EQ(1LL, ps.size());
    ASSERT_EQ(1LL, ps[p]);
}

TEST_F(TestAll, PrimeFactorizePowered) {
    auto ps8 = prime_factorize(8);
    ASSERT_EQ(1LL, ps8.size());
    ASSERT_EQ(3LL, ps8[2]);

    Num p {1};
    p <<= 16;
    p -= 15;
    auto ps = prime_factorize(p*p);
    ASSERT_EQ(1LL, ps.size());
    ASSERT_EQ(2LL, ps[p]);
}

TEST_F(TestAll, PrimeFactorizeCompound) {
    // 2*2*2*3*3*3*3*5
    auto ps = prime_factorize(3240);
    ASSERT_EQ(3LL, ps.size());
    ASSERT_EQ(3LL, ps[2]);
    ASSERT_EQ(4LL, ps[3]);
    ASSERT_EQ(1LL, ps[5]);
}

TEST_F(TestAll, Factorize1) {
    const auto actual = factorize(1);
    const Factors expected {1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, FactorizePrimes) {
    std::vector<Num> input {2,3,5,7};
    for(const auto& p : input) {
        const auto actual = factorize(p);
        const Factors expected {1,p};
        ASSERT_EQ(expected.size(), actual.size());
        ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));

        for(Num i{1}; i<=5; ++i) {
            Factors expected {1};
            Num n {1};
            for(Num j{0}; j<i; ++j) {
                n *= p;
                expected.push_back(n);
            }

            const auto actual = factorize(n);
            ASSERT_EQ(expected.size(), actual.size());
            ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
        }
    }
}

TEST_F(TestAll, FactorizeOthers) {
    const auto actual = factorize(360);
    const Factors expected {
        1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20,
        24, 30, 36, 40, 45, 60, 72, 90, 120, 180, 360
    };
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
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
