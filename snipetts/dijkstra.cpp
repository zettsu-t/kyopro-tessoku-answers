#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

using Num = long long int;
constexpr Num Inf = std::numeric_limits<Num>::max() / 2;

struct Edge {
    Num to {0};
    Num distance {0};
};
using Edges = std::vector<std::vector<Edge>>;
using Distances = std::vector<Num>;

void dijkstra_algo(Num n_nodes, Num start, const Edges& edges, Distances& distances) {
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

struct Diameter {
    std::vector<Num> nodes;
    Num distance {0};
};

Diameter tree_diameter(Num n_nodes, const Edges& edges) {
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, DijkstraAlgo) {
    const Edges edges {{}, {{2, 10}, {4, 40}}, {{1, 6}, {4, 20}}, {}, {{1, 5}, {2, 3}}};
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

TEST_F(TestAll, TreeDiameter1) {
    const Edges edges {
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

TEST_F(TestAll, TreeDiameter2) {
    const Edges edges {
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
