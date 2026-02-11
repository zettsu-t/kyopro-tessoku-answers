#include <algorithm>
#include <limits>
#include <map>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
}

std::map<Num,Num> dense_rank(std::vector<Num>& vs) {
    std::sort(vs.begin(), vs.end());
    std::map<Num,Num> tbl;
    Num prev = std::numeric_limits<Num>::min();
    Num serial {0};

    for(const auto& v : vs) {
        if (v != prev) {
            tbl[v] = serial;
            ++serial;
        }
        prev = v;
    }

    return tbl;
}


class TestAll : public ::testing::Test {};

TEST_F(TestAll, Empty) {
    std::vector<Num> input;
    const auto actual = dense_rank(input);
    EXPECT_TRUE(actual.empty());
}

TEST_F(TestAll, Same) {
    std::vector<Num> input {3,3,3,3,3};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{3,0}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestAll, Each) {
    std::vector<Num> input {2,4,8};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{2,0}, {4,1}, {8,2}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestAll, Shuffle) {
    std::vector<Num> input {4,1,2,16,8};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{1,0}, {2,1}, {4,2}, {8,3}, {16,4}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestAll, Groups1) {
    std::vector<Num> input {40,40,20,20,20,30};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{20,0}, {30,1}, {40,2}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestAll, Groups2) {
    std::vector<Num> input {0,30,30,30,10,20,20,40,40};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{0,0}, {20,1}, {30,2}, {40,3}, {50,4}};
    ASSERT_EQ(expected, actual);
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
