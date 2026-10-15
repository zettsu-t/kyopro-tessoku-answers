// SPDX-FileCopyrightText: 2026 Zettsu Tatsuya
// SPDX-License-Identifier: MIT
// See also:
// https://github.com/zettsu-t/kyopro-tessoku-answers/blob/main/results/ABC-lessons-learned.md
// https://github.com/zettsu-t/kyopro-tessoku-answers/blob/main/snippets/find_rational.cpp
// https://github.com/zettsu-t/kyopro-tessoku-answers/blob/main/snippets/lca.cpp
// https://github.com/zettsu-t/kyopro-tessoku-answers/tree/main/snippets/

// -------------------------------------------------------------------
// Headers
#include <bits/stdc++.h>
#include <atcoder/all>
#include <boost/multi_array.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <gtest/gtest.h>

// -------------------------------------------------------------------
// Type aliases and constants
namespace {
    using ModInt [[maybe_unused]] = atcoder::modint998244353;
    using ModInt1000000007 = atcoder::modint1000000007;
    using BigInt = boost::multiprecision::cpp_int;
    using Rational = boost::rational<BigInt>;
    using BigIntVec = std::vector<BigInt>;
    using BigFloat [[maybe_unused]] = std::float128_t;

    using Num [[maybe_unused]] = long long int;
    using Numeric = double;
    using Vec [[maybe_unused]] = std::vector<Num>;
    using Set [[maybe_unused]] = std::set<Num>;
    using Mset [[maybe_unused]] = std::multiset<Num>;
    using Edges [[maybe_unused]] = std::vector<std::vector<Num>>;
    using EdgePair [[maybe_unused]] = std::vector<std::pair<Num,Num>>;
    using Path [[maybe_unused]] = std::vector<Num>;
    using DsuNode [[maybe_unused]] = int;
    using SegtreeNode [[maybe_unused]] = int;
    constexpr Num Inf = std::numeric_limits<Num>::max() / 2;

    template<typename T>
    using Q [[maybe_unused]] = std::queue<T>;

    template<typename T>
    using PQ [[maybe_unused]] = std::priority_queue<T, std::vector<T>, std::greater<T>>;

    const std::vector<std::pair<Num, Num>> dyxs {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    };
    std::map<char, std::pair<Num, Num>> directions {
        {'U', {-1, 0}}, {'R', {0, 1}}, {'D', {1, 0}}, {'L', {0, -1}}
    };
    const std::vector<std::pair<Num, Num>> signs {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
}

// -------------------------------------------------------------------
// Policy Based Data Structures
using namespace __gnu_pbds;
using TreeElement = std::pair<Num,Num>;
using ordered_multiset = tree<
    TreeElement,
    null_type, std::less_equal<TreeElement>,
    rb_tree_tag,
    tree_order_statistics_node_update>;

// -------------------------------------------------------------------
// 2D-array from 2d_array.cpp
template <typename T, size_t H, size_t W>
void fill_2darray(std::array<std::array<T, W>, H>& arr, const T& v) {
    static_assert(std::is_same_v<size_t, typename std::array<std::array<T, W>, H>::size_type>);
    static_assert(std::is_same_v<size_t, typename std::array<T, W>::size_type>);
    for(size_t y{0}; y<H; ++y) {
        for(size_t x{0}; x<W; ++x) {
            arr.at(y).at(x) = v;
        }
    }
}

class Test2dArray : public ::testing::Test {};

TEST_F(Test2dArray, Array) {
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    std::array<std::array<int, width>, height> arr;

    for(int v{0}; v<10; ++v) {
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                arr.at(y).at(x) = v;
            }
        }

        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr.at(y).at(x));
            }
        }
    }
}

TEST_F(Test2dArray, Array2) {
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    std::array<std::array<int, width>, height> arr;

    for(int v{0}; v<10; ++v) {
        fill_2darray(arr, v);
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr.at(y).at(x));
            }
        }
    }
}

TEST_F(Test2dArray, Boost) {
    using Matrix = boost::multi_array<int, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    MatrixShape shape {{height, width}};
    Matrix arr(shape);

    for(int v{0}; v<10; ++v) {
        std::fill_n(arr.data(), arr.num_elements(), v);
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr[y][x]);
            }
        }
    }
}

// -------------------------------------------------------------------
// Angular sorting from angular_sorting.cpp
// input (X,Y,index)[]
// out {(rank,X,Y):indexes}
std::map<std::tuple<Num,Num,Num>, std::vector<Num>> sort_angular(
    const std::vector<std::tuple<Num,Num,Num>>& points
) {
    using Coord = std::pair<Num,Num>;
    using CoordSet = std::set<
        Coord,
        decltype([](const Coord& lhs, const Coord& rhs) {
            return ((lhs.first * rhs.second - lhs.second * rhs.first) > 0);
        })>;
    std::map<Coord, std::vector<Num>> table;
    std::array<CoordSet, 2> coord_set;

    for(const auto& [x,y,index] : points) {
        const auto g = std::abs(std::gcd(std::abs(x), std::abs(y)));
        const auto dx = x / g;
        const auto dy = y / g;
        table[std::make_pair(dx, dy)].push_back(index);
        const Num set_index = ((y < 0) || ((y == 0) && (x < 0)));
        coord_set[set_index].insert(std::make_pair(dx, dy));
    }

    std::map<std::tuple<Num,Num,Num>, std::vector<Num>> result;
    Num serial {0};
    for(auto&& coords : coord_set) {
        for(const auto& coord : coords) {
            const auto [x,y] = coord;
            const std::tuple<Num,Num,Num> p (serial, x, y);
            result[p] = table[coord];
            ++serial;
        }
    }

    return result;
}

class TestAngularSorting : public ::testing::Test {};

TEST_F(TestAngularSorting, DijkstraAlgo) {
    // 10       9
    //    4 5 6
    //    3-o-7-8--
    //    2 1 0
    // 11       12 13

    const std::vector<std::tuple<Num,Num,Num>> points {
        { 1, -1, 0},
        { 0, -1, 1},
        {-1, -1, 2},
        {-1,  0, 3},
        {-1,  1, 4},
        { 0,  1, 5},
        { 1,  1, 6},
        { 1,  0, 7},
        { 2,  0, 8},
        { 2,  2, 9},
        {-2,  2, 10},
        {-2, -2, 11},
        { 2, -2, 12},
        { 6, -4, 13},
    };

    const std::vector<std::tuple<Num,Num,std::vector<Num>>> expected {
        { 1,  0, {7,8}},
        { 1,  1, {6,9}},
        { 0,  1, {5}},
        {-1,  1, {4,10}},
        {-1,  0, {3}},
        {-1, -1, {2,11}},
        { 0, -1, {1}},
        { 1, -1, {0,12}},
        { 3, -2, {13}}
    };

    const auto actual = sort_angular(points);
    Num index {0};
    for(const auto& [point, indexes] : actual) {
        const auto& [serial, x, y] = point;
        ASSERT_EQ(serial, index);
        const auto& [expected_x, expected_y, expected_indexes] = expected.at(index);
        ASSERT_EQ(expected_x, x);
        ASSERT_EQ(expected_y, y);

        const auto size = indexes.size();
        ASSERT_EQ(size, expected_indexes.size());
        for(size_t i{0}; i<size; ++i) {
            ASSERT_EQ(expected_indexes.at(i), indexes.at(i));
        }

        ++index;
    }
}

// -------------------------------------------------------------------
// Distance between vertices with BFS as ABC 151-D
// https://atcoder.jp/contests/abc151/submissions/40955856

// -------------------------------------------------------------------
// Bipartite graph from bipartite_graph.cpp
Edges to_adjacent_matrix(const EdgePair& in_edges) {
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
    atcoder::dsu tree(static_cast<DsuNode>(n*2 + 1));

    for(const auto& [u, v] : edges) {
        if (u == v) {
            return false;
        }

        const auto u0 = u * 2;
        const auto u1 = u0 | 1;
        const auto v0 = v * 2;
        const auto v1 = v0 | 1;
        tree.merge(static_cast<DsuNode>(u0), static_cast<DsuNode>(v1));
        tree.merge(static_cast<DsuNode>(u1), static_cast<DsuNode>(v0));
    }

    for(Num i{0}; i<n; ++i) {
        const auto i0 = i * 2;
        if (tree.same(static_cast<DsuNode>(i0), static_cast<DsuNode>(i0 | 1))) {
            return false;
        }
    }

    return true;
}

bool check_bipartite_graph(const Edges& edges) {
    const Num n = static_cast<Num>(edges.size());
    atcoder::dsu tree(static_cast<DsuNode>(n*2 + 1));

    for(Num u{0}; u<n; ++u) {
        for(const auto& v : edges.at(u)) {
            if (u == v) {
                return false;
            }

            const auto u0 = u * 2;
            const auto u1 = u0 | 1;
            const auto v0 = v * 2;
            const auto v1 = v0 | 1;
            tree.merge(static_cast<DsuNode>(u0), static_cast<DsuNode>(v1));
            tree.merge(static_cast<DsuNode>(u1), static_cast<DsuNode>(v0));
        }
    }

    for(Num i{0}; i<n; ++i) {
        const auto i0 = i * 2;
        if (tree.same(static_cast<DsuNode>(i0), static_cast<DsuNode>(i0 | 1))) {
            return false;
        }
    }

    return true;
}

class TestToAdjacentMatrix : public ::testing::Test {};

TEST_F(TestToAdjacentMatrix, All) {
    const EdgePair in_edges {{0, 2}, {2, 2}, {2, 3}};
    const auto actual = to_adjacent_matrix(in_edges);
    const Edges expected {{2}, {}, {0,2,3}, {2}};

    ASSERT_EQ(expected.size(), actual.size());
    for(Num i{0}; i<4; ++i) {
        ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));
    }
}

class TestBipartiteGraph : public ::testing::Test {};

TEST_F(TestBipartiteGraph, Empty) {
    const EdgePair edges;
    ASSERT_TRUE(check_bipartite_graph(edges, 0));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges)));
}

TEST_F(TestBipartiteGraph, One) {
    const EdgePair edges;
    ASSERT_TRUE(check_bipartite_graph(edges, 1));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges)));
}

TEST_F(TestBipartiteGraph, Self) {
    const EdgePair edges {{0, 0}};
    ASSERT_FALSE(check_bipartite_graph(edges, 1));
    ASSERT_FALSE(check_bipartite_graph(to_adjacent_matrix(edges)));
}

TEST_F(TestBipartiteGraph, Two) {
    const EdgePair edges {{0, 1}};
    ASSERT_TRUE(check_bipartite_graph(edges, 2));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges)));
}

TEST_F(TestBipartiteGraph, Three) {
    const EdgePair edges1 {{0, 1}, {1, 2}};
    ASSERT_TRUE(check_bipartite_graph(edges1, 3));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges1)));

    const EdgePair edges2 {{0, 1}, {1, 2}, {2, 0}};
    ASSERT_FALSE(check_bipartite_graph(edges2, 3));
    ASSERT_FALSE(check_bipartite_graph(to_adjacent_matrix(edges2)));
}

TEST_F(TestBipartiteGraph, Four) {
    const EdgePair edges1 {{0, 1}, {0, 3}, {2, 1}, {2, 3}};
    ASSERT_TRUE(check_bipartite_graph(edges1, 4));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges1)));

    const EdgePair edges2 {{0, 1}, {0, 2}, {0, 3}, {1, 2}};
    ASSERT_FALSE(check_bipartite_graph(edges2, 4));
    ASSERT_FALSE(check_bipartite_graph(to_adjacent_matrix(edges2)));

    const EdgePair edges3 {{0, 1}, {2, 3}};
    ASSERT_TRUE(check_bipartite_graph(edges3, 4));
    ASSERT_TRUE(check_bipartite_graph(to_adjacent_matrix(edges3)));

    const EdgePair edges4 {{0, 1}, {2, 3}, {3, 3}};
    ASSERT_FALSE(check_bipartite_graph(edges4, 4));
    ASSERT_FALSE(check_bipartite_graph(to_adjacent_matrix(edges4)));
}

// -------------------------------------------------------------------
// Combination from combinations.cpp
ModInt calc_combo(Num n, Num k) {
    if (n < k) {
        return 0;
    }

    ModInt n_combo = 1;
    for(Num i{1}; i<=k; ++i) {
        n_combo *= (n + 1 - i);
    }

    for(Num i{1}; i<=k; ++i) {
        n_combo /= i;
    }

    return n_combo;
}

class TestCalcCombo : public ::testing::Test {};

TEST_F(TestCalcCombo, Small) {
    EXPECT_EQ(1, calc_combo(0, 0).val());
    EXPECT_EQ(0, calc_combo(0, 1).val());

    const std::vector<std::vector<Num>> answers {
        {1, 1, 0},
        {1, 2, 1, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 6, 4, 1, 0},
        {1, 5, 10, 10, 5, 1, 0}
    };

    for(Num n{1}; n<=5; ++n) {
        for(Num k{0}; k<=(n+1); ++k) {
            const auto actual = calc_combo(n, k).val();
            const auto expected = answers[n-1][k];
            EXPECT_EQ(expected, actual);
        }
    }
}

TEST_F(TestCalcCombo, Large) {
    const auto actual10 = calc_combo(40, 10).val();
    const auto expected10 = 847660528;
    EXPECT_EQ(expected10, actual10);

    const auto actual11 = calc_combo(40, 11).val();
    const auto expected11 = 315312734;
    EXPECT_EQ(expected11, actual11);

    const auto actual8 = calc_combo(51, 8).val();
    const auto expected8 = 636763050;
    EXPECT_EQ(expected8, actual8);

    const auto actual9 = calc_combo(51, 9).val();
    const auto expected9 = 47579291;
    EXPECT_EQ(expected9, actual9);
}

// -------------------------------------------------------------------
// Dense rank from dense_rank.cpp
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

class TestDenseRank : public ::testing::Test {};

TEST_F(TestDenseRank, Empty) {
    std::vector<Num> input;
    const auto actual = dense_rank(input);
    EXPECT_TRUE(actual.empty());
}

TEST_F(TestDenseRank, Same) {
    std::vector<Num> input {3,3,3,3,3};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{3,0}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestDenseRank, Each) {
    std::vector<Num> input {2,4,8};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{2,0}, {4,1}, {8,2}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestDenseRank, Shuffle) {
    std::vector<Num> input {4,1,2,16,8};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{1,0}, {2,1}, {4,2}, {8,3}, {16,4}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestDenseRank, Groups1) {
    std::vector<Num> input {40,40,20,20,20,30};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{20,0}, {30,1}, {40,2}};
    ASSERT_EQ(expected, actual);
}

TEST_F(TestDenseRank, Groups2) {
    std::vector<Num> input {0,30,30,30,10,20,20,50,50};
    const auto actual = dense_rank(input);
    const std::map<Num,Num> expected {{0,0}, {10,1}, {20,2}, {30,3}, {50,4}};
    ASSERT_EQ(expected, actual);
}

// -------------------------------------------------------------------
// Dijkstra's algorithm from dijkstra.cpp and dijkstra2.cpp
struct WeightedEdge {
    Num to {0};
    Num distance {0};
};

struct Diameter {
    std::vector<Num> nodes;
    Num distance {0};
};

