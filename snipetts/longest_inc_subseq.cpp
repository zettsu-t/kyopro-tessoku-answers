// アルゴリズムは蟻本の65ページ目を参照

#include <algorithm>
#include <limits>
#include <map>
#include <numeric>
#include <vector>
#include <utility>
#include <atcoder/segtree>
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

Num apply_max(Num a, Num b) {
    return std::max(a,b);
}

Num unit_max() {
    return std::numeric_limits<Num>::min();
}

using SegTree = atcoder::segtree<Num, apply_max, unit_max>;

// https://atcoder.jp/contests/abc354/editorial/10027
SegTree lis_tree(const std::vector<Num>& xs) {
    const int size = static_cast<int>(xs.size());
    SegTree dp(size);

    for(int i{0}; i<size; ++i) {
        const auto& x = xs.at(i);
        dp.set(x, std::max(0LL, dp.prod(0, x)) + 1);
    }

    return dp;
}

std::pair<std::vector<Num>, std::map<Num,Num>> compress(const std::vector<Num>& v) {
    std::vector<Num> sorted = v;
    std::ranges::sort(sorted);
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    std::vector<Num> vec = v;
    std::map<Num,Num> table;
    for(auto&& x : vec) {
        Num original = x;
        x = std::ranges::lower_bound(sorted, x) - sorted.begin();
        table[original] = x;
    }

    return std::make_pair(vec, table);
}

class TestCompress : public ::testing::Test {};

TEST_F(TestCompress, All) {
    std::vector<Num> input {300,20,1,20,40};
    const std::vector<Num> expected_vec {3,1,0,1,2};
    const std::map<Num,Num> expected_table {{1,0}, {20,1}, {40,2}, {300,3}};

    const auto [compressed, table] = compress(input);
    EXPECT_EQ(expected_vec, compressed);
    for(const auto& [k,v] : expected_table) {
        ASSERT_TRUE(expected_table.contains(k));
        EXPECT_EQ(expected_table.at(k), table.at(k));
    }
}

class TestLis : public ::testing::Test {};

TEST_F(TestLis, MonoIncreaing) {
    for(size_t size {1}; size<5; ++size) {
        std::vector<Num> input(size);
        std::iota(input.begin(), input.end(), 0LL);

        const auto actual = lis(input);
        EXPECT_EQ(size, actual);

        const auto [compressed, table] = compress(input);
        auto tree = lis_tree(compressed);
        for(size_t i{1}; i<=size; ++i) {
            const Num expected = static_cast<Num>(i);
            EXPECT_EQ(expected, tree.get(i-1));
        }
    }
}

TEST_F(TestLis, MonoDecreaing) {
    for(size_t size {1}; size<5; ++size) {
        std::vector<Num> input(size);
        std::iota(input.begin(), input.end(), 0LL);
        std::ranges::reverse(input);

        const auto actual = lis(input);
        EXPECT_EQ(1, actual);

        const auto [compressed, table] = compress(input);
        auto tree = lis_tree(compressed);
        for(size_t i{1}; i<=size; ++i) {
            EXPECT_EQ(1, tree.get(i-1));
        }
    }
}

TEST_F(TestLis, First) {
    std::vector<Num> input {1,10,5,4,9,21,22,19};
    const auto actual = lis(input);
    EXPECT_EQ(5, actual);

    auto [compressed, table] = compress(input);
    const std::vector<Num> expected {1,2,2,2,3,4,5,4};
    auto tree = lis_tree(compressed);
    for(size_t i{0}; i<input.size(); ++i) {
        // 出現位置ではなく座標圧縮済の番号に対して、LIS長が書かれている
        const auto serial = table[input.at(i)];
        EXPECT_EQ(expected.at(i), tree.get(serial));
    }
}

TEST_F(TestLis, Valley) {
    std::vector<Num> input {10,4,2,1,3,5,10};
    const auto actual = lis(input);
    EXPECT_EQ(4, actual);

    auto [compressed, table] = compress(input);
    const std::vector<Num> expected {4,1,1,1,2,3,4};
    auto tree = lis_tree(compressed);
    for(size_t i{0}; i<input.size(); ++i) {
        // 出現位置ではなく座標圧縮済の番号に対して、LIS長が書かれている
        const auto serial = table[input.at(i)];
        EXPECT_EQ(expected.at(i), tree.get(serial));
    }
}

TEST_F(TestLis, Connect) {
    std::vector<Num> input {2,3,4,1,5,6,7};
    const auto actual = lis(input);
    EXPECT_EQ(6, actual);

    auto [compressed, table] = compress(input);
    const std::vector<Num> expected {1,2,3,1,4,5,6};
    auto tree = lis_tree(compressed);
    for(size_t i{0}; i<input.size(); ++i) {
        // 出現位置ではなく座標圧縮済の番号に対して、LIS長が書かれている
        const auto serial = table[input.at(i)];
        EXPECT_EQ(expected.at(i), tree.get(serial));
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
