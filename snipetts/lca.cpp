#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <atcoder/segtree>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    constexpr Num INF = std::numeric_limits<Num>::max();

    struct Edge {
        Num from {0};
        Num to {0};
        Num weight {0};
    };

    struct Node {
        Num id {0};
        Num sequence {0};
        Num depth {0};
    };

    struct LcaNode {
        Num id {0};
        Num sequence {0};
        Num depth {0};
        bool operator<(const LcaNode& rhs) const {
            return std::tie(depth, id, sequence) < std::tie(rhs.depth, rhs.id, rhs.sequence);
        }
    };

    Num apply_lca_edge(Num a, Num b) {
        return a + b;
    }

    Num unit_lca_edge(void) {
        return 0;
    }

    LcaNode apply_lca_node(LcaNode a, LcaNode b) {
        return (a < b) ? a : b;
    }

    LcaNode unit_lca_node(void) {
        LcaNode node {0, 0, INF};
        return node;
    }

    using Edges = std::unordered_map<Num, std::vector<Edge>>;
    using Path = std::vector<Num>;
    using SegmentTreeLcaNode = atcoder::segtree<LcaNode, apply_lca_node, unit_lca_node>;
    using SegmentTreeLcaEdge = atcoder::segtree<Num, apply_lca_edge, unit_lca_edge>;
    using NodeToLcaIndex = std::vector<std::optional<Num>>;
    using EdgeIndexes = std::unordered_map<Num, std::unordered_map<Num, Num>>;
    using EdgeDirection = std::unordered_map<Num, std::unordered_map<Num, Num>>;

    struct LcaSet {
        const Edges& edges;
        Num min_node_id {0};
        Num max_node_id {0};
        NodeToLcaIndex node_to_lca_index;
        std::unique_ptr<SegmentTreeLcaNode> segtree_lca_nodes;
        std::unique_ptr<SegmentTreeLcaEdge> segtree_lca_edges;
        EdgeIndexes edge_indexes;
        EdgeDirection edge_direction;
        Num sequence_node {0};
        Num sequence_edge {0};

        LcaSet(const Edges& in_edges, Num n) : edges(in_edges) {
            std::set<Num> node_ids;
            for(const auto& [k, v] : in_edges) {
                node_ids.insert(k);
                for(const auto& edge : v) {
                    node_ids.insert(edge.from);
                    node_ids.insert(edge.to);
                }
            }
            min_node_id = 1;
            max_node_id = n;

            const auto size = n * 2 + 2;
            node_to_lca_index.resize(n + 2);
            segtree_lca_nodes = std::make_unique<SegmentTreeLcaNode>(size);
            segtree_lca_edges = std::make_unique<SegmentTreeLcaEdge>(size);
        }

        void visit_dfs(Num current, Num depth);
        Num find_lca(Num root, Num depth);
        Num find_lca(void);
        Num calc_distance(Num x, Num y);
    };
}

void eular_tour(Num current, Num prev, const Edges& edges, Path& path) {
    path.push_back(current);

    auto it = edges.find(current);
    if (it == edges.end()) {
        return;
    }

    for(const auto& edge : it->second) {
        const auto& next = edge.to;
        if (next != prev) {
            eular_tour(next, current, edges, path);
            path.push_back(current);
        }
    }
}

void LcaSet::visit_dfs(Num current, Num depth) {
    if (!node_to_lca_index.at(current).has_value()) {
        node_to_lca_index.at(current) = sequence_node;
    }

    if (edges.find(current) == edges.end()) {
        return;
    }

    auto it = edges.find(current);
    if (it == edges.end()) {
        return;
    }

    for(auto& edge : it->second) {
        const auto next = edge.to;
        if (node_to_lca_index.at(next).has_value()) {
            continue;
        }

        const auto next_depth = depth + 1;

        ++sequence_node;
        const LcaNode next_node{next, sequence_node, next_depth};
        segtree_lca_nodes->set(sequence_node, next_node);

        ++sequence_edge;
        segtree_lca_edges->set(sequence_edge, edge.weight);
        visit_dfs(next, next_depth);

        ++sequence_node;
        const LcaNode current_node{current, sequence_node, depth};
        segtree_lca_nodes->set(sequence_node, current_node);
        ++sequence_edge;
        segtree_lca_edges->set(sequence_edge, -edge.weight);
    }

    return;
}