using WeightedEdges = std::vector<std::vector<WeightedEdge>>;
using Distances = std::vector<Num>;

void dijkstra_algo(Num n_nodes, Num start, const WeightedEdges& edges, Distances& distances) {
    Distances zeros(n_nodes, Inf);
    std::swap(distances, zeros);

    using Node = std::pair<Num, Num>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
    distances.at(start) = 0;
    q.push(std::make_pair(0, start));

    while(!q.empty()) {
        const auto [distance, from] = q.top();
        q.pop();

        if (distances.at(from) < distance) {
            continue;
        }

        for(const auto& edge : edges.at(from)) {
            const auto& to = edge.to;
            auto to_dist = distance + edge.distance;
            if (to_dist < distances.at(to)) {
                distances.at(to) = to_dist;
                q.push(std::make_pair(to_dist, to));
            }
        }
    }

    return;
}

Distances dijkstra_algo(Num n_nodes, Num start, const WeightedEdges& edges) {
    Distances distances(n_nodes, Inf);
    using Node = std::pair<Num, Num>;
    PQ<Node> q;
    distances.at(start) = 0;
    q.push(std::make_pair(start, 0));

    while(!q.empty()) {
        const auto [from, distance] = q.top();
        q.pop();

        if (distances.at(from) < distance) {
            continue;
        }

        for(const auto& [to, dist_added] : edges.at(from)) {
            const auto to_dist = distance + dist_added;
            if (to_dist < distances.at(to)) {
                distances.at(to) = to_dist;
                q.push(std::make_pair(to, to_dist));
            }
        }
    }

    return distances;
}

Distances dijkstra_loop(Num n_nodes, Num origin, Num to_init, Num dist_init, const WeightedEdges& edges) {
    Distances distances(n_nodes, Inf);
    using Node = std::pair<Num, Num>;
    PQ<Node> q;
    q.push(std::make_pair(to_init, dist_init));

    while(!q.empty()) {
        const auto [from, distance] = q.top();
        q.pop();

        if (distances.at(from) < distance) {
            continue;
        }

        for(const auto& [to, dist_added] : edges.at(from)) {
            const auto to_dist = distance + dist_added;
            if (to_dist < distances.at(to)) {
                distances.at(to) = to_dist;
                if (to == origin) {
                    continue;
                } else {
                    q.push(std::make_pair(to, to_dist));
                }
            }
        }
    }

    return distances;
}

Diameter tree_diameter(Num n_nodes, const WeightedEdges& edges) {
    auto find_min_non_inf = [](const Distances& vs) {
        Num value = -1;
        Num index = -1;
        Num size = static_cast<Num>(vs.size());

        for(Num i{0}; i<size; ++i) {
            const auto v = vs.at(i);
            if ((v < Inf) && (value < v)) {
                value = v;
                index = i;
            }
        }

        return index;
    };

    Distances distances_first;
    dijkstra_algo(n_nodes, 0, edges, distances_first);
    const auto left = find_min_non_inf(distances_first);

    Distances distances_second;
    dijkstra_algo(n_nodes, left, edges, distances_second);
    const auto right = find_min_non_inf(distances_second);
    const auto distance = distances_second.at(right);

    std::vector<Num> nodes {left, right};
    std::ranges::sort(nodes);
    const Diameter result{nodes, distance};
    return result;
}

class TestDijkstraAlgo : public ::testing::Test {};

TEST_F(TestDijkstraAlgo, DijkstraAlgo) {
    const WeightedEdges edges {{}, {{2, 10}, {4, 40}}, {{1, 6}, {4, 20}}, {}, {{1, 5}, {2, 3}}};
    const Num n = static_cast<Num>(edges.size());

    Distances distances1;
    dijkstra_algo(n, 1, edges, distances1);
    const Distances expected1 {Inf, 0, 10, Inf, 30};
    ASSERT_EQ(expected1.size(), distances1.size());
    ASSERT_TRUE(std::equal(expected1.begin(), expected1.end(), distances1.begin()));

    Distances distances2;
    dijkstra_algo(n, 2, edges, distances2);
    const Distances expected2 {Inf, 6, 0, Inf, 20};
    ASSERT_EQ(expected2.size(), distances2.size());
    ASSERT_TRUE(std::equal(expected2.begin(), expected2.end(), distances2.begin()));

    Distances distances3;
    dijkstra_algo(n, 3, edges, distances3);
    const Distances expected3 {Inf, Inf, Inf, 0, Inf};
    ASSERT_EQ(expected3.size(), distances3.size());
    ASSERT_TRUE(std::equal(expected3.begin(), expected3.end(), distances3.begin()));

    Distances distances4;
    dijkstra_algo(n, 4, edges, distances4);
    const Distances expected4 {Inf, 5, 3, Inf, 0};
    ASSERT_EQ(expected4.size(), distances4.size());
    ASSERT_TRUE(std::equal(expected4.begin(), expected4.end(), distances4.begin()));
}

TEST_F(TestDijkstraAlgo, Directed) {
    constexpr Num n_nodes = 5;
    const WeightedEdges edges {{}, {{2, 10}, {4, 40}}, {{1, 6}, {4, 20}}, {}, {{1, 5}, {2, 3}}};

    Distances distances1 = dijkstra_algo(n_nodes, 1, edges);
    const Distances expected1 {Inf, 0, 10, Inf, 30};
    ASSERT_EQ(expected1.size(), distances1.size());
    ASSERT_TRUE(std::equal(expected1.begin(), expected1.end(), distances1.begin()));

    Distances distances2 = dijkstra_algo(n_nodes, 2, edges);
    const Distances expected2 {Inf, 6, 0, Inf, 20};
    ASSERT_EQ(expected2.size(), distances2.size());
    ASSERT_TRUE(std::equal(expected2.begin(), expected2.end(), distances2.begin()));

    Distances distances3 = dijkstra_algo(n_nodes, 3, edges);
    const Distances expected3 {Inf, Inf, Inf, 0, Inf};
    ASSERT_EQ(expected3.size(), distances3.size());
    ASSERT_TRUE(std::equal(expected3.begin(), expected3.end(), distances3.begin()));

    Distances distances4 = dijkstra_algo(n_nodes, 4, edges);
    const Distances expected4 {Inf, 5, 3, Inf, 0};
    ASSERT_EQ(expected4.size(), distances4.size());
    ASSERT_TRUE(std::equal(expected4.begin(), expected4.end(), distances4.begin()));
}

TEST_F(TestDijkstraAlgo, Loop) {
    constexpr Num n_nodes = 6;
    const WeightedEdges edges {{{1, 3}}, {{0, 5}}, {{0, 1}, {3, 11}}, {{4, 22}}, {{2, 33}}, {{1, 1}}};
    const std::vector<std::vector<Num>> expected {{8}, {8}, {Inf, 66}, {66}, {66}, {Inf}};

    for(Num x{0}; x<n_nodes; ++x) {
        for(size_t i{0}; i<expected.at(x).size(); ++i) {
            const auto [y, c] = edges.at(x).at(i);
            const auto actual = dijkstra_loop(n_nodes, x, y, c, edges);
            EXPECT_EQ(expected.at(x).at(i), actual.at(x));
        }
    }
}

TEST_F(TestDijkstraAlgo, TreeDiameter1) {
    const WeightedEdges edges {
        {{3, 1}},
        {{3, 1}},
        {{3, 1}},
        {{0, 1}, {1, 1}, {2, 1}, {4, 1}},
        {{3, 1}, {5, 20}, {6, 100}},
        {{4, 20}},
        {{4, 100}},
        {}
    };
    const Num n = static_cast<Num>(edges.size());

    const auto actual = tree_diameter(n, edges);
    const std::vector<Num> expected {5,6};
    ASSERT_EQ(expected, actual.nodes);
    ASSERT_EQ(120, actual.distance);
}

TEST_F(TestDijkstraAlgo, TreeDiameter2) {
    const WeightedEdges edges {
        {{2, 3}},
        {{2, 100}},
        {{0, 3}, {1, 100}, {3, 5}},
        {{2, 5}, {4, 20}, {5, 20}},
        {{3, 20}},
        {{3, 20}}
    };
    const Num n = static_cast<Num>(edges.size());

    const auto actual = tree_diameter(n, edges);
    const std::vector<Num> expected {1,4};
    ASSERT_EQ(expected, actual.nodes);
    ASSERT_EQ(125, actual.distance);
}

// -------------------------------------------------------------------
// Eular tour.cpp from eular_tour.cpp
void eular_tour(Num current, Num prev, const Edges& edges, Path& path) {
    path.push_back(current);

    for(const auto& next : edges[current]) {
        if (next != prev) {
            eular_tour(next, current, edges, path);
            path.push_back(current);
        }
    }
}

class TestEularTour : public ::testing::Test {};

