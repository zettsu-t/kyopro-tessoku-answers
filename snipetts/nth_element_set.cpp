// https://codeforces.com/blog/entry/11080?locale=en
#include <algorithm>
#include <map>
#include <set>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <gtest/gtest.h>

class TestSet : public ::testing::Test {};

TEST_F(TestSet, All) {
    using namespace __gnu_pbds;
    using Element = int;
    const std::vector<Element> v {1,2,3,5,8,13,21};
    auto sorted = v;
    std::ranges::sort(sorted);

    using ordered_set = tree<
        Element,
        null_type, std::less<Element>,
        rb_tree_tag,
        tree_order_statistics_node_update>;

    ordered_set s;
    for(const auto& v : sorted) {
        s.insert(v);
    }

    for(size_t i{0}; i < sorted.size(); ++i) {
        EXPECT_EQ(sorted.at(i), *s.find_by_order(i));
    }

    const auto mx = *std::ranges::max_element(sorted);
    for(Element i{0}; i<=mx; ++i){
        const auto expected = std::ranges::lower_bound(sorted, i) - sorted.begin();
        EXPECT_EQ(expected, s.order_of_key(i));
    }
}

class TestMultiset : public ::testing::Test {};

TEST_F(TestMultiset, All) {
    using namespace __gnu_pbds;
    using Element = int;
    const std::vector<Element> v {5,6,6,6,7,7,8,9,9,9};
    auto sorted = v;
    std::ranges::sort(sorted);

    using ordered_multiset = tree<
        Element,
        null_type, std::less_equal<Element>,
        rb_tree_tag,
        tree_order_statistics_node_update>;

    ordered_multiset s;
    for(const auto& v : sorted) {
        s.insert(v);
    }

    for(size_t i{0}; i < sorted.size(); ++i) {
        EXPECT_EQ(sorted.at(i), *s.find_by_order(i));
    }

    const auto mx = *std::ranges::max_element(sorted);
    for(Element i{0}; i<=mx; ++i){
        const auto expected = std::ranges::lower_bound(sorted, i) - sorted.begin();
        EXPECT_EQ(expected, s.order_of_key(i));
    }
}

class TestMap : public ::testing::Test {};

TEST_F(TestMap, All) {
    using namespace __gnu_pbds;
    using Key = std::string;
    using Value = int;
    using Node = std::pair<Key,Value>;
    const std::vector<Node> v {
        {"efg", 40},
        {"pqr", 30},
        {"wxy", 20},
    };
    auto sorted = v;
    std::ranges::sort(sorted);

    using ordered_map = tree<
        Node,
        null_type, std::less<Node>,
        rb_tree_tag,
        tree_order_statistics_node_update>;

    ordered_map s;
    for(const auto& v : sorted) {
        s.insert(v);
    }

    for(size_t i{0}; i < sorted.size(); ++i) {
        EXPECT_EQ(sorted.at(i), *s.find_by_order(i));
    }

    EXPECT_EQ(0, s.order_of_key(Node{"abc", 100}));
    EXPECT_EQ(0, s.order_of_key(Node{"efg", 39}));
    EXPECT_EQ(0, s.order_of_key(Node{"efg", 40}));
    EXPECT_EQ(1, s.order_of_key(Node{"efg", 41}));
    EXPECT_EQ(2, s.order_of_key(Node{"wxy", 20}));
    EXPECT_EQ(3, s.order_of_key(Node{"wxy", 21}));
    EXPECT_EQ(3, s.order_of_key(Node{"xyz", 0}));
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
