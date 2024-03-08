// アルゴリズムは蟻本の65ページ目を参照

#include <algorithm>
#include <numeric>
#include <vector>
#include <gtest/gtest.h>

using Num = long long int;

size_t lis(const std::vector<Num>& xs) {
    std::vector<Num> lis;
    for (const auto& a : xs) {
        auto it = std::lower_bound(lis.begin(), lis.end(), a);
        if (it == lis.end()) {
            lis.push_back(a);
        } else {
            *it = a;
        }
    }

    return lis.size();
}

class TestLis : public ::testing::Test {};

TEST_F(TestLis, MonoIncreaing) {
    for(size_t size {1}; size<5; ++size) {
        std::vector<Num> input(size);
        std::iota(input.begin(), input.end(), 0LL);

        const auto actual = lis(input);
        EXPECT_EQ(size, actual);
    }
}

TEST_F(TestLis, MonoDecreaing) {
    for(size_t size {1}; size<5; ++size) {
        std::vector<Num> input(size);
        std::iota(input.begin(), input.end(), 0LL);
        std::ranges::reverse(input);

        const auto actual = lis(input);
        EXPECT_EQ(1, actual);
    }
}

TEST_F(TestLis, First) {
    std::vector<Num> input {1,10,5,4,9,21,22,19};
    const auto actual = lis(input);
    EXPECT_EQ(5, actual);
}

TEST_F(TestLis, Valley) {
    std::vector<Num> input {10,4,2,1,3,5,11};
    const auto actual = lis(input);
    EXPECT_EQ(4, actual);
}

TEST_F(TestLis, Connect) {
    std::vector<Num> input {2,3,4,1,5,6,7};
    const auto actual = lis(input);
    EXPECT_EQ(6, actual);
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