TEST_F(TestEularTour, All) {
    Edges edges(7);
    std::vector<Num> to1 {2,5};
    std::vector<Num> to2 {3,4};
    std::vector<Num> to4 {6};
    edges.at(1) = to1;
    edges.at(2) = to2;
    edges.at(4) = to4;

    Path actual;
    eular_tour(1, 0, edges, actual);
    const Path expected {1,2,3,2,4,6,4,2,1,5,1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

// -------------------------------------------------------------------
// Extended GCD from extended_gcd.cpp
Num extended_gcd(Num a, Num b, Num& x, Num& y) {
    Num d = a;
    if (b != 0) {
        d = extended_gcd(b, a % b, y, x);
        y -= (a / b) * x;
    } else {
        x = 1;
        y = 0;
    }

    return d;
}

Num abs_gcd(Num a, Num b) {
    return std::gcd(std::abs(a), std::abs(b));
}

Num inner_product(Num a, Num b, Num x, Num y) {
    return a * x + b * y;
}

class TestAbsGcd : public ::testing::Test {};

TEST_F(TestAbsGcd, MutuallyPrime) {
    for(const auto& [sign_a, sign_b] : signs) {
        Num a {3};
        Num b {5};
        EXPECT_EQ(1, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(1, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Even) {
    for(const auto& [sign_a, sign_b] : signs) {
        Num a {6};
        Num b {10};
        EXPECT_EQ(2, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(2, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Power) {
    for(const auto& [sign_a, sign_b] : signs) {
        constexpr Num expected {3};
        Num a {expected};
        Num b = a * a * a;
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
    }
}

TEST_F(TestAbsGcd, Zero) {
    for(const auto& [sign_a, sign_b] : signs) {
        constexpr Num expected {4};
        Num a {expected};
        Num b {0};
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
        std::swap(a, b);
        EXPECT_EQ(expected, abs_gcd(a * sign_a, b * sign_b));
    }
}

class TestExtendedGcd : public ::testing::Test {};

TEST_F(TestExtendedGcd, MutuallyPrime) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const std::vector<Num> expected_y_signs {-1, 1, 1, -1};
    const Num base_a {3};
    const Num base_b {5};

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        ASSERT_GT(expected_y_signs.size(), trial);

        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const auto& y_sign = expected_y_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(2, x);
        EXPECT_EQ(1 * y_sign, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Even) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const std::vector<Num> expected_y_signs {-1, 1, 1, -1};
    const Num base_a {6};
    const Num base_b {10};

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        ASSERT_GT(expected_y_signs.size(), trial);

        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const auto& y_sign = expected_y_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(2, x);
        EXPECT_EQ(1 * y_sign, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Power) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const Num base_a {3};
    const Num base_b = base_a * base_a * base_a;

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, x);
        EXPECT_EQ(0, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

TEST_F(TestExtendedGcd, Zero) {
    const std::vector<Num> expected_gcd_signs {1, 1, -1, -1};
    const Num base_a {4};
    const Num base_b = 0;

    for(size_t trial{0}; trial<signs.size(); ++trial) {
        ASSERT_GT(expected_gcd_signs.size(), trial);
        const auto& [sign_a, sign_b] = signs.at(trial);
        const auto& gcd_sign = expected_gcd_signs.at(trial);
        const Num a = base_a * sign_a;
        const Num b = base_b * sign_b;
        Num x {0};
        Num y {0};

        const auto actual = extended_gcd(a, b, x, y);
        const auto expected = abs_gcd(a, b) * gcd_sign;
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, x);
        EXPECT_EQ(0, y);
        EXPECT_EQ(expected, inner_product(a, b, x, y));
    }
}

// -------------------------------------------------------------------
// Prime Factorize from factorize.cpp
using Factors = std::vector<Num>;
using FactorMap = std::map<Num, Num>;

FactorMap prime_factorize(Num x) {
    FactorMap factors;
    auto r = x;

    for(Num p{2}; (p*p)<=x; ++p) {
        while((r % p) == 0) {
            factors[p] += 1;
            r /= p;
        }

        if (r == 1) {
            break;
        }
    }

    if (r > 1) {
        factors[r] = 1;
    }

    return factors;
}

Factors factorize(Num x) {
    Factors factors;
    for(Num p{1}; (p*p)<=x; ++p) {
        if ((x % p) != 0) {
            continue;
        }

        factors.push_back(p);
        const auto q = x / p;
        if (q != p) {
            factors.push_back(q);
        }
    }

    std::sort(factors.begin(), factors.end());
    auto it = std::unique(factors.begin(), factors.end());
    factors.erase(it, factors.end());
    return factors;
}

class TestFactorize : public ::testing::Test {};

TEST_F(TestFactorize, PrimeFactorize1) {
    auto ps = prime_factorize(1);
    ASSERT_EQ(0LL, ps.size());
}

TEST_F(TestFactorize, PrimeFactorizePrimes) {
    auto ps2 = prime_factorize(2);
    ASSERT_EQ(1LL, ps2.size());
    ASSERT_EQ(1LL, ps2[2]);

    auto ps3 = prime_factorize(3);
    ASSERT_EQ(1LL, ps3.size());
    ASSERT_EQ(1LL, ps3[3]);

    Num p {1};
    p <<= 31;
    p -= 1;
    auto ps = prime_factorize(p);
    ASSERT_EQ(1LL, ps.size());
    ASSERT_EQ(1LL, ps[p]);
}

TEST_F(TestFactorize, PrimeFactorizePowered) {
    auto ps8 = prime_factorize(8);
    ASSERT_EQ(1LL, ps8.size());
    ASSERT_EQ(3LL, ps8[2]);

    Num p {1};
    p <<= 16;
    p -= 15;
    auto ps = prime_factorize(p*p);
    ASSERT_EQ(1LL, ps.size());
    ASSERT_EQ(2LL, ps[p]);
}

TEST_F(TestFactorize, PrimeFactorizeCompound) {
    // 2*2*2*3*3*3*3*5
    auto ps = prime_factorize(3240);
    ASSERT_EQ(3LL, ps.size());
    ASSERT_EQ(3LL, ps[2]);
    ASSERT_EQ(4LL, ps[3]);
    ASSERT_EQ(1LL, ps[5]);
}

TEST_F(TestFactorize, Factorize1) {
    const auto actual = factorize(1);
    const Factors expected {1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestFactorize, FactorizePrimes) {
    std::vector<Num> input {2,3,5,7};
    for(const auto& p : input) {
        const auto actual = factorize(p);
        const Factors expected {1,p};
        ASSERT_EQ(expected.size(), actual.size());
        ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));

        for(Num i{1}; i<=5; ++i) {
            Factors expected_local {1};
            Num n {1};
            for(Num j{0}; j<i; ++j) {
                n *= p;
                expected_local.push_back(n);
            }

            const auto actual_local = factorize(n);
            ASSERT_EQ(expected_local.size(), actual_local.size());
            ASSERT_TRUE(std::equal(actual_local.begin(), actual_local.end(), expected_local.begin()));
        }
    }
}

TEST_F(TestFactorize, FactorizeOthers) {
    const auto actual = factorize(360);
    const Factors expected {
        1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20,
        24, 30, 36, 40, 45, 60, 72, 90, 120, 180, 360
    };
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

// -------------------------------------------------------------------
// Fenwick tree from fenwick_tree.cpp
struct Bit {
    std::vector<Num> bits_;
    Num max_ {0};
    Bit(Num size) : bits_(size+1, 0), max_(size) {}

    void increment(Num pos) {
        for(auto i=pos; i<=max_; i+=(i&-i)) {
            bits_.at(i) += 1;
        }
    }

    void decrement(Num pos) {
        for(auto i=pos; i<=max_; i+=(i&-i)) {
            bits_.at(i) -= 1;
        }
    }

    Num count(Num right) const {
        Num total {0};
        for (auto i=right; i>0; i-=(i&-i)) {
            total += bits_.at(i);
        }
        return total;
    }
};

class TestFenwickTree : public ::testing::Test {};

TEST_F(TestFenwickTree, Sequential) {
    constexpr Num n = 11;
    Bit bit(n);

    for(Num i{1}; i<=n; ++i) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }

    for(Num i{n}; i>=1; --i) {
        bit.decrement(i);
        ASSERT_EQ(i-1, bit.count(i));
        for(Num j{1}; j<=(i-1); ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }
}

TEST_F(TestFenwickTree, OddEven) {
    constexpr Num n = 19;
    Bit bit(n);

    for(Num i{1}; i<=n; i+=2) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = (j + 1) / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
    }

    for(Num i{2}; i<=n; i+=2) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }

    for(Num i{1}; i<=n; i+=2) {
        bit.decrement(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = j / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
    }

    for(Num i{2}; i<=n; i+=2) {
        bit.decrement(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = (j <= i) ? 0 : (j - i) / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
    }
}

// -------------------------------------------------------------------
// Fixed point numbers from fixed_point_number.cpp
std::pair<Num, Num>
str_to_fixed_point(const std::string& s, size_t n_frac_digits) {
    const auto pos = s.find('.');
    if (pos == std::string::npos) {
        std::pair<Num, Num> num {std::atoll(s.c_str()), 0};
        return num;
    }

    const auto int_part_str = s.substr(0, pos);
    const Num int_part = std::atoll(int_part_str.c_str());

    const auto len = std::min(s.size() - pos - 1, n_frac_digits);
    const auto frac_part_str = s.substr(pos + 1, len);
    Num frac_part = std::atoll(frac_part_str.c_str());

    for(size_t i = frac_part_str.size(); i < n_frac_digits; ++i) {
        frac_part *= 10;
    }

    std::pair<Num, Num> num {int_part, frac_part};
    return num;
}

class TestFixedPointNumber : public ::testing::Test {};

TEST_F(TestFixedPointNumber, Zero) {
    const std::vector<std::string> input {
        "", ".", "0", "0.", "0.0", "0.00", "0.000",
        "00", "00.", "00.0", "00.00", "00.000"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(0LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestFixedPointNumber, One) {
    const std::vector<std::string> input {
        "1", "1.", "1.0", "1.00", "01", "01.", "01.0", "01.00"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(1LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestFixedPointNumber, Int) {
    const std::vector<std::string> input {
        "987", "987.", "987.0", "987.00", "0987", "0987.", "0987.0", "0987.00"
    };

    for(const auto& s : input) {
        for(size_t n_frac_digits {0}; n_frac_digits < 5; ++n_frac_digits) {
            const auto [int_part, frac_part] = str_to_fixed_point(s, n_frac_digits);
            EXPECT_EQ(987LL, int_part);
            EXPECT_EQ(0LL, frac_part);
        }
    }
}

TEST_F(TestFixedPointNumber, Frac) {
    const std::string input {"76.543"};
    const long long int expected_int = 76;

    const auto [int_part0, frac_part0] = str_to_fixed_point(input, 0);
    EXPECT_EQ(expected_int, int_part0);
    EXPECT_EQ(0LL, frac_part0);

    const auto [int_part1, frac_part1] = str_to_fixed_point(input, 1);
    EXPECT_EQ(expected_int, int_part1);
    EXPECT_EQ(5LL, frac_part1);

    const auto [int_part2, frac_part2] = str_to_fixed_point(input, 2);
    EXPECT_EQ(expected_int, int_part2);
    EXPECT_EQ(54LL, frac_part2);

    long long int expected_frac = 543;
    for(size_t n_frac_digits {3}; n_frac_digits < 7; ++n_frac_digits) {
        const auto [int_part, frac_part] = str_to_fixed_point(input, n_frac_digits);
        EXPECT_EQ(expected_int, int_part);
        EXPECT_EQ(expected_frac, frac_part);
        expected_frac *= 10;
    }
}

TEST_F(TestFixedPointNumber, Sample) {
    const auto [int_part, frac_part] = str_to_fixed_point("65.4321", 4);
    EXPECT_EQ(65LL, int_part);
    EXPECT_EQ(4321LL, frac_part);
}

// -------------------------------------------------------------------
// Floyd Warshall from floyd_warshall.cpp
using Distance = long long int;
constexpr Num MaxWidth = 1000;
using FloydWarshallMatrix = std::array<std::array<Num, MaxWidth>, MaxWidth>;

void floyd_warshall(Num n, FloydWarshallMatrix& distances) {
    for(decltype(n) i{0}; i<n; ++i) {
        for(decltype(n) x{0}; x<n; ++x) {
            for(decltype(n) y{0}; y<n; ++y) {
                distances[x][y] = std::min(distances[x][y], distances[x][i] + distances[i][y]);
            }
        }
    }
}

void init_floyd_warshall_matrix(FloydWarshallMatrix& distances, Num value) {
    for(Num i{0}; i<MaxWidth; ++i) {
        for(Num j{0}; j<MaxWidth; ++j) {
            distances[i][j] = value;
        }
    }
}

class TestFloydWarshall : public ::testing::Test {
protected:
    FloydWarshallMatrix actual;
    FloydWarshallMatrix expected;
};

TEST_F(TestFloydWarshall, All) {
    constexpr Num n = 5;
    static_assert(n < MaxWidth);
    constexpr Distance inf = 100000000000LL;

    init_floyd_warshall_matrix(actual, inf);
    actual[0][1] = 10;
    actual[1][3] = 20;
    actual[0][3] = 40;
    actual[1][1] = 0;
    actual[1][0] = 6;
    actual[3][1] = 3;
    actual[3][0] = 5;

    init_floyd_warshall_matrix(expected, inf);
    expected[0][0] = 16;
    expected[0][1] = 10;
    expected[0][2] = inf;
    expected[0][3] = 30;
    expected[1][0] = 6;
    expected[1][1] = 0;
    expected[1][2] = inf;
    expected[1][3] = 20;
    expected[2][0] = inf;
    expected[2][1] = inf;
    expected[2][2] = inf;
    expected[2][3] = inf;
    expected[3][0] = 5;
    expected[3][1] = 3;
    expected[3][2] = inf;
    expected[3][3] = 23;

    floyd_warshall(n, actual);
    for(Num x{1}; x<=n; ++x) {
        for(Num y{1}; y<=n; ++y) {
            EXPECT_EQ(expected[y][x], actual[y][x]);
        }
    }
}

// use std::gcd instead of gcd.cpp
// use atcoder::modint instead of mod_div.cpp
// -------------------------------------------------------------------
// 直線の標準形(canonical form) from line.cpp
struct Line {
    bool vertical_ {false};  // Y軸に平行、X軸に垂直
    Rational slope_ {0};
    Rational intercept_ {0};

    Line(Rational x0, Rational y0, Rational x1, Rational y1) {
        const auto dy = y1 - y0;
        const auto dx = x1 - x0;

        if (dx == 0) {
            vertical_ = true;
            intercept_ = x0;
        } else {
            slope_ = dy;
            slope_ /= dx;

            auto height = slope_;
            height *= x0;
            intercept_ = y0;
            intercept_ -= height;
        }
    }

    Line(bool vertical, Rational slope, Rational intercept) :
        vertical_(vertical), slope_(slope), intercept_(intercept) {
    }

    // 線の上にある点を中心に90度回転する
    // 線の上に無い点については未定義
    Line rotate90on(Rational x, Rational y) const {
        bool vertical {false};
        Rational slope {0};
        Rational intercept {0};

        if (vertical_) {
            // 水平線にする
            intercept = y;
        } else if (slope_ == 0) {
            // 垂直線にする
            vertical = true;
            intercept = x;
        } else {
            slope = -1;
            slope /= slope_;
            Rational height = slope;
            height *= x;
            intercept = y;
            intercept -= height;
        }

        return Line(vertical, slope, intercept);
    }

    bool is_equal(const Line& other) const {
        return ((vertical_ == other.vertical_) && (slope_ == other.slope_) && (intercept_ == other.intercept_));
    }

    bool operator==(const Line& other) const {
        return is_equal(other);
    }

    bool operator!=(const Line& other) const {
        return !is_equal(other);
    }

    // 交点があればtrue
    bool cross(const Line& other) const {
        if (vertical_ != other.vertical_) {
            return true;
        }

        if (slope_ != other.slope_) {
            return true;
        }

        return intercept_ == other.intercept_;
    };

    void print(std::ostream& os) const {
        os << vertical_ << " " << slope_ << ":" << intercept_ << "\n";
    }
};

class TestLine : public ::testing::Test {};

TEST_F(TestLine, Initialize) {
    const Num xunit = 3;
    const Num yunit = 2;

    for(Num dx{-9}; dx<=9; dx+=3) {
        const Line vertival(true, 0, dx);
        for(Num dy{-5}; dy<=5; ++dy) {
            const Line horizontal(false, 0, dy);
            Rational expected_slope = yunit;
            expected_slope /= xunit;
            Rational expected_intercept = 1;
            expected_intercept /= xunit;
            expected_intercept += dy;
            expected_intercept -= (dx * yunit) / xunit;

            const Line expected(false, expected_slope, expected_intercept);
            const Line actual1(4 + dx, 3 + dy, 7 + dx, 5 + dy);
            const Line actual2(1 + dx, 1 + dy, 7 + dx, 5 + dy);
            EXPECT_TRUE(expected == actual1);
            EXPECT_FALSE(expected != actual1);

            EXPECT_TRUE(expected == actual2);
            EXPECT_FALSE(expected != actual2);

            EXPECT_FALSE(expected == vertival);
            EXPECT_TRUE(expected != vertival);

            EXPECT_FALSE(expected == horizontal);
            EXPECT_TRUE(expected != horizontal);

            if ((dx != 0) && (dy != 0)) {
                const Line cross(1 + dx, 1 + dy, 7 + dx, 5 - dy);
                EXPECT_FALSE(expected == cross);
                EXPECT_TRUE(expected != cross);
            }
        }
    }
}

TEST_F(TestLine, Vertical) {
    for(Num dx{-1}; dx<=1; ++dx) {
        const Line horizontal(false, 0, dx);
        const Line expected(true, 0, dx);
        const Line actual(dx, -2, dx, 3);
        EXPECT_TRUE(expected == actual);
        EXPECT_FALSE(expected != actual);
        EXPECT_FALSE(expected == horizontal);
        EXPECT_TRUE(expected != horizontal);
    }
}

TEST_F(TestLine, Horizontal) {
    for(Num dy{-1}; dy<=1; ++dy) {
        const Line vertical(true, 0, dy);
        const Line expected(false, 0, dy);
        const Line actual(-2, dy, 3, dy);
        EXPECT_TRUE(expected == actual);
        EXPECT_FALSE(expected != actual);
        EXPECT_FALSE(expected == vertical);
        EXPECT_TRUE(expected != vertical);
    }
}

TEST_F(TestLine, Rotate) {
    Rational slope1 {1};
    slope1 /= 4;
    Rational intercept1 {5};
    intercept1 /= 2;
    const Line line1(false, slope1, intercept1);

    Rational slope2 {-4};
    Rational intercept2 {11};
    const Line line2(false, slope2, intercept2);

    EXPECT_TRUE(line2 == line1.rotate90on(2, 3));
    EXPECT_TRUE(line1 == line2.rotate90on(2, 3));
}

TEST_F(TestLine, RotateVertical) {
    const auto sx = 5;
    const auto sy = 7;
    const Line vertical(true, 0, sx);
    const Line horizontal(false, 0, sy);
    EXPECT_TRUE(vertical == horizontal.rotate90on(sx, sy));
    EXPECT_TRUE(horizontal == vertical.rotate90on(sx, sy));
}

TEST_F(TestLine, Cross) {
    const auto sx = 5;
    const auto sy = 7;
    const auto dx = 5;
    const auto dy = 7;
    const auto ofs = 4;

    const Line vertical(true, 0, sx);
    const Line vertical_alt(true, 0, sx + 1);
    const Line horizontal(false, 0, sy);
    const Line horizontal_alt(false, 0, sy - 1);
    const Line line(sx, dx+dx, sy, sy+dy);
    const Line cross(sx, dx+dx, sy, sy-dy);
    const Line parallel(sx+ofs, dx+dx+ofs, sy+ofs, sy+dy+ofs);

    EXPECT_TRUE(vertical.cross(vertical));
    EXPECT_FALSE(vertical.cross(vertical_alt));
    EXPECT_TRUE(vertical.cross(horizontal));
    EXPECT_TRUE(vertical.cross(line));
    EXPECT_TRUE(vertical.cross(cross));
    EXPECT_TRUE(vertical.cross(parallel));

    EXPECT_TRUE(horizontal.cross(vertical));
    EXPECT_TRUE(horizontal.cross(horizontal));
    EXPECT_FALSE(horizontal.cross(horizontal_alt));
    EXPECT_TRUE(horizontal.cross(line));
    EXPECT_TRUE(horizontal.cross(cross));
    EXPECT_TRUE(horizontal.cross(parallel));

    EXPECT_TRUE(line.cross(vertical));
    EXPECT_TRUE(line.cross(vertical_alt));
    EXPECT_TRUE(line.cross(horizontal));
    EXPECT_TRUE(line.cross(horizontal_alt));
    EXPECT_TRUE(line.cross(line));
    EXPECT_TRUE(line.cross(cross));
    EXPECT_FALSE(line.cross(parallel));

    EXPECT_TRUE(cross.cross(cross));
    EXPECT_TRUE(cross.cross(line));
    EXPECT_FALSE(line.cross(parallel));

    EXPECT_TRUE(parallel.cross(parallel));
    EXPECT_FALSE(parallel.cross(line));
    EXPECT_TRUE(parallel.cross(cross));
}

// -------------------------------------------------------------------
// Linear sieve from linear_sieve.cpp
void linear_sieve(Num upper, Vec& least_primes, Vec& primes) {
    Vec empty_least_primes(upper + 1);
    std::swap(empty_least_primes, least_primes);

    Vec empty_primes;
    std::swap(empty_primes, primes);

    for(Num i{2}; i<=upper; ++i) {
        if (least_primes.at(i) == 0) {
            least_primes.at(i) = i;
            primes.push_back(i);
        }

        for(const auto& prime : primes) {
            const auto prod = prime * i;
            if ((prod > upper) || (prime > least_primes.at(i))) {
                break;
            }
            least_primes.at(prod) = prime;
        }
    }
}

std::map<Num,Num> prime_factorize(Num x, const Vec& least_primes) {
    Num rem {x};
    std::map<Num,Num> table;

    while(rem > 1) {
        const auto p = least_primes.at(rem);
        table[p] += 1;
        rem /= p;
    }

    return table;
}

const Vec full_primes {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

class TestLinearSieve : public ::testing::Test {};

TEST_F(TestLinearSieve, LeastPrimes) {
    const Vec expected {0, 0, 2, 3, 2, 5, 2, 7, 2, 3, 2};
    Vec least_primes;
    Vec primes;
    linear_sieve(10, least_primes, primes);
    ASSERT_EQ(expected, least_primes);
}

TEST_F(TestLinearSieve, LeastPrimesFull) {
    constexpr Num upper = 100;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    Vec expected(upper + 1);
    for(Num i{2}; i<=upper; ++i) {
        for(const auto& prime : full_primes) {
            if ((i % prime) == 0) {
                expected.at(i) = prime;
                break;
            }
        }
    }

    ASSERT_EQ(expected, least_primes);
}

TEST_F(TestLinearSieve, Primes) {
    for(Num i{2}; i<100; ++i) {
        Vec least_primes;
        Vec primes;
        linear_sieve(i, least_primes, primes);

        auto it = std::ranges::upper_bound(full_primes, i);
        Vec expected;
        std::copy(full_primes.begin(), it, std::back_inserter(expected));
        ASSERT_EQ(expected, primes);
    }
}

TEST_F(TestLinearSieve, Large) {
    constexpr Num upper = 10000009;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);
    ASSERT_EQ(least_primes.size(), upper + 1);
}

class TestPrimeFactorize : public ::testing::Test {};

TEST_F(TestPrimeFactorize, Small) {
    constexpr Num upper = 10;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    std::vector<std::map<Num,Num>> expected {
        {}, {}, {{2,1}}, {{3,1}}, {{2,2}}, {{5,1}},
        {{2,1},{3,1}}, {{7,1}}, {{2,3}}, {{3,2}}, {{2,1},{5,1}}
    };

    for(Num i{2}; i<=2; ++i) {
        const auto actual = prime_factorize(i, least_primes);
        ASSERT_EQ(expected.at(i), actual);
    }
}

TEST_F(TestPrimeFactorize, Auto) {
    constexpr Num upper = 100;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    for(Num i{2}; i<100; ++i) {
        Num rem {i};
        std::map<Num,Num> expected;
        for(const auto& prime : primes) {
            while ((rem % prime) == 0) {
                expected[prime] += 1;
                rem /= prime;
            }
        }

        const auto actual = prime_factorize(i, least_primes);
        ASSERT_EQ(expected, actual);
    }
}

TEST_F(TestPrimeFactorize, Large) {
    constexpr Num upper = 10000009;
    Vec least_primes;
    Vec primes;
    linear_sieve(upper, least_primes, primes);

    constexpr Num size = 200000;
    std::vector<std::map<Num,Num>> ans;
    for(Num i{upper-size+1}; i<=upper; ++i) {
        ans.push_back(prime_factorize(i, least_primes));
    }

    ASSERT_EQ(ans.size(), size);
}

// -------------------------------------------------------------------
// LIS from longest_inc_subseq.cpp
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

Num apply_num_max(Num a, Num b) {
    return std::max(a,b);
}

Num unit_num_max() {
    return std::numeric_limits<Num>::min();
}

using LisSegTree = atcoder::segtree<Num, apply_num_max, unit_num_max>;

LisSegTree lis_tree(const std::vector<Num>& xs) {
    const auto size = static_cast<SegtreeNode>(xs.size());
    LisSegTree dp(size);

    for(SegtreeNode i{0}; i<size; ++i) {
        const auto& x = xs.at(i);
        dp.set(static_cast<SegtreeNode>(x), std::max(0LL, dp.prod(0, static_cast<SegtreeNode>(x))) + 1);
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
            EXPECT_EQ(expected, tree.get(static_cast<SegtreeNode>(i-1)));
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
            EXPECT_EQ(1, tree.get(static_cast<SegtreeNode>(i-1)));
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
        EXPECT_EQ(expected.at(i), tree.get(static_cast<SegtreeNode>(serial)));
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
        EXPECT_EQ(expected.at(i), tree.get(static_cast<SegtreeNode>(serial)));
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
        EXPECT_EQ(expected.at(i), tree.get(static_cast<SegtreeNode>(serial)));
    }
}

// omit multiarray.cpp
// omit n_lines.cpp
// omit norm1_positions.cpp
// -------------------------------------------------------------------
// n-th element from nth_element_set.cpp
class TestNthElementSet : public ::testing::Test {};

TEST_F(TestNthElementSet, All) {
    using Element = int;
    const std::vector<Element> vs {1,2,3,5,8,13,21};
    std::vector<Element> sorted = vs;
    std::ranges::sort(sorted);

    using ordered_set = tree<
        Element,
        null_type, std::less<Element>,
        rb_tree_tag,
        tree_order_statistics_node_update>;

    ordered_set s;
    for(const auto& x : sorted) {
        s.insert(x);
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

class TestNthElementSMultiset : public ::testing::Test {};

TEST_F(TestNthElementSMultiset, All) {
    using Element = int;
    const std::vector<Element> vs {5,6,6,6,7,7,8,9,9,9};
    std::vector<Element> sorted = vs;
    std::ranges::sort(sorted);

    using ordered_multiset_local = tree<
        Element,
        null_type, std::less_equal<Element>,
        rb_tree_tag,
        tree_order_statistics_node_update>;

    ordered_multiset_local s;
    for(const auto& x : sorted) {
        s.insert(x);
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

class TestNthElementMap : public ::testing::Test {};

TEST_F(TestNthElementMap, All) {
    using Key = std::string;
    using Value = int;
    using Node = std::pair<Key,Value>;
    const std::vector<Node> vs {
        {"efg", 40},
        {"pqr", 30},
        {"wxy", 20},
    };
    std::vector<Node> sorted = vs;
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

// omit padding.cpp
// omit palindrome.cpp
// use atcoder::modint instead of power.cpp
// -------------------------------------------------------------------
// Matrix powered from power_mat.cpp
template <typename T, size_t Size>
struct MatrixFixedPoweredBase {
    using Elements = std::array<std::array<T, Size>, Size>;
    Elements elements_;
    T mod_ {1};

    MatrixFixedPoweredBase(T diag, T mod) : mod_(mod) {
        for(size_t row{0}; row<Size; ++row) {
            for(size_t column{0}; column<Size; ++column) {
                elements_.at(row).at(column) = (row == column) ? diag : 0;
            }
        }
    }

    MatrixFixedPoweredBase mul(const MatrixFixedPoweredBase& other) const {
        MatrixFixedPoweredBase prod(0, mod_);

        for(size_t row{0}; row<Size; ++row) {
            for(size_t column{0}; column<Size; ++column) {
                for(size_t i{0}; i<Size; ++i) {
                    auto& cell = prod.elements_.at(row).at(column);
                    cell = static_cast<T>(cell + (elements_.at(row).at(i) * other.elements_.at(i).at(column)));
                    cell = static_cast<T>(cell % mod_);
                    cell = static_cast<T>((cell + mod_) % mod_);
                }
            }
        }

        return prod;
    }

    T get(size_t row, size_t column) const {
        return elements_.at(row).at(column);
    }
};

template <typename T>
struct MatrixFixedPowered {
    static inline constexpr size_t Width {64};
    std::vector<T> matrix_powered_by_2_;

    MatrixFixedPowered(const T& mat) {
        matrix_powered_by_2_.push_back(mat);

        for(size_t i{0}; (i+1)<Width; ++i) {
            const auto& base = matrix_powered_by_2_.back();
            const auto powered = base.mul(base);
            matrix_powered_by_2_.push_back(powered);
        }
    }

    T power(size_t index) {
        const auto mod = matrix_powered_by_2_.at(0).mod_;
        T prod(1, mod);
        std::bitset<Width> bits(index);

        for(size_t i{0}; i<64; ++i) {
            if (bits[i]) {
                prod = prod.mul(matrix_powered_by_2_.at(i));
            }
        }
        return prod;
    }
};

template <typename T>
struct MatrixVarBase {
    using Elements = std::vector<std::vector<T>>;
    Elements elements_;
    T mod_ {1};
    size_t size_ {0};

    MatrixVarBase(T diag, T mod, size_t size) :
        elements_(size, std::vector<T>(size)), mod_(mod), size_(size) {
        for(size_t row{0}; row<size_; ++row) {
            for(size_t column{0}; column<size_; ++column) {
                elements_.at(row).at(column) = (row == column) ? diag : 0;
            }
        }
    }

    MatrixVarBase mul(const MatrixVarBase& other) const {
        MatrixVarBase prod(0, mod_, size_);

        for(size_t row{0}; row<size_; ++row) {
            for(size_t column{0}; column<size_; ++column) {
                for(size_t i{0}; i<size_; ++i) {
                    auto& cell = prod.elements_.at(row).at(column);
                    cell = static_cast<T>(cell + (elements_.at(row).at(i)) * static_cast<T>(other.elements_.at(i).at(column)));
                    cell = static_cast<T>(cell % mod_);
                    cell = static_cast<T>((cell + mod_) % mod_);
                }
            }
        }

        return prod;
    }

    T get(size_t row, size_t column) const {
        return elements_.at(row).at(column);
    }
};

template <typename T>
struct MatrixVarPowered {
    static inline constexpr size_t Width {64};
    std::vector<T> matrix_powered_by_2_;

    MatrixVarPowered(const T& mat) {
        matrix_powered_by_2_.push_back(mat);

        for(size_t i{0}; (i+1)<Width; ++i) {
            const auto& base = matrix_powered_by_2_.back();
            const auto powered = base.mul(base);
            matrix_powered_by_2_.push_back(powered);
        }
    }

    T power(size_t index) {
        const auto mod = matrix_powered_by_2_.at(0).mod_;
        const auto size = matrix_powered_by_2_.at(0).size_;
        T prod(1, mod, size);
        std::bitset<Width> bits(index);

        for(size_t i{0}; i<64; ++i) {
            if (bits[i]) {
                prod = prod.mul(matrix_powered_by_2_.at(i));
            }
        }
        return prod;
    }
};

// ABC 293-E
// https://atcoder.jp/contests/abc293/tasks/abc293_e
class TestMatrixFixedPowered : public ::testing::Test {};

TEST_F(TestMatrixFixedPowered, Elements) {
    using Num = int32_t;
    using Mat = MatrixFixedPoweredBase<Num, 2>;
    const std::vector<Num> mods {2, 3, 256};
    for(const auto& mod : mods) {
        Mat base = Mat(0, mod);
        base.elements_.at(0).at(0) = 1;
        base.elements_.at(0).at(1) = 2;
        base.elements_.at(1).at(0) = 3;
        base.elements_.at(1).at(1) = 4;

        MatrixFixedPowered<Mat> mat_powered(base);
        const auto powered = mat_powered.power(2);
        const Num expected00 = 7 % mod;
        const Num expected01 = 10 % mod;
        const Num expected10 = 15 % mod;
        const Num expected11 = 22 % mod;
        EXPECT_EQ(expected00, powered.get(0, 0));
        EXPECT_EQ(expected01, powered.get(0, 1));
        EXPECT_EQ(expected10, powered.get(1, 0));
        EXPECT_EQ(expected11, powered.get(1, 1));
    }
}

TEST_F(TestMatrixFixedPowered, Case1) {
    using Num = int16_t;
    using Mat = MatrixFixedPoweredBase<Num, 2>;
    constexpr Num a = 3;
    constexpr Num x = 4;
    constexpr Num m = 7;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixFixedPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(5, powered.get(0, 1));
}

TEST_F(TestMatrixFixedPowered, Case2) {
    using Num = int32_t;
    using Mat = MatrixFixedPoweredBase<Num, 2>;
    constexpr Num a = 8;
    constexpr Num x = 10;
    constexpr Num m = 9;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixFixedPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(0, powered.get(0, 1));
}

TEST_F(TestMatrixFixedPowered, Case3) {
    using Num = uint64_t;
    using Mat = MatrixFixedPoweredBase<Num, 2>;
    constexpr Num a = 1000000000ULL;
    constexpr Num x = 1000000000000ULL;
    constexpr Num m = 998244353;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixFixedPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(919667211ULL, powered.get(0, 1));
}

// ABC 293-E
// https://atcoder.jp/contests/abc293/tasks/abc293_e
class TestMatrixVarPowered : public ::testing::Test {};

TEST_F(TestMatrixVarPowered, Elements) {
    constexpr size_t Size = 2;
    using Num = int32_t;
    using Mat = MatrixVarBase<Num>;
    const std::vector<Num> mods {2, 3, 256};
    for(const auto& mod : mods) {
        Mat base = Mat(0, mod, Size);
        base.elements_.at(0).at(0) = 1;
        base.elements_.at(0).at(1) = 2;
        base.elements_.at(1).at(0) = 3;
        base.elements_.at(1).at(1) = 4;

        MatrixVarPowered<Mat> mat_powered(base);
        const auto powered = mat_powered.power(2);
        const Num expected00 = 7 % mod;
        const Num expected01 = 10 % mod;
        const Num expected10 = 15 % mod;
        const Num expected11 = 22 % mod;
        EXPECT_EQ(expected00, powered.get(0, 0));
        EXPECT_EQ(expected01, powered.get(0, 1));
        EXPECT_EQ(expected10, powered.get(1, 0));
        EXPECT_EQ(expected11, powered.get(1, 1));
    }
}

TEST_F(TestMatrixVarPowered, Case1) {
    constexpr size_t Size = 2;
    using Num = int16_t;
    using Mat = MatrixVarBase<Num>;
    constexpr Num a = 3;
    constexpr Num x = 4;
    constexpr Num m = 7;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixVarPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(5, powered.get(0, 1));
}

TEST_F(TestMatrixVarPowered, Case2) {
    constexpr size_t Size = 2;
    using Num = int32_t;
    using Mat = MatrixVarBase<Num>;
    constexpr Num a = 8;
    constexpr Num x = 10;
    constexpr Num m = 9;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixVarPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(0, powered.get(0, 1));
}

TEST_F(TestMatrixVarPowered, Case3) {
    constexpr size_t Size = 2;
    using Num = uint64_t;
    using Mat = MatrixVarBase<Num>;
    constexpr Num a = 1000000000ULL;
    constexpr Num x = 1000000000000ULL;
    constexpr Num m = 998244353;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixVarPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(919667211ULL, powered.get(0, 1));
}

TEST_F(TestMatrixVarPowered, Size) {
    // m <- matrix(1:9, nrow=3, ncol=3)
    // m %*% m %*% m
    constexpr size_t Size = 3;
    using Num = uint64_t;
    using Mat = MatrixVarBase<Num>;
    Mat base = Mat(0, 1000000000, Size);
    base.elements_.at(0).at(0) = 1;
    base.elements_.at(1).at(0) = 2;
    base.elements_.at(2).at(0) = 3;
    base.elements_.at(0).at(1) = 4;
    base.elements_.at(1).at(1) = 5;
    base.elements_.at(2).at(1) = 6;
    base.elements_.at(0).at(2) = 7;
    base.elements_.at(1).at(2) = 8;
    base.elements_.at(2).at(2) = 9;

    MatrixVarPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(2);
    EXPECT_EQ(30ULL, powered.get(0, 0));
    EXPECT_EQ(36ULL, powered.get(1, 0));
    EXPECT_EQ(42ULL, powered.get(2, 0));
    EXPECT_EQ(66ULL, powered.get(0, 1));
    EXPECT_EQ(81ULL, powered.get(1, 1));
    EXPECT_EQ(96ULL, powered.get(2, 1));
    EXPECT_EQ(102ULL, powered.get(0, 2));
    EXPECT_EQ(126ULL, powered.get(1, 2));
    EXPECT_EQ(150ULL, powered.get(2, 2));

    const auto powered3 = mat_powered.power(3);
    EXPECT_EQ(1548ULL, powered3.get(2, 1));
    EXPECT_EQ(2034ULL, powered3.get(1, 2));
}

// -------------------------------------------------------------------
// 2^2^i from power_of_two.cpp
template<typename ModT>
std::vector<ModT> two_to_the_power_of_two(void) {
    std::vector<ModT> powered(32);
    powered.at(0) = 2;
    for(Num i{1}; i<32; ++i) {
        powered.at(i) = powered.at(i-1) * powered.at(i-1);
    }

    return powered;
}

template<typename ModT>
ModT two_to_the_n_power(Num n) {
    const auto powered = two_to_the_power_of_two<ModT>();

    ModT answer = 1;
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

class TestTwoToThePowerOfTwo : public ::testing::Test {};

TEST_F(TestTwoToThePowerOfTwo, TwoToThePowerOfTwo) {
    const auto actual = two_to_the_power_of_two<ModInt1000000007>();
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

TEST_F(TestTwoToThePowerOfTwo, TwoToTheNpower) {
    std::vector<Num> input {1,2,3,4,5,6,7,8,9,15,16,17,31,32,33,63,64,65,127,128};
    std::vector<ModInt1000000007> actual;
    for(const auto& i : input) {
        actual.push_back(two_to_the_n_power<ModInt1000000007>(i));
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

// -------------------------------------------------------------------
// Primes from prime.cpp
void find_primes(Num upper, std::vector<Num>& primes,
                 std::vector<bool>& are_primes, std::vector<Num>& n_primes) {
    primes.clear();
    are_primes.clear();
    n_primes.clear();
    are_primes.resize(upper + 1, true);
    n_primes.resize(upper + 1, 0);

    for(Num i{0}; i<=std::min(upper, 1LL); ++i) {
        are_primes.at(i) = false;
    }

    for(Num p{2}; (p*p)<=upper; ++p) {
        if (!are_primes.at(p)) {
            continue;
        }
        for(Num i=p*2; i<=upper; i+=p) {
            are_primes.at(i) = false;
        }
    }

    for(Num p{2}; p<=upper; ++p) {
        if (are_primes.at(p)) {
            primes.push_back(p);
        }
        n_primes.at(p) += n_primes.at(p-1) + are_primes.at(p);
    }
}

bool is_prime(Num n) {
    if (n < 2) {
        return false;
    }

    auto r = n;
    for(decltype(r) p{2}; (p*p)<=n; ++p) {
        if ((r % p) == 0) {
            r /= p;
        }

        if (r <= 1) {
            break;
        }
    }

    return (r == n);
}

class TestPrimes : public ::testing::Test {};

TEST_F(TestPrimes, Clear) {
    std::vector<Num> primes {10, 20};
    std::vector<bool> are_primes {true, false};
    std::vector<Num> n_primes {7, 8};
    constexpr Num upper = 0;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 1);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_TRUE(n_primes.size() == 1);
    ASSERT_TRUE(n_primes.at(0) == 0);
}

TEST_F(TestPrimes, Primes0) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 0;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 1);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_TRUE(n_primes.size() == 1);
    ASSERT_TRUE(n_primes.at(0) == 0);
}

TEST_F(TestPrimes, Primes1) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 1;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.empty());
    ASSERT_TRUE(are_primes.size() == 2);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_FALSE(are_primes.at(1));

    ASSERT_TRUE(n_primes.size() == 2);
    ASSERT_TRUE(n_primes.at(0) == 0);
    ASSERT_TRUE(n_primes.at(1) == 0);
}

TEST_F(TestPrimes, Primes2) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 2;
    find_primes(upper, primes, are_primes, n_primes);

    ASSERT_TRUE(primes.size() == 1);
    ASSERT_TRUE(primes.at(0) == 2);

    ASSERT_TRUE(are_primes.size() == 3);
    ASSERT_FALSE(are_primes.at(0));
    ASSERT_FALSE(are_primes.at(1));
    ASSERT_TRUE(are_primes.at(2));

    ASSERT_FALSE(is_prime(0));
    ASSERT_FALSE(is_prime(1));
    ASSERT_TRUE(is_prime(2));

    ASSERT_TRUE(n_primes.size() == 3);
    ASSERT_TRUE(n_primes.at(0) == 0);
    ASSERT_TRUE(n_primes.at(1) == 0);
    ASSERT_TRUE(n_primes.at(2) == 1);
}

TEST_F(TestPrimes, PrimesShort) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 37;
    find_primes(upper, primes, are_primes, n_primes);

    const std::vector<Num> expected {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    ASSERT_EQ(expected.size(), primes.size());
    ASSERT_TRUE(std::equal(primes.begin(), primes.end(), expected.begin()));

    Num count {0};
    for(Num i{0}; i<=upper; ++i) {
        const auto found = std::find(expected.begin(), expected.end(), i) != expected.end();
        ASSERT_EQ(found, are_primes.at(i));
        ASSERT_EQ(are_primes.at(i), is_prime(i));

        count += found;
        ASSERT_EQ(count, n_primes.at(i));
    }
}

TEST_F(TestPrimes, PrimesSqrt) {
    std::vector<Num> primes;
    std::vector<bool> are_primes;
    std::vector<Num> n_primes;
    constexpr Num upper = 7 * 7;
    find_primes(upper, primes, are_primes, n_primes);

    const std::vector<Num> expected {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    ASSERT_EQ(expected.size(), primes.size());
    ASSERT_TRUE(std::equal(primes.begin(), primes.end(), expected.begin()));

    Num count {0};
    for(Num i{0}; i<=upper; ++i) {
        const auto found = std::find(expected.begin(), expected.end(), i) != expected.end();
        ASSERT_EQ(found, are_primes.at(i));
        ASSERT_EQ(are_primes.at(i), is_prime(i));

        count += found;
        ASSERT_EQ(count, n_primes.at(i));
    }
}

// -------------------------------------------------------------------
// Rolling hash from rolling_hash.cpp
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

class TestStringHash : public ::testing::Test {};

TEST_F(TestStringHash, Example) {
    constexpr Num base = 256;
    constexpr Num prime = 257;
    const std::string s {"abcABCabc"};
    StringHash shash(s, base, prime);
    ASSERT_EQ(shash.digest(0, 3), shash.digest(6, 9));
}

TEST_F(TestStringHash, Sequence) {
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

TEST_F(TestStringHash, Digits) {
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

TEST_F(TestStringHash, Compare) {
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

TEST_F(TestStringHash, Merge) {
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

// -------------------------------------------------------------------
// rotate and shrink a bitboard from rorate_board.cpp
using BitBoardLine = std::bitset<200>;
using BitBoard = std::array<BitBoardLine, 200>;

void read_bitboard(Num n, std::istream& is, BitBoard& board) {
    for(Num i=0; i<n; ++i) {
        std::string s;
        is >> s;
        for(Num j=0; j<n; ++j) {
            if (s.at(j) == '#') {
                board[i].set(j);
            }
        }
    }
}

void shrink_bitboard(Num n, BitBoard& board) {
    Num top {-1};
    Num bottom {0};
    Num left {1000000000000LL};
    Num right {-1};

    for(Num i=0; i<n; ++i) {
        if (board[i].count() > 0) {
            bottom = i;
            if (top < 0) {
                top = i;
            }
        }

        for(Num j=0; j<200; ++j) {
            if (board[i][j]) {
                left = std::min(left, j);
                break;
            }
        }

        for(Num j=199; j>=0; --j) {
            if (board[i][j]) {
                right = std::max(right, j);
                break;
            }
        }
    }

    BitBoard new_board;
    Num y {0};
    for(Num i=top; i<=bottom; ++i, ++y) {
        Num x {0};
        for(Num j=left; j<=right; ++j, ++x) {
            if (board[i][j]) {
                new_board[y].set(x);
            }
        }
    }

    board = new_board;
    return;
}

void rotate_bitboard(Num n, BitBoard& board) {
    BitBoard new_board;
    for(Num i=0; i<200; ++i) {
        for(Num j=0; j<200; ++j) {
            if (board[i][j]) {
                new_board[j].set(n-1-i);
            }
        }
    }

    board = new_board;
    return;
}

bool is_equal_bitboard(Num n, const BitBoard& lhs, const BitBoard& rhs) {
    for(Num i=0; i<n; ++i) {
        for(Num j=0; j<n; ++j) {
            if (lhs[i][j] != rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

[[maybe_unused]] void sample_rotate_bitboard(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;

    BitBoard board_s;
    BitBoard board_t;
    read_bitboard(n, is, board_s);
    read_bitboard(n, is, board_t);
    shrink_bitboard(n, board_s);
    shrink_bitboard(n, board_t);

    for(int i{0}; i<4; ++i) {
        if (is_equal_bitboard(n, board_s, board_t)) {
            os << "Yes\n";
            return;
        }
        rotate_bitboard(n, board_t);
        shrink_bitboard(n, board_t);
    }

    os << "No\n";
    return;
}

// -------------------------------------------------------------------
// rotate and shrink a number board from rotate_board.cpp
using NumBoard = std::vector<std::vector<Num>>;

NumBoard rotate_board(const NumBoard& input) {
    auto input_height = input.size();
    size_t input_width = 0;
    for(const auto& row : input) {
        input_width = std::max(input_width, row.size());
    }
    auto target_height = input_width;
    auto target_width = input_height;

    std::vector<std::vector<Num>> board(target_height);
    for(decltype(target_height) y{0}; y<target_height; ++y) {
        board.at(y).resize(target_width, 0);
        for(decltype(target_width) x{0}; x<target_width; ++x) {
            const auto& row = input.at(input_height-1-x);
            board.at(y).at(x) = (y < row.size()) ? row.at(y) : 0;
        }
    }

    return board;
}

NumBoard shrink_board(const NumBoard& input) {
    const NumBoard zero;
    auto height = static_cast<Num>(input.size());
    if (height == 0) {
        return zero;
    }

    Num width = 0;
    for(const auto& row : input) {
        width = std::max(width, static_cast<Num>(row.size()));
    }

    Num top {height + 1LL};
    Num bottom {-1};
    Num left {width + 1LL};
    Num right {-1};

    Num count {0};
    for(Num y{0}; y<height; ++y) {
        const auto& row = input.at(y);
        auto it_left = std::find_if(row.begin(), row.end(), [](const auto& e) { return (e > 0); });
        if (it_left == row.end()) {
            continue;
        }

        ++count;
        top = std::min(top, y);
        bottom = y;
        const auto pos = static_cast<Num>(it_left - row.begin());
        left = std::min(left, pos);

        auto max_x = std::min(width, static_cast<Num>(row.size()));
        for(Num x{max_x-1}; x>=left; --x) {
            if (row.at(x) > 0) {
                right = std::max(right, x);
                break;
            }
        }
    }

    if (count == 0) {
        return zero;
    }

    NumBoard board(bottom - top + 1);
    Num y {0};
    for(Num i=top; i<=bottom; ++i, ++y) {
        board.at(y).resize(right - left + 1, 0);
        Num x {0};
        for(Num j=left; j<=right; ++j, ++x) {
            board.at(y).at(x) = input.at(i).at(j);
        }
    }

    return board;
}

bool are_equal_boards(const NumBoard& lhs, const NumBoard& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    auto size = lhs.size();
    for(decltype(size) i{0}; i<size; ++i) {
        if (lhs.at(i).size() != rhs.at(i).size()) {
            return false;
        }

        const auto eq = std::equal(lhs.at(i).begin(), lhs.at(i).end(), rhs.at(i).begin());
        if (!eq) {
            return false;
        }
    }

    return true;
}

class TestNumBoardRotate : public ::testing::Test {};

TEST_F(TestNumBoardRotate, ToRow) {
    NumBoard input {{4, 5, 7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 3);
    for(Num y{0}; y<3; ++y) {
        ASSERT_EQ(input.at(0).at(y), actual.at(y).at(0));
    }
}

TEST_F(TestNumBoardRotate, ToColumn) {
    NumBoard input {{4}, {5}, {7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 1);
    const auto& column = actual.at(0);
    const NumBoard expected {{7, 5, 4}};
    ASSERT_TRUE(std::equal(column.begin(), column.end(), expected.at(0).begin()));
    ASSERT_TRUE(are_equal_boards(expected, actual));
    ASSERT_FALSE(are_equal_boards(expected, input));
}

TEST_F(TestNumBoardRotate, Square) {
    NumBoard input1 {{3}};
    const auto actual1 = rotate_board(input1);
    ASSERT_TRUE(actual1.size() == 1);
    ASSERT_TRUE(actual1.at(0).size() == 1);
    ASSERT_TRUE(actual1.at(0).at(0) == 3);

    NumBoard input2 {{3, 4}, {5, 6}};
    const auto actual2 = rotate_board(input2);
    ASSERT_TRUE(actual2.size() == 2);
    ASSERT_TRUE(actual2.at(0).size() == 2);
    ASSERT_TRUE(actual2.at(1).size() == 2);

    const NumBoard expected2 {{5, 3}, {6, 4}};
    ASSERT_TRUE(std::equal(actual2.at(0).begin(), actual2.at(0).end(), expected2.at(0).begin()));
    ASSERT_TRUE(std::equal(actual2.at(1).begin(), actual2.at(1).end(), expected2.at(1).begin()));
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
    ASSERT_FALSE(are_equal_boards(expected2, input2));
}

TEST_F(TestNumBoardRotate, Rectangular) {
    const NumBoard expected1 {{3, 4, 5}, {6, 7, 8}};
    const NumBoard expected2 {{6, 3}, {7, 4}, {8, 5}};
    const NumBoard expected3 {{8, 7, 6}, {5, 4, 3}};
    const NumBoard expected4 {{5, 8}, {4, 7}, {3, 6}};

    ASSERT_TRUE(are_equal_boards(expected2, rotate_board(expected1)));
    ASSERT_TRUE(are_equal_boards(expected3, rotate_board(expected2)));
    ASSERT_TRUE(are_equal_boards(expected4, rotate_board(expected3)));
    ASSERT_TRUE(are_equal_boards(expected1, rotate_board(expected4)));
}

TEST_F(TestNumBoardRotate, Imbalanced) {
    const NumBoard input1 {{3, 4}, {6, 7, 8}};
    const NumBoard expected1 {{6, 3}, {7, 4}, {8, 0}};
    ASSERT_TRUE(are_equal_boards(expected1, rotate_board(input1)));

    const NumBoard input2 {{3, 4, 5}, {6, 7}};
    const NumBoard expected2 {{6, 3}, {7, 4}, {0, 5}};
    ASSERT_TRUE(are_equal_boards(expected2, rotate_board(input2)));
}

class TestNumBoardShrink : public ::testing::Test {};

TEST_F(TestNumBoardShrink, Empty) {
    const NumBoard input;
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal_boards(input, actual));
}

TEST_F(TestNumBoardShrink, Zero) {
    const NumBoard input {{0}};
    const NumBoard expected;
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal_boards(expected, actual));
}

TEST_F(TestNumBoardShrink, One) {
    const NumBoard input {{1}};
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal_boards(input, actual));
}

TEST_F(TestNumBoardShrink, Zeros) {
    const NumBoard input {{0,0,0}, {0,0,0}, {0,0,0}};
    const auto actual = shrink_board(input);
    const NumBoard expected;
    ASSERT_TRUE(are_equal_boards(expected, actual));
}

TEST_F(TestNumBoardShrink, Center) {
    const NumBoard input {{0,0,0}, {0,1,0}, {0,0,0}};
    const auto actual = shrink_board(input);
    const NumBoard expected {{1}};
    ASSERT_TRUE(are_equal_boards(expected, actual));
}

TEST_F(TestNumBoardShrink, Filled) {
    const NumBoard input {{1,1,1}, {1,1,1}, {1,1,1}};
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal_boards(input, actual));
}

TEST_F(TestNumBoardShrink, Corner) {
    const NumBoard input1 {{1,0,0}, {0,0,0}, {0,0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(input1, actual1));

    const NumBoard input2 {{0,0,1}, {0,0,0}, {1,0,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(input2, actual2));
}

TEST_F(TestNumBoardShrink, LeftTop) {
    const NumBoard input1 {{1,0,0}, {0,1,0}, {0,0,0}};
    const NumBoard expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(expected1, actual1));

    const NumBoard input2 {{0,1,0}, {1,0,0}, {0,0,0}};
    const NumBoard expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
}

TEST_F(TestNumBoardShrink, LeftBottom) {
    const NumBoard input1 {{0,0,0}, {1,0,0}, {0,1,0}};
    const NumBoard expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(expected1, actual1));

    const NumBoard input2 {{0,0,0}, {0,1,0}, {1,0,0}};
    const NumBoard expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
}

TEST_F(TestNumBoardShrink, RightTop) {
    const NumBoard input1 {{0,1,0}, {0,0,1}, {0,0,0}};
    const NumBoard expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(expected1, actual1));

    const NumBoard input2 {{0,0,1}, {0,1,0}, {0,0,0}};
    const NumBoard expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
}

TEST_F(TestNumBoardShrink, RightBottom) {
    const NumBoard input1 {{0,0,0}, {0,1,0}, {0,0,1}};
    const NumBoard expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(expected1, actual1));

    const NumBoard input2 {{0,0,0}, {0,0,1}, {0,1,0}};
    const NumBoard expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
}

TEST_F(TestNumBoardShrink, Imbalanced) {
    const NumBoard input1 {{0}, {0,1,0}, {0,0,1}};
    const NumBoard expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal_boards(expected1, actual1));

    const NumBoard input2 {{0,0,1}, {0,1,0}, {0}};
    const NumBoard expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal_boards(expected2, actual2));
}

// -------------------------------------------------------------------
// round integers from round.cpp
std::pair<Num, Num> ifloor(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = p % q;
        quotient = p / q;
    } else {
        remainder = (q - (std::abs(p) % q)) % q;
        quotient = (p - remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iceil(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = (q - p % q) % q;
        quotient = (p + remainder) / q;
    } else {
        remainder = std::abs(p) % q;
        quotient = (p + remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, -rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iround(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    const Num sign = (p > 0) ? 1 : -1;
    const Num abs_p = std::abs(p);
    const Num quotient = sign * (abs_p / q);
    const Num remainder = sign * (abs_p % q);
    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}

struct TestCase {
    Num divisor {0};
    const std::vector<std::pair<Num, Num>> expected;
};

class TestRountInt : public ::testing::Test {};

TEST_F(TestRountInt, IfloorZeros) {
    const auto [quotient, remainder] = ifloor(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
}

TEST_F(TestRountInt, Ifloor) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-5, 1}, {-4, 0}, {-4, 1}, {-3, 0}, {-3, 1}, {-2, 0}, {-2, 1}, {-1, 0}, {-1, 1},
          {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}, {3, 0}, {3, 1}, {4, 0}, {4, 1}, {5, 0}}},
        {3,
         {{-4, 2}, {-3, 0}, {-3, 1}, {-3, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-1, 0}, {-1, 1}, {-1, 2},
          {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}}},
        {-3,
         {{3, -1}, {3, 0}, {2, -2}, {2, -1}, {2, 0}, {1, -2}, {1, -1}, {1, 0}, {0, -2}, {0, -1},
          {0, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-3, -2}, {-3, -1}, {-3, 0}, {-4, -2}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = ifloor(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((divisor * actual_rem) >= 0);
        }
    }
}

TEST_F(TestRountInt, IceilZeros) {
    const auto [quotient, remainder] = iceil(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
}

TEST_F(TestRountInt, Iceil) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-4, -1}, {-4, 0}, {-3, -1}, {-3, 0}, {-2, -1}, {-2, 0}, {-1, -1}, {-1, 0}, {0, -1},
          {0, 0}, {1, -1}, {1, 0}, {2, -1}, {2, 0}, {3, -1}, {3, 0}, {4, -1}, {4, 0}, {5, -1}, {5, 0}}},
        {3,
         {{-3, -1}, {-3, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {0, -2}, {0, -1},
          {0, 0}, {1, -2}, {1, -1}, {1, 0}, {2, -2}, {2, -1}, {2, 0}, {3, -2}, {3, -1}, {3, 0}, {4, -2}}},
        {-3,
         {{4, 2}, {3, 0}, {3, 1}, {3, 2}, {2, 0}, {2, 1}, {2, 2}, {1, 0}, {1, 1}, {1, 2},
          {0, 0}, {0, 1}, {0, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-3, 0}, {-3, 1}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = iceil(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((divisor * actual_rem) <= 0);
        }
    }
}

TEST_F(TestRountInt, IroundZeros) {
    const auto [quotient, remainder] = iceil(1LL, 0LL);
    ASSERT_EQ(0, quotient);
    ASSERT_EQ(0, remainder);
}

TEST_F(TestRountInt, Iround) {
    constexpr Num width {10};
    constexpr size_t test_size = width * 2 + 1;
    const std::vector<TestCase> test_cases {
        {1,
         {{-10, 0}, {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}}},
        {2,
         {{-5, 0}, {-4, -1}, {-4, 0}, {-3, -1}, {-3, 0}, {-2, -1}, {-2, 0}, {-1, -1}, {-1, 0}, {0, -1},
          {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}, {3, 0}, {3, 1}, {4, 0}, {4, 1}, {5, 0}}},
        {3,
         {{-3, -1}, {-3, 0}, {-2, -2}, {-2, -1}, {-2, 0}, {-1, -2}, {-1, -1}, {-1, 0}, {0, -2}, {0, -1},
          {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}}},
        {-3,
         {{3, -1}, {3, 0}, {2, -2}, {2, -1}, {2, 0}, {1, -2}, {1, -1}, {1, 0}, {0, -2}, {0, -1},
          {0, 0}, {0, 1}, {0, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}, {-3, 0}, {-3, 1}}}
    };

    for(const auto& test_case : test_cases) {
        const auto divisor = test_case.divisor;
        const auto& expected = test_case.expected;
        ASSERT_EQ(test_size, expected.size());

        Num dividend_index {0};
        for(Num dividend{-width}; dividend<=width; ++dividend, ++dividend_index) {
            const auto [actual_quo, actual_rem] = iround(dividend, divisor);
            const auto [expected_quo, expected_rem] = expected.at(dividend_index);
            ASSERT_EQ(expected_quo, actual_quo);
            ASSERT_EQ(expected_rem, actual_rem);
            ASSERT_EQ(dividend, expected_quo * divisor + expected_rem);
            ASSERT_TRUE((expected_quo * dividend * divisor) >= 0);
            ASSERT_TRUE((dividend * actual_rem) >= 0);
        }
    }
}

// -------------------------------------------------------------------
// RLE from run_length.cpp
template <typename T>
struct RunLength {
    struct Run {
        Num pos {0};
        Num len {0};
        T value {0};
        bool operator==(const Run& rhs) const {
            return std::tie(pos, len, value) == std::tie(rhs.pos, rhs.len, rhs.value);
        }
    };

    using Runs = std::vector<Run>;
    Runs runs;

    RunLength(const std::vector<T>& seq) {
        std::optional<Run> current;

        Num pos {0};
        for(const auto& value : seq) {
            const Run run {pos, 1, value};

            if (!current.has_value()) {
                current = run;
            } else {
                if (current.value().value != value) {
                    runs.push_back(current.value());
                    current = run;
                } else {
                    current.value().len += 1;
                }
            }
            ++pos;
        }

        if (current.has_value()) {
            runs.push_back(current.value());
        }
    }
};

std::vector<std::pair<Num,Num>> rle_func(const std::vector<Num>& vs) {
    Num prev = std::numeric_limits<Num>::min();
    Num len {0};
    std::vector<std::pair<Num,Num>> runs;

    for(const auto& v : vs) {
        if (prev != v){
            if (len > 0) {
                runs.emplace_back(prev, len);
            }
            prev = v;
            len = 0;
        }
        ++len;
    }

    runs.emplace_back(prev, len);
    return runs;
}

class TestRunLength : public ::testing::Test {};

TEST_F(TestRunLength, Empty) {
    const std::vector<Num> input;
    const auto actual = RunLength<Num>(input);
    EXPECT_TRUE(actual.runs.empty());
}

TEST_F(TestRunLength, Same) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {3,3,3,3,3};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,5,3}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));

    const auto actual_rle = rle_func(input);
    const std::vector<std::pair<Num,Num>> expected_rle {{3,5}};
    ASSERT_EQ(expected_rle.size(), actual_rle.size());
    ASSERT_TRUE(std::equal(actual_rle.begin(), actual_rle.end(), expected_rle.begin()));
}

TEST_F(TestRunLength, Each) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {2,4,8};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,1,2}, {1,1,4}, {2,1,8}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));

    const auto actual_rle = rle_func(input);
    const std::vector<std::pair<Num,Num>> expected_rle {{2,1},{4,1},{8,1}};
    ASSERT_EQ(expected_rle.size(), actual_rle.size());
    ASSERT_TRUE(std::equal(actual_rle.begin(), actual_rle.end(), expected_rle.begin()));
}

TEST_F(TestRunLength, Groups1) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {40,40,20,20,20,30};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,2,40}, {2,3,20}, {5,1,30}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));

    const auto actual_rle = rle_func(input);
    const std::vector<std::pair<Num,Num>> expected_rle {{40,2},{20,3},{30,1}};
    ASSERT_EQ(expected_rle.size(), actual_rle.size());
    ASSERT_TRUE(std::equal(actual_rle.begin(), actual_rle.end(), expected_rle.begin()));
}

TEST_F(TestRunLength, Groups2) {
    using Rle = RunLength<char>;
    const std::vector<char> input {'a','b','b','b','c'};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,1,'a'}, {1,3,'b'}, {4,1,'c'}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

// -------------------------------------------------------------------
// sqrt(int) from sqrt.cpp
template<typename T>
T isqrt(const T& x) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    auto pred = [&](const auto& thr) {
        return (thr <= x) && ((thr * thr) <= x);
    };

    T upper {1};
    upper <<= (sizeof(T) * 4);
    upper -= 1;
    return *std::ranges::partition_point(std::views::iota(T{0}, upper), pred) - 1;
}

template<typename T, typename U>
T inverse_pow(const T& x, const U& n) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    if (n == 0) {
        return (x == 0) ? 0 : 1;
    }

    if (n == 1) {
        return x;
    }

    if (n == 2) {
        return isqrt(x);
    }

    auto pred = [&](const auto& thr) {
        T prod {1};
        for(U i{0}; i<n; ++i) {
            prod *= thr;
            if (prod > x) {
                return false;
            }
        }

        return true;
    };

    double inv_n {1};
    inv_n /= static_cast<decltype(inv_n)>(n);
    const T center = static_cast<T>(std::floor(std::pow(x, inv_n)));
    const T lower = std::max(T{0}, center - 2);
    const T upper = std::max(lower, center + 3);
    return *std::ranges::partition_point(std::views::iota(lower, upper + 1), pred) - 1;
}

class TestSqrt : public ::testing::Test {};

TEST_F(TestSqrt, Negative) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, isqrt(Int{-1}));
}

TEST_F(TestSqrt, Low) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, isqrt(Int{0}));
    EXPECT_EQ(Int{1}, isqrt(Int{1}));

    for(Int i{2}; i<=102; ++i) {
        const Int actual = isqrt(i);
        Int expected {0};
        for(Int j{0}; j<=i; ++j) {
            if ((j * j) <= i) {
                expected = j;
            } else {
                break;
            }
        }

        EXPECT_EQ(expected, actual);
    }
}

