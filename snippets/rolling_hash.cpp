// 0-based indexing and [L,R) ranges
#include <string>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
}

struct StringHash {
    std::string s;
    Num b {0};
    Num mod {0};
    std::vector<Num> digests;
    std::vector<Num> multipliers;

    StringHash(const std::string& arg_s, Num arg_b, Num arg_mod) :
        s(arg_s), b(arg_b), mod(arg_mod) {
        auto size = s.size();
        Num digest = 0;
        Num multiplier = 1;
        digests.push_back(digest);
        multipliers.push_back(multiplier);

        for(decltype(size) i{0}; i<size; ++i) {
            const Num c = (mod + static_cast<Num>(s.at(i))) % mod;
            digest = ((digest * b) + c) % mod;
            digests.push_back(digest);
            multiplier = (multiplier * b) % mod;
            multipliers.push_back(multiplier);
        }
    }

    Num digest(Num left, Num right) const {
        return (mod + digests.at(right) -
                (multipliers.at(right - left) * digests.at(left)) % mod)
            % mod;
    }

    Num merge(Num left_digest, Num right_digest, Num right_len) const {
        return (right_digest + (left_digest * multipliers.at(right_len)) % mod) % mod;
    }
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Example) {
    constexpr Num base = 256;
    constexpr Num prime = 257;
    const std::string s {"abcABCabc"};
    StringHash shash(s, base, prime);
    ASSERT_EQ(shash.digest(0, 3), shash.digest(6, 9));
}

TEST_F(TestAll, Sequence) {
    const std::vector<Num> primes{257, 65537};
    const Num size = 100;
    const std::string s(size, ' ');

    for(const auto& prime : primes) {
        for(Num base{254}; base<257; ++base) {
            StringHash shash(s, base, prime);

            std::vector<Num> answers;
            Num expected {0};
            for(Num len{0}; len<=size; ++len) {
                answers.push_back(expected);
                ASSERT_EQ(expected, shash.digest(0, len));
                expected = (expected * base + 32) % prime;
            }

            for(size_t left{0}; left<=size; ++left) {
                for(size_t right{left}; right<=size; ++right) {
                    ASSERT_EQ(answers.at(right - left), shash.digest(left, right));
                }
            }
        }
    }
}

TEST_F(TestAll, Digits) {
    const std::vector<Num> primes{257, 65537};
    const std::string s {"012345"};
    const Num size = static_cast<Num>(s.size());

    for(const auto& prime : primes) {
        for(Num base{256}; base<=256; ++base) {
            StringHash shash(s, base, prime);

            for(Num left{0}; left<=size; ++left) {
                Num expected {0};
                Num offset {0};
                for(Num right{left}; right<=size; ++right, ++offset) {
                    ASSERT_EQ(expected, shash.digest(left, right));
                    expected = (expected * base + right + 48) % prime;
                }
            }
        }
    }
}

TEST_F(TestAll, Compare) {
    Num mod = 1;
    mod <<= 29;
    const std::vector<Num> primes{mod - 3, mod - 33, mod - 43};

    constexpr Num base = 256;
    const std::string s {"abcdebcd"};
    const Num size = static_cast<Num>(s.size());

    for(const auto& prime : primes) {
        StringHash shash(s, base, prime);
        for(Num left1{0}; left1<size; ++left1) {
            for(Num len1{1}; len1<=(size-left1); ++len1) {
                const auto s1 = s.substr(left1, len1);

                for(Num left2{left1}; left2<size; ++left2) {
                    for(Num len2{1}; len2<=(size-left2); ++len2) {
                        const auto s2 = s.substr(left2, len2);

                        const auto digest_s1 = shash.digest(left1, left1+len1);
                        const auto digest_s2 = shash.digest(left2, left2+len2);
                        if (s1 == s2) {
                            EXPECT_EQ(digest_s1, digest_s2);
                        } else {
                            EXPECT_NE(digest_s1, digest_s2);
                        }
                    }
                }
            }
        }
    }
}

TEST_F(TestAll, Merge) {
    Num mod = 1;
    mod <<= 29;
    const std::vector<Num> primes{mod - 3, mod - 33, mod - 43};

    constexpr Num base = 256;
    const std::string s {"abcdebcd"};
    const Num size = static_cast<Num>(s.size());

    for(const auto& prime : primes) {
        StringHash shash(s, base, prime);
        for(Num left1{0}; left1<size; ++left1) {
            for(Num len1{1}; len1<=(size-left1); ++len1) {
                const auto s1 = s.substr(left1, len1);

                for(Num left2{left1}; left2<size; ++left2) {
                    for(Num len2{1}; len2<=(size-left2); ++len2) {
                        const auto s2 = s.substr(left2, len2);

                        auto ss = s1;
                        ss += s2;
                        StringHash shash_ss(ss, base, prime);
                        const auto digest_ss = shash_ss.digest(0, len1+len2);

                        const auto digest_s1 = shash.digest(left1, left1+len1);
                        const auto digest_s2 = shash.digest(left2, left2+len2);
                        const auto merged = shash.merge(digest_s1, digest_s2, len2);
                        EXPECT_EQ(digest_ss, merged);
                    }
                }
            }
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
