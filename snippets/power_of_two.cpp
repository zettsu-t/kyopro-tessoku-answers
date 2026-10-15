#include <vector>
#include <atcoder/modint.hpp>
#include <gtest/gtest.h>

namespace {
    using Num = int;
    using ModInt = atcoder::modint1000000007;
}

std::vector<ModInt> two_to_the_power_of_two(void) {
    std::vector<ModInt> powered(32);
    powered.at(0) = 2;
    for(Num i{1}; i<32; ++i) {
        powered.at(i) = powered.at(i-1) * powered.at(i-1);
    }

    return powered;
}

ModInt two_to_the_n_power(Num n) {
    const auto powered = two_to_the_power_of_two();

    ModInt answer = 1;
    Num index = 0;
    Num r = n;
    while(r > 0) {
        if ((r & 1) > 0) {
            answer *= powered.at(index);
        }
        r >>= 1;
        ++index;
    }

    return answer;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, TwoToThePowerOfTwo) {
    const auto actual = two_to_the_power_of_two();
    const auto expected = {
               2,         4,         16,       256,
            65536, 294967268, 582344008, 279632277,
        792845266, 418385479, 812734592, 409643880,
        246797651, 112754241, 774491455, 669157962,
        973586826, 125655169, 385894014, 998632880,
         36221046, 164150368, 125918023, 405241093,
        306292255, 816280417, 513497891, 215687307,
         75265151, 915398907,  71108578, 829787081
    };

    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, TwoToTheNpower) {
    std::vector<Num> input {1,2,3,4,5,6,7,8,9,15,16,17,31,32,33,63,64,65,127,128};
    std::vector<ModInt> actual;
    for(const auto& i : input) {
        actual.push_back(two_to_the_n_power(i));
    }

    const auto expected = {
        2, 4, 8, 16, 32, 64, 128, 256, 512,
        32768, 65536, 131072, 147483634,
        294967268, 589934536, 291172004, 582344008,
        164688009 ,639816142, 279632277
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