TEST_F(TestSqrt, Int8) {
    using Int = int8_t;
    EXPECT_EQ(Int{10}, isqrt(Int{120}));

    Int i {121};
    for(;;) {
        EXPECT_EQ(Int{11}, isqrt(Int{i}));
        if (i == 127) {
            break;
        }
        ++i;
    }
}

TEST_F(TestSqrt, Uint8) {
    using Int = uint8_t;
    Int ui{121};
    for(; ui<=128; ++ui) {
        EXPECT_EQ(Int{11}, isqrt(Int{ui}));
    }

    ui = 196;
    for(; ui<225; ++ui) {
        EXPECT_EQ(Int{14}, isqrt(Int{ui}));
    }
}

TEST_F(TestSqrt, IntMax) {
    using Int = int64_t;
    Int expected = 0x7ffffffeLL;
    std::vector<Int> inputs {4611686009837453316LL, 4611686014132420608LL};
    for(const auto& input : inputs) {
        EXPECT_EQ(Int{expected}, isqrt(Int{input}));
        EXPECT_TRUE((expected * expected) <= input);
        EXPECT_TRUE(((expected + 1) * (expected + 1)) > input);
    }
}

TEST_F(TestSqrt, UIntMax) {
    using Int = uint64_t;
    Int expected = 0xfffffffeULL;
    std::vector<Int> inputs {18446744056529682436ULL, 18446744065119617024ULL};
    for(const auto& input : inputs) {
        EXPECT_EQ(Int{expected}, isqrt(Int{input}));
        EXPECT_TRUE((expected * expected) <= input);
        EXPECT_TRUE(((expected + 1) * (expected + 1)) > input);
    }
}