Num LcaSet::find_lca(Num root, Num depth) {
    ++sequence_node;
    const LcaNode root_node{root, sequence_node, 1};
    segtree_lca_nodes->set(sequence_node, root_node);

    visit_dfs(root, depth);

    const Num lca_size = sequence_node;
    Num prev = segtree_lca_nodes->prod(1, 2).id;
    typename EdgeIndexes::mapped_type zero_index;
    typename EdgeDirection::mapped_type zero_direction;

    for(Num i{2}; i<=lca_size; ++i) {
        Num current = segtree_lca_nodes->prod(i, i+1).id;
        if (edge_indexes.find(prev) == edge_indexes.end()) {
            edge_indexes[prev] = zero_index;
        }
        (edge_indexes[prev])[current] = i - 1;

        if (edge_direction.find(prev) == edge_direction.end()) {
            edge_direction[prev] = zero_direction;
        }

        if (edge_direction.find(current) == edge_direction.end()) {
            edge_direction[current] = zero_direction;
        }

        if (edge_direction[prev].find(current) == edge_direction[prev].end()) {
            (edge_direction[prev])[current] = 1;
            (edge_direction[current])[prev] = -1;
        }

        prev = current;
    }

    return lca_size;
}

Num LcaSet::find_lca(void) {
    return find_lca(min_node_id, 1);
}

Num LcaSet::calc_distance(Num x, Num y) {
    auto u = node_to_lca_index.at(x).value();
    auto v = node_to_lca_index.at(y).value();
    const auto lca = (u <= v) ? segtree_lca_nodes->prod(u, v+1) :
        segtree_lca_nodes->prod(v, u+1);

    const auto lca_index = node_to_lca_index.at(lca.id).value();
    Num dist{0};
    if (x == y) {
        dist = 0;
    } else if (lca.id == x) {
        dist = segtree_lca_edges->prod(lca_index, v);
    } else if (lca.id == y) {
        dist = segtree_lca_edges->prod(lca_index, u);
    } else {
        dist = segtree_lca_edges->prod(lca_index, v);
        dist += segtree_lca_edges->prod(lca_index, u);
    }
    return dist;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    const std::vector<Edge> test_case {
        {1,2,300}, {2,4,30}, {2,5,40}, {5,8,5}, {5,9,6},
        {1,3,400}, {3,6,50}, {3,7,60}
    };
    const auto n_edges = test_case.size();
    const auto n_nodes = n_edges + 1;
    const auto n_lca_nodes = n_edges * 2 + 1;

    const std::vector<LcaNode> expected_lcas {
        {1,1,1}, {2,2,2}, {4,3,3}, {2,4,2}, {5,5,3}, {8,6,4}, {5,7,3}, {9,8,4}, {5,9,3}, {2,10,2},
        {1,11,1}, {3,12,2}, {6,13,3}, {3,14,2}, {7,15,3}, {3,16,2}, {1,17,1}
    };

    struct VisitedEdge {
        Num sequence {0};
        Num from {0};
        Num to {0};
        Num direction {0};
    };

    const std::vector<VisitedEdge> expected_edges {
        {1,1,2,1}, {2,2,4,1}, {3,4,2,-1}, {4,2,5,1},
        {5,5,8,1}, {6,8,5,-1}, {7,5,9,1}, {8,9,5,-1}, {9,5,2,-1}, {10,2,1,-1},
        {11,1,3,1}, {12,3,6,1}, {13,6,3,-1}, {14,3,7,1}, {15,7,3,-1}, {16,3,1,-1}
    };

    Edges edges;
    for(const auto& forward : test_case) {
        edges[forward.from].push_back(forward);
        auto rev = forward;
        std::swap(rev.from, rev.to);
        edges[rev.from].push_back(rev);
    }

    LcaSet lca_set(edges, static_cast<Num>(n_nodes));
    lca_set.find_lca();
    ASSERT_EQ(1, lca_set.min_node_id);
    ASSERT_EQ(9, lca_set.max_node_id);
    ASSERT_EQ(n_lca_nodes, lca_set.sequence_node);
    ASSERT_EQ(n_edges * 2, lca_set.sequence_edge);
    ASSERT_EQ(n_lca_nodes, expected_lcas.size());
    ASSERT_LE(n_nodes + 1, lca_set.node_to_lca_index.size());

    for(size_t i{1}; i<=n_lca_nodes; ++i) {
        const auto actual = lca_set.segtree_lca_nodes->prod(i,i+1);
        const auto expected = expected_lcas.at(i-1);
        ASSERT_EQ(i, actual.sequence);
        ASSERT_EQ(expected.id, actual.id);
        ASSERT_EQ(expected.sequence, actual.sequence);
        ASSERT_EQ(expected.depth, actual.depth);
        ASSERT_EQ(i, actual.sequence);
    }

    for(size_t i{1}; i<=n_nodes; ++i) {
        const auto& e = lca_set.node_to_lca_index.at(i);
        ASSERT_TRUE(e.has_value());

        bool found {false};
        for(const auto& lca : expected_lcas) {
            if (lca.id == static_cast<Num>(i)) {
                EXPECT_EQ(lca.sequence, e.value());
                found = true;
                break;
            }
        }
        ASSERT_TRUE(found);
    }

    ASSERT_EQ(n_edges * 2, expected_edges.size());
    for(const auto& [from, m] : lca_set.edge_indexes) {
        for(const auto& [to, sequence] : m) {
            bool found {false};
            for(const auto& expected : expected_edges) {
                if ((expected.from == from) && (expected.to == to)) {
                    ASSERT_EQ(expected.sequence, sequence);
                    found = true;
                    break;
                }
            }
            ASSERT_TRUE(found);
        }
    }

    for(const auto& [from, m] : lca_set.edge_direction) {
        for(const auto& [to, direction] : m) {
            Num sequence {1};
            bool found {false};
            for(const auto& expected : expected_edges) {
                if ((expected.from == from) && (expected.to == to)) {
                    ASSERT_EQ(expected.direction, direction);
                    found = true;
                    break;
                }
                ++sequence;
            }
            ASSERT_TRUE(found);
        }
    }

    struct Query {
        Num from {0};
        Num to {0};
        Num lca {0};
    };

    const std::vector<Query> queries {
        {1, 1, 1}, {2, 2, 2}, {8, 8, 8},
        {1, 2, 1}, {2, 4, 2}, {3, 7, 3},
        {2, 3, 1}, {4, 7, 1}, {4, 5, 2}, {6, 7, 3}, {4, 9, 2}
    };

    for(const auto& q : queries) {
        Num left {-1};
        Num alt_right {-1};
        Num right {-1};
        for(const auto& e : expected_lcas) {
            if (e.id == q.from) {
                if (left < 0) {
                    left = e.sequence;
                }
            }
            if (e.id == q.to) {
                if (alt_right < 0) {
                    alt_right = e.sequence;
                } else {
                    right = e.sequence;
                }
            }
        }

        right = std::max(alt_right, right);
        if (left > right) {
            std::swap(left, right);
        }
        const auto actual = lca_set.segtree_lca_nodes->prod(left, right+1).id;
        ASSERT_EQ(q.lca, actual);
    }

    EXPECT_EQ(0,   lca_set.calc_distance(1, 1));
    EXPECT_EQ(300, lca_set.calc_distance(1, 2));
    EXPECT_EQ(700, lca_set.calc_distance(2, 3));
    EXPECT_EQ(40,  lca_set.calc_distance(2, 5));
    EXPECT_EQ(46,  lca_set.calc_distance(2, 9));
}

