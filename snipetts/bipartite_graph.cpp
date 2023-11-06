#include <bits/stdc++.h>
#include <atcoder/dsu>
#include <atcoder/modint>
#include <gtest/gtest.h>

namespace {
using ModInt [[maybe_unused]] = atcoder::modint998244353;
using Num [[maybe_unused]] = long long int;
using Vec [[maybe_unused]] = std::vector<Num>;
using Set [[maybe_unused]] = std::set<Num>;
using Mset [[maybe_unused]] = std::multiset<Num>;

template<typename T>
using Q [[maybe_unused]] = std::queue<T>;

template<typename T>
using PQ [[maybe_unused]] = std::priority_queue<T, std::vector<T>, std::greater<T>>;

using Edges [[maybe_unused]] = std::vector<std::vector<Num>>;
using EdgePair [[maybe_unused]] = std::vector<std::pair<Num,Num>>;
}

Edges to_matrix(const EdgePair& in_edges) {
    Num n {0};
    for(const auto& [u, v] : in_edges) {
        n = std::max(n, u);
        n = std::max(n, v);
    }

    Edges edges(n + 1);
    for(const auto& [u, v] : in_edges) {
        edges.at(u).push_back(v);
        if (u != v) {
            edges.at(v).push_back(u);
        }
    }

    return edges;
}

bool check_bipartite_graph(const EdgePair& edges, Num n) {
    atcoder::dsu tree(n*2 + 1);

    for(const auto& [u, v] : edges) {
        if (u == v) {
            return false;
        }

        const auto u0 = u * 2;
        const auto u1 = u0 | 1;
        const auto v0 = v * 2;
        const auto v1 = v0 | 1;
        tree.merge(u0, v1);
        tree.merge(u1, v0);
    }

    for(Num i{0}; i<n; ++i) {
        const auto i0 = i * 2;
        if (tree.same(i0, i0 | 1)) {
            return false;
        }
    }

    return true;
}

bool check_bipartite_graph(const Edges& edges) {
    const Num n = static_cast<Num>(edges.size());
    atcoder::dsu tree(n*2 + 1);

    for(Num u{0}; u<n; ++u) {
        for(const auto& v : edges.at(u)) {
            if (u == v) {
                return false;
            }

            const auto u0 = u * 2;
            const auto u1 = u0 | 1;
            const auto v0 = v * 2;
            const auto v1 = v0 | 1;
            tree.merge(u0, v1);
            tree.merge(u1, v0);
        }
    }

    for(Num i{0}; i<n; ++i) {
        const auto i0 = i * 2;
        if (tree.same(i0, i0 | 1)) {
            return false;
        }
    }

    return true;
}

class TestToMatrix : public ::testing::Test {};

TEST_F(TestToMatrix, All) {
    const EdgePair in_edges {{0, 2}, {2, 2}, {2, 3}};
    const auto actual = to_matrix(in_edges);
    const Edges expected {{2}, {}, {0,2,3}, {2}};

    ASSERT_EQ(expected.size(), actual.size());
    for(Num i{0}; i<4; ++i) {
        ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));
    }
}

class TestEdges : public ::testing::Test {};

TEST_F(TestEdges, Empty) {
    const EdgePair edges;
    ASSERT_TRUE(check_bipartite_graph(edges, 0));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges)));
}

TEST_F(TestEdges, One) {
    const EdgePair edges;
    ASSERT_TRUE(check_bipartite_graph(edges, 1));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges)));
}

TEST_F(TestEdges, Self) {
    const EdgePair edges {{0, 0}};
    ASSERT_FALSE(check_bipartite_graph(edges, 1));
    ASSERT_FALSE(check_bipartite_graph(to_matrix(edges)));
}

TEST_F(TestEdges, Two) {
    const EdgePair edges {{0, 1}};
    ASSERT_TRUE(check_bipartite_graph(edges, 2));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges)));
}

TEST_F(TestEdges, Three) {
    const EdgePair edges1 {{0, 1}, {1, 2}};
    ASSERT_TRUE(check_bipartite_graph(edges1, 3));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges1)));

    const EdgePair edges2 {{0, 1}, {1, 2}, {2, 0}};
    ASSERT_FALSE(check_bipartite_graph(edges2, 3));
    ASSERT_FALSE(check_bipartite_graph(to_matrix(edges2)));
}

TEST_F(TestEdges, Four) {
    const EdgePair edges1 {{0, 1}, {0, 3}, {2, 1}, {2, 3}};
    ASSERT_TRUE(check_bipartite_graph(edges1, 4));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges1)));

    const EdgePair edges2 {{0, 1}, {0, 2}, {0, 3}, {1, 2}};
    ASSERT_FALSE(check_bipartite_graph(edges2, 4));
    ASSERT_FALSE(check_bipartite_graph(to_matrix(edges2)));

    const EdgePair edges3 {{0, 1}, {2, 3}};
    ASSERT_TRUE(check_bipartite_graph(edges3, 4));
    ASSERT_TRUE(check_bipartite_graph(to_matrix(edges3)));

    const EdgePair edges4 {{0, 1}, {2, 3}, {3, 3}};
    ASSERT_FALSE(check_bipartite_graph(edges4, 4));
    ASSERT_FALSE(check_bipartite_graph(to_matrix(edges4)));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