class TestInversePow : public ::testing::Test {};

TEST_F(TestInversePow, Negative) {
    using Int = int32_t;
    EXPECT_EQ(Int{0}, inverse_pow(Int{-1}, 0));
}

TEST_F(TestInversePow, Zero) {
    using Int = int64_t;
    EXPECT_EQ(Int{0}, inverse_pow(0, 0));

    for(Int base{1}; base<=100; ++base) {
        EXPECT_EQ(Int{1}, inverse_pow(base, 0));
    }
}

TEST_F(TestInversePow, One) {
    using Int = int64_t;
    for(Int base{0}; base<=100; ++base) {
        EXPECT_EQ(base, inverse_pow(base, 1));
    }
}

TEST_F(TestInversePow, Low) {
    using Int = int64_t;

    for(Int index{2}; index<=10; ++index) {
        for(Int base{0}; base<=10; ++base) {
            Int arg {1};
            for(Int i{0}; i<index; ++i) {
                arg *= base;
            }

            const Int actual = inverse_pow(arg, index);
            EXPECT_EQ(base, actual);
        }
    }
}

TEST_F(TestInversePow, Typical) {
    using Int = int64_t;
    constexpr Int arg = 1000000000000000000LL;
    EXPECT_EQ(Int{1000000000LL}, inverse_pow(arg, 2));
    EXPECT_EQ(Int{1000000LL}, inverse_pow(arg, 3));
    EXPECT_EQ(Int{1000LL}, inverse_pow(arg, 6));
    EXPECT_EQ(Int{100LL}, inverse_pow(arg, 9));
    EXPECT_EQ(Int{10LL}, inverse_pow(arg, 18));
    EXPECT_EQ(Int{31622LL}, inverse_pow(arg, 4));
    EXPECT_EQ(Int{3981LL}, inverse_pow(arg, 5));
}