TEST_F(TestAll, Root) {
    const std::vector<Edge> test_case {{3,2,100}, {2,1,30}, {2,4,40}, {3,5,200}};
    const auto n_edges = test_case.size();
    const auto n_nodes = n_edges + 1;
    const auto n_lca_nodes = n_edges * 2 + 1;

    Edges edges;
    for(const auto& forward : test_case) {
        edges[forward.from].push_back(forward);
        auto rev = forward;
        std::swap(rev.from, rev.to);
        edges[rev.from].push_back(rev);
    }

    LcaSet lca_set(edges, static_cast<Num>(n_nodes));
    lca_set.find_lca(3, 1);

    const std::vector<LcaNode> expected_lcas {
        {3,1,1}, {2,2,2}, {1,3,3}, {2,4,2}, {4,5,3}, {2,6,2}, {3,7,1}, {5,8,2}, {3,9,1}};

    ASSERT_EQ(n_lca_nodes, expected_lcas.size());
    for(size_t i{1}; i<=n_lca_nodes; ++i) {
        const auto actual = lca_set.segtree_lca_nodes->prod(i,i+1);
        const auto expected = expected_lcas.at(i-1);
        ASSERT_EQ(i, actual.sequence);
        ASSERT_EQ(expected.id, actual.id);
        ASSERT_EQ(expected.sequence, actual.sequence);
        ASSERT_EQ(expected.depth, actual.depth);
        ASSERT_EQ(i, actual.sequence);
    }

    EXPECT_EQ(0,   lca_set.calc_distance(1, 1));
    EXPECT_EQ(30,  lca_set.calc_distance(1, 2));
    EXPECT_EQ(130, lca_set.calc_distance(1, 3));
    EXPECT_EQ(330, lca_set.calc_distance(1, 5));
    EXPECT_EQ(70,  lca_set.calc_distance(1, 4));

    EXPECT_EQ(0,   lca_set.calc_distance(2, 2));
    EXPECT_EQ(100, lca_set.calc_distance(2, 3));
    EXPECT_EQ(40,  lca_set.calc_distance(2, 4));
    EXPECT_EQ(300, lca_set.calc_distance(2, 5));

    EXPECT_EQ(0,   lca_set.calc_distance(3, 3));
    EXPECT_EQ(140, lca_set.calc_distance(3, 4));
    EXPECT_EQ(200, lca_set.calc_distance(3, 5));
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
