#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

// 最小素因数を求める。
using Num = long long int;
using Vec = std::vector<Num>;

void linear_sieve(Num upper, Vec& least_primes, Vec& primes) {
    Vec empty_least_primes(upper + 1);
    std::swap(empty_least_primes, least_primes);

    Vec empty_primes;
    std::swap(empty_primes, primes);

    for(Num i{2}; i<=upper; ++i) {
        if (least_primes.at(i) == 0) {
            least_primes.at(i) = i;
            primes.push_back(i);
        }

        for(const auto& prime : primes) {
            const auto prod = prime * i;
            if ((prod > upper) || (prime > least_primes.at(i))) {
                break;
            }
            least_primes.at(prod) = prime;
        }
    }
}

std::map<Num,Num> prime_factorize(Num x, const Vec& least_primes) {
    Num rem {x};
    std::map<Num,Num> table;

    while(rem > 1) {
        const auto p = least_primes.at(rem);
        table[p] += 1;
        rem /= p;
    }

    return table;
}

const Vec full_primes {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

class TestLinearSieve : public ::testing::Test {};

TEST_F(TestLinearSieve, LeastPrimes) {
    const Vec expected {0, 0, 2, 3, 2, 5, 2, 7, 2, 3, 2};
    Vec least_primes;
    Vec primes;
    linear_sieve(10, least_primes, primes);
    ASSERT_EQ(expected, least_primes);
}

TEST_F(TestLinearSieve, LeastPrimesFull) {
    constexpr Num upper = 100;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    Vec expected(upper + 1);
    for(Num i{2}; i<=upper; ++i) {
        for(const auto& prime : full_primes) {
            if ((i % prime) == 0) {
                expected.at(i) = prime;
                break;
            }
        }
    }

    ASSERT_EQ(expected, least_primes);
}

TEST_F(TestLinearSieve, Primes) {
    for(Num i{2}; i<100; ++i) {
        Vec least_primes;
        Vec primes;
        linear_sieve(i, least_primes, primes);

        auto it = std::ranges::upper_bound(full_primes, i);
        Vec expected;
        std::copy(full_primes.begin(), it, std::back_inserter(expected));
        ASSERT_EQ(expected, primes);
    }
}

TEST_F(TestLinearSieve, Large) {
    constexpr Num upper = 10000009;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);
    ASSERT_EQ(least_primes.size(), upper + 1);
}

class TestPrimeFactorize : public ::testing::Test {};

TEST_F(TestPrimeFactorize, Small) {
    constexpr Num upper = 10;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    std::vector<std::map<Num,Num>> expected {
        {}, {}, {{2,1}}, {{3,1}}, {{2,2}}, {{5,1}},
        {{2,1},{3,1}}, {{7,1}}, {{2,3}}, {{3,2}}, {{2,1},{5,1}}
    };

    for(Num i{2}; i<=2; ++i) {
        const auto actual = prime_factorize(i, least_primes);
        ASSERT_EQ(expected.at(i), actual);
    }
}

TEST_F(TestPrimeFactorize, Auto) {
    constexpr Num upper = 100;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    for(Num i{2}; i<100; ++i) {
        Num rem {i};
        std::map<Num,Num> expected;
        for(const auto& prime : primes) {
            while ((rem % prime) == 0) {
                expected[prime] += 1;
                rem /= prime;
            }
        }

        const auto actual = prime_factorize(i, least_primes);
        ASSERT_EQ(expected, actual);
    }
}

TEST_F(TestPrimeFactorize, Large) {
    constexpr Num upper = 10000009;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    constexpr Num size = 200000;
    std::vector<std::map<Num,Num>> ans;
    for(Num i{upper-size+1}; i<=upper; ++i) {
        ans.push_back(prime_factorize(i, least_primes));
    }

    ASSERT_EQ(ans.size(), size);
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