TEST_F(TestInversePow, Pow3) {
    using Int = int64_t;
    constexpr int n = 3;

    for(Int i{1000000 - 11*100}; i<=(1000000 + 11*100); i+=11) {
        Int arg {1};

        for(int j{0}; j<n; ++j) {
            arg *= i;
        }

        for(Int ofs{-20}; ofs<=20; ++ofs) {
            const Int expected = i - (ofs < 0);
            EXPECT_EQ(expected, inverse_pow(arg + ofs, n));
        }
    }
}

TEST_F(TestInversePow, Pow6) {
    using Int = int64_t;
    constexpr int n = 6;

    for(Int i{1000- 3*100}; i<=(1000 + 3*100); i+=3) {
        Int arg {1};

        for(int j{0}; j<n; ++j) {
            arg *= i;
        }

        for(Int ofs{-20}; ofs<=20; ++ofs) {
            const Int expected = i - (ofs < 0);
            EXPECT_EQ(expected, inverse_pow(arg + ofs, n));
        }
    }
}

// -------------------------------------------------------------------
// A subsequence not necessarily contiguous from subsequence.cpp
size_t find_subsequence(const std::string& target, const std::string& sub) {
    size_t n_matched {0};
    size_t target_size = target.size();
    size_t sub_size = sub.size();

    for(size_t cursor{0}; (cursor < target_size) && (n_matched < sub_size); ++cursor) {
        if (target.at(cursor) == sub.at(n_matched)) {
            ++n_matched;
        }
    }

    return n_matched;
}

