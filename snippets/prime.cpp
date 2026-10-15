#include <algorithm>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

using Num = long long int;

void find_primes(Num upper, std::vector<Num>& primes,
                 std::vector<bool>& are_primes, std::vector<Num>& n_primes) {
    primes.clear();
    are_primes.clear();
    n_primes.clear();
    are_primes.resize(upper + 1, true);
    n_primes.resize(upper + 1, 0);

    for(Num i{0}; i<=std::min(upper, 1LL); ++i) {
        are_primes.at(i) = false;
    }

    for(Num p{2}; (p*p)<=upper; ++p) {
        if (!are_primes.at(p)) {
            continue;
        }
        for(Num i=p*2; i<=upper; i+=p) {
            are_primes.at(i) = false;
        }
    }

    for(Num p{2}; p<=upper; ++p) {
        if (are_primes.at(p)) {
            primes.push_back(p);
        }
        n_primes.at(p) += n_primes.at(p-1) + are_primes.at(p);
    }
}

bool is_prime(Num n) {
    if (n < 2) {
        return false;
    }

    auto r = n;
    for(decltype(r) p{2}; (p*p)<=n; ++p) {
        if ((r % p) == 0) {
            r /= p;
        }

        if (r <= 1) {
            break;
        }
    }

    return (r == n);
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Clear) {
    std::vector<Num> primes {10, 20};
    std::vector<bool> are_primes {true, false};
    std::vector<Num> n_primes {7, 8};
    constexpr Num upper = 0;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 1);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_TRUE(n_primes.size() == 1);
    ASSERT_TRUE(n_primes.at(0) == 0);
}

TEST_F(TestAll, Primes0) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 0;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 1);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_TRUE(n_primes.size() == 1);
    ASSERT_TRUE(n_primes.at(0) == 0);
}

TEST_F(TestAll, Primes1) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 1;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 2);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_FALSE(are_primes.at(1));

    ASSERT_TRUE(n_primes.size() == 2);
    ASSERT_TRUE(n_primes.at(0) == 0);
    ASSERT_TRUE(n_primes.at(1) == 0);
}

TEST_F(TestAll, Primes2) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 2;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.size() == 1);
    ASSERT_TRUE(primes.at(0) == 2);

    ASSERT_TRUE(are_primes.size() == 3);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_FALSE(are_primes.at(1));
    ASSERT_TRUE(are_primes.at(2));

    ASSERT_FALSE(is_prime(0));
    ASSERT_FALSE(is_prime(1));
    ASSERT_TRUE(is_prime(2));

    ASSERT_TRUE(n_primes.size() == 3);
    ASSERT_TRUE(n_primes.at(0) == 0);
    ASSERT_TRUE(n_primes.at(1) == 0);
    ASSERT_TRUE(n_primes.at(2) == 1);
}

TEST_F(TestAll, PrimesShort) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 37;
    find_primes(upper, primes, are_primes, n_primes);

    const std::vector<Num> expected {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    ASSERT_EQ(expected.size(), primes.size());
    ASSERT_TRUE(std::equal(primes.begin(), primes.end(), expected.begin()));

    Num count {0};
    for(Num i{0}; i<=upper; ++i) {
        const auto found = std::find(expected.begin(), expected.end(), i) != expected.end();
        ASSERT_EQ(found, are_primes.at(i));
        ASSERT_EQ(are_primes.at(i), is_prime(i));

        count += found;
        ASSERT_EQ(count, n_primes.at(i));
    }
}

TEST_F(TestAll, PrimesSqrt) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 7 * 7;
    find_primes(upper, primes, are_primes, n_primes);

    const std::vector<Num> expected {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    ASSERT_EQ(expected.size(), primes.size());
    ASSERT_TRUE(std::equal(primes.begin(), primes.end(), expected.begin()));

    Num count {0};
    for(Num i{0}; i<=upper; ++i) {
        const auto found = std::find(expected.begin(), expected.end(), i) != expected.end();
        ASSERT_EQ(found, are_primes.at(i));
        ASSERT_EQ(are_primes.at(i), is_prime(i));

        count += found;
        ASSERT_EQ(count, n_primes.at(i));
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
