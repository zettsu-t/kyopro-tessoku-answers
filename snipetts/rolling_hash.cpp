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
            Num c = (mod + static_cast<Num>(s.at(i))) % mod;
            digest = ((digest * b) + c) % mod;
            digests.push_back(digest);
            multiplier = (multiplier * b) % mod;
            multipliers.push_back(multiplier);
        }
    }

    Num digest(Num left, Num right) const {
        return (mod + digests.at(right) -
                (multipliers.at(right - left + 1) * digests.at(left - 1)) % mod)
            % mod;
    }
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    Num mod = 1;
    mod <<= 29;
    constexpr Num base = 256;
    Num prime1 = mod - 3;
    Num prime2 = mod - 33;

    const std::string s {"abcdabc"};
    const Num size = static_cast<Num>(s.size());
    StringHash hash1(s, base, prime1);
    StringHash hash2(s, base, prime2);

    for(Num left1{0}; left1<size; ++left1) {
        for(Num len1{1}; len1<=(size-left1); ++len1) {
            const auto s1 = s.substr(left1, len1);
            for(Num left2{left1}; left2<size; ++left2) {
                for(Num len2{1}; len2<=(size-left2); ++len2) {
                    const auto s2 = s.substr(left2, len2);
                    const auto d1_1 = hash1.digest(left1+1, left1+len1);
                    const auto d1_2 = hash2.digest(left1+1, left1+len1);
                    const auto d2_1 = hash1.digest(left2+1, left2+len2);
                    const auto d2_2 = hash2.digest(left2+1, left2+len2);
                    if (s1 == s2) {
                        EXPECT_EQ(d1_1, d2_1);
                        EXPECT_EQ(d1_2, d2_2);
                    } else {
                        EXPECT_NE(d1_1, d2_1);
                        EXPECT_NE(d1_2, d2_2);
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