class TestSubsequence : public ::testing::Test {};

const std::vector<std::string> test_cases {
    "abc", "a", "aa", "aaa", "aab", "abb"
};

TEST_F(TestSubsequence, Exact) {
    for(const auto& s : test_cases) {
        const auto actual = find_subsequence(s, s);
        ASSERT_EQ(s.size(), actual);
    }
}

TEST_F(TestSubsequence, Symmetric) {
    for(const auto& s : test_cases) {
        const auto size = s.size();
        for(size_t len {1}; (len + 1) < size; ++len) {
            const auto sub = s.substr(0, len);

            const auto actual = find_subsequence(s, sub);
            ASSERT_EQ(len, actual);

            const auto rev = find_subsequence(sub, s);
            ASSERT_EQ(len, rev);
        }
    }
}

TEST_F(TestSubsequence, SubSeq) {
    const std::string target = "abbcccddddeeeccca";
    const std::vector<std::string> full_cases {
        "aa", "abcdeca", "ace", "bda", "cec"
    };

    for(const auto& s : full_cases) {
        const auto actual = find_subsequence(target, s);
        ASSERT_EQ(s.size(), actual);
    }

    const std::vector<std::pair<std::string, size_t>> sub_cases {
        {"aaa", 2}, {"abcxeca", 3}, {"dddddd", 4}, {"xyz", 0}
    };

    for(const auto& [s, expected] : sub_cases) {
        const auto actual = find_subsequence(target, s);
        ASSERT_EQ(expected, actual);
    }
}

// -------------------------------------------------------------------
// ternary search from ternary_search.cpp and ternary_search_int.cpp
namespace {
    using FuncNumeric = Numeric(&)(Numeric);
}

Numeric ternary_search_numeric(Numeric lower, Numeric upper, FuncNumeric func) {
    constexpr Numeric tolerance = 1e-15;
    Numeric left {lower};
    Numeric right {upper};

    while(std::abs(left - right) > tolerance) {
        Numeric relative = std::max(std::abs(left), std::abs(right));
        relative = std::abs(left - right) / std::max(relative, tolerance);
        if (relative < tolerance) {
            break;
        }

        Numeric midleft = (left * 2.0 + right) / 3.0;
        Numeric midright = (left + right * 2.0) / 3.0;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    return left;
}

namespace {
    constexpr Numeric x_origin = 1.5;
    constexpr Numeric y_origin = -2.5;
    constexpr Numeric test_tolerance = 1e-6;

    Numeric func1(Numeric x) {
        const auto w = x - x_origin;
        return w * w + y_origin;
    }

    Numeric func0(Numeric x) {
        return x * x;
    }
}

class TestTernarySearchNumeric : public ::testing::Test {};

TEST_F(TestTernarySearchNumeric, Origin) {
    constexpr auto lower = x_origin;
    constexpr auto upper = x_origin;
    const auto actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);
    EXPECT_NEAR(y_origin, func1(actual), test_tolerance);
}

TEST_F(TestTernarySearchNumeric, Min) {
    Numeric lower = x_origin - 2.0;
    Numeric upper = x_origin + 2.0;
    auto actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);

    lower = x_origin - 10.0;
    actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);

    upper = x_origin + 20.0;
    actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(x_origin, actual, test_tolerance);
}

TEST_F(TestTernarySearchNumeric, MonotonicLeft) {
    constexpr auto lower = x_origin - 2.0;
    auto upper = x_origin - 1.0;
    auto actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(upper, actual, test_tolerance);

    upper = x_origin;
    actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(upper, actual, test_tolerance);
}

TEST_F(TestTernarySearchNumeric, MonotonicRight) {
    auto lower = x_origin + 1.0;
    constexpr auto upper = x_origin + 2.0;
    auto actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(lower, actual, test_tolerance);

    lower = x_origin;
    actual = ternary_search_numeric(lower, upper, func1);
    EXPECT_NEAR(lower, actual, test_tolerance);
}

TEST_F(TestTernarySearchNumeric, Zero) {
    constexpr auto zero = 0.0;
    auto actual = ternary_search_numeric(zero, zero, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);
    EXPECT_NEAR(zero, func0(actual), test_tolerance);

    actual = ternary_search_numeric(-1.0, 1.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search_numeric(-2.0, 3.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search_numeric(-2.0, zero, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);

    actual = ternary_search_numeric(zero, 2.0, func0);
    EXPECT_NEAR(zero, actual, test_tolerance);
}

namespace {
    using FuncResultInt = long long int;
    using FuncInt = FuncResultInt(&)(Num);
    using TernarySearchResultInt = std::pair<FuncResultInt,Num>;
}

TernarySearchResultInt ternary_search_int(Num lower, Num upper, FuncInt func) {
    Num left {lower};
    Num right {upper};

    while(std::abs(left - right) > 3) {
        const Num midleft = (left * 2 + right) / 3;
        const Num midright = (left + right * 2) / 3;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    std::vector<TernarySearchResultInt> ps;
    for(Num i{left}; i<=right; ++i) {
        ps.emplace_back(func(i), i);
    }

    return *std::ranges::min_element(ps);
}

namespace {
    constexpr Num Xoffset = 567;

    const std::vector<std::pair<Num,Num>> test_case_ranges {
        {-1000000000000000LL, -500000000000000LL},
        {-1000000000000000LL, 500000000000000LL},
        {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {-1, 0}, {0, 0}, {0, 1}
    };

    Num func_int_increase(Num x) {
        return x * 2;
    }

    Num func_int_decrease(Num x) {
        return -x * 3;
    }

    Num func_int_flat(Num x) {
        return -45678;
    }

    Num func_int_hull(Num x) {
        return std::abs(x - Xoffset);
    }
}

class TestTernarySearchInt : public ::testing::Test {};

TEST_F(TestTernarySearchInt, Increase) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search_int(lower, upper, func_int_increase);
        EXPECT_EQ(lower * 2, value);
        EXPECT_EQ(lower, arg);
    }
}

TEST_F(TestTernarySearchInt, Decrease) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search_int(lower, upper, func_int_decrease);
        EXPECT_EQ(-upper * 3, value);
        EXPECT_EQ(upper, arg);
    }
}

TEST_F(TestTernarySearchInt, Flat) {
    for(const auto& [lower, upper] : test_case_ranges) {
        const auto [value, arg] = ternary_search_int(lower, upper, func_int_flat);
        EXPECT_EQ(-45678, value);
        EXPECT_EQ(lower, arg);
    }
}

TEST_F(TestTernarySearchInt, Hull) {
    const std::vector<std::tuple<Num,Num,Num,Num>> test_case {
        {Xoffset - 2, Xoffset + 3, 0, Xoffset},
        {Xoffset - 2, Xoffset + 2, 0, Xoffset},
        {Xoffset - 2, Xoffset + 1, 0, Xoffset},
        {Xoffset - 2, Xoffset, 0, Xoffset},
        {Xoffset - 3, Xoffset, 0, Xoffset},
        {Xoffset - 4, Xoffset, 0, Xoffset},
        {Xoffset - 1, Xoffset - 1, 1, Xoffset - 1},
        {Xoffset - 1, Xoffset, 0, Xoffset},
        {Xoffset - 1, Xoffset + 1, 0, Xoffset},
        {Xoffset + 1, Xoffset + 1, 1, Xoffset + 1},
        {Xoffset, Xoffset, 0, Xoffset},
        {Xoffset, Xoffset + 1, 0, Xoffset},
        {Xoffset, Xoffset + 2, 0, Xoffset},
        {Xoffset, Xoffset + 3, 0, Xoffset},
    };

    for(const auto& [lower, upper, expected_value, expected_arg] : test_case) {
        const auto [value, arg] = ternary_search_int(lower, upper, func_int_hull);
        EXPECT_EQ(expected_value, value);
        EXPECT_EQ(expected_arg, arg);
    }
}

// -------------------------------------------------------------------
// Topological sort from topological_sort.cpp
bool topological_sort(Num n, const Edges& graph, const Vec& in_degrees, Vec& answer) {
    auto updated_in_degrees = in_degrees;
    std::priority_queue<Num, std::vector<Num>, std::greater<Num>> candidates;

    for(decltype(n) i{0}; i<n; ++i) {
        if (updated_in_degrees.at(i) == 0) {
            candidates.push(i);
        }
    }

    Vec sorted_nodes;
    bool unique {true};

    while(!candidates.empty()) {
        unique &= (candidates.size() == 1);
        const auto current = candidates.top();
        candidates.pop();
        sorted_nodes.push_back(current);

        for(const auto& next : graph[current]) {
            updated_in_degrees.at(next) -= 1;
            if (updated_in_degrees.at(next) <= 0) {
                candidates.push(next);
            }
        }
    }

    std::swap(answer, sorted_nodes);
    return unique && (static_cast<Num>(answer.size()) == n);
}

class TestTopologicalSort : public ::testing::Test {};

TEST_F(TestTopologicalSort, TreeForward) {
    constexpr Num n = 7;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{0, 5}, {0, 3}, {0, 4}, {3, 2}, {4, 6}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Vec actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {0,1,3,2,4,5,6};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestTopologicalSort, TreeReverse) {
    constexpr Num n = 7;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{6, 1}, {6, 3}, {6, 2}, {3, 4}, {2, 0}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Vec actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {5,6,1,2,0,3,4};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestTopologicalSort, PathGraph) {
    constexpr Num n = 5;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{4, 3}, {3, 2}, {2, 1}, {1, 0}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Vec actual;
    EXPECT_TRUE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {4,3,2,1,0};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestTopologicalSort, Merge) {
    constexpr Num n = 5;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{4, 1}, {1, 2}, {1, 0}, {2, 3}, {0, 3}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Vec actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {4,1,0,2,3};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestTopologicalSort, NotConnected) {
    constexpr Num n = 2;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges;
    Vec actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {0,1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestTopologicalSort, Loop) {
    constexpr Num n = 4;
    Edges graph(n);
    Vec in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{0, 1}, {1, 2}, {2, 3}, {3, 1}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Vec actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Vec expected {0};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

// -------------------------------------------------------------------
// Sort tournament from tournament.cpp
namespace {
    using TournamentMatches = std::vector<std::pair<Num, Num>>;
    using TournamentChildren = std::vector<std::pair<Num, Num>>;
    using TournamentLeaves = std::vector<Num>;
    using TournamentLeafIndexes = std::vector<Num>;
}

void visit_dfs(const TournamentChildren& children, Num current, TournamentLeaves& leaves) {
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

TournamentLeaves sort_tournament(Num n, const TournamentMatches& matches) {
    atcoder::dsu tree(static_cast<DsuNode>(n+1));
    for(decltype(n) i{1}; i<=n; ++i) {
        tree.merge(static_cast<DsuNode>(i), static_cast<DsuNode>(i));
    }

    std::vector<Num> parents(n*2, 0);
    std::vector<Num> top_nodes(n*2, 0);
    for(decltype(n) i{1}; i<=n; ++i) {
        parents.at(i) = i;
        top_nodes.at(i) = i;
    }

    TournamentChildren children(n*2);
    Num match_node = n;
    for(const auto& [p,q] : matches) {
        ++match_node;
        const Num node_p = top_nodes.at(tree.leader(static_cast<DsuNode>(p)));
        const Num node_q = top_nodes.at(tree.leader(static_cast<DsuNode>(q)));

        parents.at(node_p) = match_node;
        parents.at(node_q) = match_node;
        tree.merge(static_cast<DsuNode>(p), static_cast<DsuNode>(q));
        top_nodes.at(tree.leader(static_cast<DsuNode>(p))) = match_node;
        children.at(match_node) = std::make_pair(node_p, node_q);
    }

    TournamentLeaves leaves;
    visit_dfs(children, n*2-1, leaves);
    return leaves;
}

TournamentLeafIndexes leaf_to_index(const TournamentLeaves& leaves, Num n) {
    TournamentLeafIndexes indexes(n+1, -1);
    for(Num i{0}; i<n; ++i) {
        indexes.at(leaves.at(i)) = i;
    }
    return indexes;
}

class TestTournament : public ::testing::Test {};

TEST_F(TestTournament, Sample1) {
    const TournamentMatches matches {{4, 2}, {4, 3}, {2, 1}};
    const TournamentLeaves expected {4, 2, 3, 1};
    const Num n = static_cast<Num>(expected.size());
    const auto actual = sort_tournament(n, matches);

    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));

    const auto indexes = leaf_to_index(actual, n);
    const TournamentLeaves expected_index {-1, 3, 1, 2, 0};
    ASSERT_EQ(expected_index.size(), indexes.size());
    ASSERT_TRUE(std::equal(expected_index.begin(), expected_index.end(),
                           indexes.begin(), indexes.end()));
}

TEST_F(TestTournament, Sample2) {
    const TournamentMatches matches {{2, 3}, {4, 5}, {3, 6}, {4, 1}, {2, 1}};
    const TournamentLeaves expected {2, 3, 6, 4, 5, 1};
    const Num n = static_cast<Num>(expected.size());
    const auto actual = sort_tournament(n, matches);

    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end()));

    const auto indexes = leaf_to_index(actual, n);
    const TournamentLeaves expected_index {-1, 5, 0, 1, 3, 4, 2};
    ASSERT_EQ(expected_index.size(), indexes.size());
    ASSERT_TRUE(std::equal(expected_index.begin(), expected_index.end(),
                           indexes.begin(), indexes.end()));
}

