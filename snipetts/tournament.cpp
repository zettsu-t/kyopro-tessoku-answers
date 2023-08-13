#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <atcoder/dsu>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using Matches = std::vector<std::pair<Num, Num>>;
    using Children = std::vector<std::pair<Num, Num>>;
    using Leaves = std::vector<Num>;
    using LeafIndexes = std::vector<Num>;
}

void visit_dfs(const Children& children, Num current, Leaves& leaves) {
    const auto left = children.at(current).first;
    const auto right = children.at(current).second;
    if (left == 0) {
        leaves.push_back(current);
        return;
    }

    visit_dfs(children, left, leaves);
    visit_dfs(children, right, leaves);
    return;
}

Leaves sort_tournament(Num n, const Matches& matches) {
    atcoder::dsu tree(n+1);
    for(decltype(n) i{1}; i<=n; ++i) {
        tree.merge(i, i);
    }

    std::vector<Num> parents(n*2, 0);
    std::vector<Num> top_nodes(n*2, 0);
    for(decltype(n) i{1}; i<=n; ++i) {
        parents.at(i) = i;
        top_nodes.at(i) = i;
    }

    Children children(n*2);
    Num match_node = n;
    for(const auto& [p,q] : matches) {
        ++match_node;
        const Num node_p = top_nodes.at(tree.leader(p));
        const Num node_q = top_nodes.at(tree.leader(q));

        parents.at(node_p) = match_node;
        parents.at(node_q) = match_node;
        tree.merge(p, q);
        top_nodes.at(tree.leader(p)) = match_node;
        children.at(match_node) = std::make_pair(node_p, node_q);
    }

    Leaves leaves;
    visit_dfs(children, n*2-1, leaves);
    return leaves;
}

LeafIndexes leaf_to_index(const Leaves& leaves, Num n) {
    LeafIndexes indexes(n+1, -1);
    for(Num i{0}; i<n; ++i) {
        indexes.at(leaves.at(i)) = i;
    }
    return indexes;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Sample1) {
    const Matches matches {{4, 2}, {4, 3}, {2, 1}};
    const Leaves expected {4, 2, 3, 1};
    const Num n = static_cast<Num>(expected.size());
    const auto actual = sort_tournament(n, matches);

    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));

    const auto indexes = leaf_to_index(actual, n);
    const Leaves expected_index {-1, 3, 1, 2, 0};
    ASSERT_EQ(expected_index.size(), indexes.size());
    ASSERT_TRUE(std::equal(expected_index.begin(), expected_index.end(),
                           indexes.begin(), indexes.end()));
}

TEST_F(TestAll, Sample2) {
    const Matches matches {{2, 3}, {4, 5}, {3, 6}, {4, 1}, {2, 1}};
    const Leaves expected {2, 3, 6, 4, 5, 1};
    const Num n = static_cast<Num>(expected.size());
    const auto actual = sort_tournament(n, matches);

    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));

    const auto indexes = leaf_to_index(actual, n);
    const Leaves expected_index {-1, 5, 0, 1, 3, 4, 2};
    ASSERT_EQ(expected_index.size(), indexes.size());
    ASSERT_TRUE(std::equal(expected_index.begin(), expected_index.end(),
                           indexes.begin(), indexes.end()));
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