// -------------------------------------------------------------------
// Trie from trie_tree.cpp
struct TrieTreeNode {
    char chr {'*'};
    std::string value;
    size_t multiplicity {0};
    std::map<char, std::unique_ptr<TrieTreeNode>> children;

    TrieTreeNode() = default;

    void add(const std::string& s, size_t pos) {
        if (s.size() == pos) {
            value = s;
            ++multiplicity;
            return;
        }

        const auto& c = s.at(pos);
        if (!children.contains(c)) {
            children[c] = std::make_unique<TrieTreeNode>();
            children[c]->chr = c;
        }
        children[c]->add(s, pos + 1);
    }

    void add(const std::string& s) {
        add(s, 0);
    }

    void print_dfs(std::ostream& os) const {
        os << "(" << chr << ":" << value << ":" << multiplicity << ")\n";

        std::vector<std::pair<char, const TrieTreeNode*>> ptrs;
        for(const auto& [c, p] : children) {
            ptrs.push_back(std::make_pair(c, p.get()));
        }
        std::ranges::sort(ptrs);

        for(const auto& child : ptrs) {
            child.second->print_dfs(os);
        }
    }
};

class TestTrie : public ::testing::Test {};

TEST_F(TestTrie, Isolate) {
    TrieTreeNode top;
    {
        std::string expected {"(*::0)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("d");
    {
        std::string expected {"(*::0)\n(d:d:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("ef");
    {
        std::string expected {"(*::0)\n(d:d:1)\n(e::0)\n(f:ef:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("aba");
    {
        std::string expected {"(*::0)\n(a::0)\n(b::0)\n(a:aba:1)\n(d:d:1)\n(e::0)\n(f:ef:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestTrie, Overlap) {
    TrieTreeNode top;
    top.add("ef");
    top.add("efg");
    {
        std::string expected {"(*::0)\n(e::0)\n(f:ef:1)\n(g:efg:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("e");
    {
        std::string expected {"(*::0)\n(e:e:1)\n(f:ef:1)\n(g:efg:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestTrie, Branch) {
    TrieTreeNode top;
    top.add("efg");
    top.add("efh");
    {
        std::string expected {"(*::0)\n(e::0)\n(f::0)\n(g:efg:1)\n(h:efh:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestTrie, Duplicate) {
    TrieTreeNode top;
    top.add("efg");
    top.add("e");
    top.add("efg");
    {
        std::string expected {"(*::0)\n(e:e:1)\n(f::0)\n(g:efg:2)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("e");
    {
        std::string expected {"(*::0)\n(e:e:2)\n(f::0)\n(g:efg:2)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

// use atcoder::dsu instead of union_find_tree.cpp
// -------------------------------------------------------------------
// Zigzag scan from zigzag.cpp
[[maybe_unused]] void zigzag_scan(std::istream& is, std::ostream& os) {
    using Matrix = boost::multi_array<ModInt, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;

    Num x {0};
    Num y {0};
    is >> x >> y;

    MatrixShape shape {{4, 1000009}};
    Matrix board(shape);
    ModInt zero;
    std::fill_n(board.data(), board.num_elements(), zero);
    board[0][0] = 1;

    Num line {0};
    for(Num s{0}; s<(x+y); ++s) {
        Num y_pos = std::max(0LL, s - x);
        Num x_pos = std::min(x, s);
        for(;;) {
            if ((y_pos > y) || (x_pos < 0)) {
                break;
            }
            const auto line1 = (line + 1) % 4;
            const auto line2 = (line + 2) % 4;
            board[line2][x_pos+1] += board[line][x_pos];
            board[line1][x_pos+2] += board[line][x_pos];
            ++y_pos;
            --x_pos;
        }
        line = (line + 1) % 4;
    }

    line = (x + y + 1) % 4;
    os << board[line][x].val() << "\n";
    return;
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// -------------------------------------------------------------------
// LCA from lca.cpp
// Caution: this LCA library uses 1-based indexing
namespace {
    struct LcaEdge {
        Num from {0};
        Num to {0};
        Num weight {0};
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
        LcaNode node {0, 0, Inf};
        return node;
    }

    using LcaEdges = std::unordered_map<Num, std::vector<LcaEdge>>;
    using Path = std::vector<Num>;
    using SegmentTreeLcaNode = atcoder::segtree<LcaNode, apply_lca_node, unit_lca_node>;
    using SegmentTreeLcaEdge = atcoder::segtree<Num, apply_lca_edge, unit_lca_edge>;
    using NodeToLcaIndex = std::vector<std::optional<Num>>;
    using EdgeIndexes = std::unordered_map<Num, std::unordered_map<Num, Num>>;
    using EdgeDirection = std::unordered_map<Num, std::unordered_map<Num, Num>>;

    struct LcaSet {
        const LcaEdges& edges;
        Num min_node_id {0};
        Num max_node_id {0};
        NodeToLcaIndex node_to_lca_index;
        std::unique_ptr<SegmentTreeLcaNode> segtree_lca_nodes;
        std::unique_ptr<SegmentTreeLcaEdge> segtree_lca_edges;
        EdgeIndexes edge_indexes;
        EdgeDirection edge_direction;
        Num sequence_node {0};
        Num sequence_edge {0};

        LcaSet(const LcaEdges& in_edges, Num n) : edges(in_edges) {
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

void eular_tour(Num current, Num prev, const LcaEdges& edges, Path& path) {
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
        segtree_lca_nodes->set(static_cast<DsuNode>(sequence_node), next_node);

        ++sequence_edge;
        segtree_lca_edges->set(
            static_cast<DsuNode>(sequence_edge),
            edge.weight);
        visit_dfs(next, next_depth);

        ++sequence_node;
        const LcaNode current_node{current, sequence_node, depth};
        segtree_lca_nodes->set(
            static_cast<DsuNode>(sequence_node),
            current_node);
        ++sequence_edge;
        segtree_lca_edges->set(
            static_cast<DsuNode>(sequence_edge),
            -edge.weight);
    }

    return;
}

Num LcaSet::find_lca(Num root, Num depth) {
    ++sequence_node;
    const LcaNode root_node{root, sequence_node, 1};
    segtree_lca_nodes->set(static_cast<DsuNode>(sequence_node), root_node);

    visit_dfs(root, depth);

    const Num lca_size = sequence_node;
    Num prev = segtree_lca_nodes->prod(1, 2).id;
    typename EdgeIndexes::mapped_type zero_index;
    typename EdgeDirection::mapped_type zero_direction;

    for(Num i{2}; i<=lca_size; ++i) {
        Num current = segtree_lca_nodes->prod(
            static_cast<DsuNode>(i),
            static_cast<DsuNode>(i+1)).id;
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
    const auto lca = (u <= v) ? segtree_lca_nodes->prod(
        static_cast<DsuNode>(u),
        static_cast<DsuNode>(v+1)) :
        segtree_lca_nodes->prod(
            static_cast<DsuNode>(v),
            static_cast<DsuNode>(u+1));

    const auto lca_index = node_to_lca_index.at(lca.id).value();
    Num dist{0};
    if (x == y) {
        dist = 0;
    } else if (lca.id == x) {
        dist = segtree_lca_edges->prod(
            static_cast<DsuNode>(lca_index),
            static_cast<DsuNode>(v));
    } else if (lca.id == y) {
        dist = segtree_lca_edges->prod(
            static_cast<DsuNode>(lca_index),
            static_cast<DsuNode>(u));
    } else {
        dist = segtree_lca_edges->prod(
            static_cast<DsuNode>(lca_index),
            static_cast<DsuNode>(v));
        dist += segtree_lca_edges->prod(
            static_cast<DsuNode>(lca_index),
            static_cast<DsuNode>(u));
    }
    return dist;
}

class TestLca : public ::testing::Test {};

TEST_F(TestLca, All) {
    const std::vector<LcaEdge> test_case {
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

    LcaEdges edges;
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
        const auto actual = lca_set.segtree_lca_nodes->prod(
            static_cast<DsuNode>(i),
            static_cast<DsuNode>(i+1));
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
        const auto actual = lca_set.segtree_lca_nodes->prod(
            static_cast<DsuNode>(left),
            static_cast<DsuNode>(right+1)).id;
        ASSERT_EQ(q.lca, actual);
    }

    EXPECT_EQ(0,   lca_set.calc_distance(1, 1));
    EXPECT_EQ(300, lca_set.calc_distance(1, 2));
    EXPECT_EQ(300, lca_set.calc_distance(2, 1));
    EXPECT_EQ(700, lca_set.calc_distance(2, 3));
    EXPECT_EQ(700, lca_set.calc_distance(3, 2));
    EXPECT_EQ(40,  lca_set.calc_distance(2, 5));
    EXPECT_EQ(40,  lca_set.calc_distance(5, 2));
    EXPECT_EQ(46,  lca_set.calc_distance(2, 9));
    EXPECT_EQ(46,  lca_set.calc_distance(9, 2));
}

TEST_F(TestLca, Root) {
    const std::vector<LcaEdge> test_case {{3,2,100}, {2,1,30}, {2,4,40}, {3,5,200}};
    const auto n_edges = test_case.size();
    const auto n_nodes = n_edges + 1;
    const auto n_lca_nodes = n_edges * 2 + 1;

    LcaEdges edges;
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
        const auto actual = lca_set.segtree_lca_nodes->prod(
            static_cast<DsuNode>(i),
            static_cast<DsuNode>(i+1));
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
    EXPECT_EQ(70,  lca_set.calc_distance(1, 4));
    EXPECT_EQ(330, lca_set.calc_distance(1, 5));

    EXPECT_EQ(30,  lca_set.calc_distance(2, 1));
    EXPECT_EQ(0,   lca_set.calc_distance(2, 2));
    EXPECT_EQ(100, lca_set.calc_distance(2, 3));
    EXPECT_EQ(40,  lca_set.calc_distance(2, 4));
    EXPECT_EQ(300, lca_set.calc_distance(2, 5));

    EXPECT_EQ(130, lca_set.calc_distance(3, 1));
    EXPECT_EQ(100, lca_set.calc_distance(3, 2));
    EXPECT_EQ(0,   lca_set.calc_distance(3, 3));
    EXPECT_EQ(140, lca_set.calc_distance(3, 4));
    EXPECT_EQ(200, lca_set.calc_distance(3, 5));

    EXPECT_EQ(70,  lca_set.calc_distance(4, 1));
    EXPECT_EQ(40,  lca_set.calc_distance(4, 2));
    EXPECT_EQ(140, lca_set.calc_distance(4, 3));
    EXPECT_EQ(0,   lca_set.calc_distance(4, 4));
    EXPECT_EQ(340, lca_set.calc_distance(4, 5));

    EXPECT_EQ(330, lca_set.calc_distance(5, 1));
    EXPECT_EQ(300, lca_set.calc_distance(5, 2));
    EXPECT_EQ(200, lca_set.calc_distance(5, 3));
    EXPECT_EQ(340, lca_set.calc_distance(5, 4));
    EXPECT_EQ(0,   lca_set.calc_distance(5, 5));
}

// -------------------------------------------------------------------
// find p/q that equals to a given ModInt from find_rational.cpp
void find_rationale_interactive(std::istream& is, std::ostream& os) {
    constexpr Num mod = 998244353;
    const Num upper = std::floor(std::pow(mod,0.3));

    Num n {0};
    is >> n;

    std::set<std::tuple<Num,Num,Num>> ss;
    for(Num bottom{2}; bottom < upper; ++bottom) {
        for(Num top{-upper}; top < upper; ++top) {
            ModInt m = top;
            m /= bottom;
            if (m.val() != n) {
                continue;
            }

            const Num g = std::gcd(std::abs(bottom), std::abs(top));
            Num sum = std::abs(bottom) + std::abs(top);
            sum /= g;
            const auto b = bottom / g;
            const auto t = top / g;
            ss.insert(std::make_tuple(sum, b, t));
        }
    }

    Num rem = 10;
    for(const auto& [sum, bottom, top] : ss) {
        os << top << "/" << bottom << "\n";
        if (--rem <= 0) {
            break;
        }
    }
}

std::vector<Rational> find_rationale(Num n) {
    constexpr Num mod = 998244353;
    const Num upper = std::floor(std::pow(mod,0.3));

    std::set<std::tuple<Num,Num,Num>> ss;
    for(Num bottom{2}; bottom < upper; ++bottom) {
        for(Num top{-upper}; top < upper; ++top) {
            ModInt m = top;
            m /= bottom;
            if (m.val() != n) {
                continue;
            }

            const Num g = std::gcd(std::abs(bottom), std::abs(top));
            Num sum = std::abs(bottom) + std::abs(top);
            sum /= g;
            const auto b = bottom / g;
            const auto t = top / g;
            ss.insert(std::make_tuple(sum, b, t));
        }
    }

    std::vector<Rational> rs;
    for(const auto& [sum, bottom, top] : ss) {
        Rational r {top};
        r /= bottom;
        rs.push_back(r);
    }

    std::sort(rs.begin(), rs.end());
    auto it = std::unique(rs.begin(), rs.end());
    rs.erase(it, rs.end());
    return rs;
}

class TestFindRationale : public ::testing::Test {};

TEST_F(TestFindRationale, All) {
    const Vec ps {0,1,3};
    const Vec qs {1,7,11};
    for(const auto& p : ps) {
        for(const auto& q : qs) {
            ModInt r {p};
            r /= q;
            const auto actual_vec = find_rationale(r.val());
            ASSERT_EQ(1, actual_vec.size());

            Rational expected {p};
            expected /= q;
            ASSERT_EQ(expected, actual_vec.at(0));
        }
    }
}

/*
Local Variables:
mode: c++
coding: utf-8-dos
tab-width: nil
c-file-style: "stroustrup"
End:
*/
