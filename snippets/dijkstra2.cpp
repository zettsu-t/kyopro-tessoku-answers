#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;

    template<typename T>
    using PQ = std::priority_queue<T, std::vector<T>, std::greater<T>>;

    using Edges = std::vector<std::vector<std::pair<Num,Num>>>;
    using Distances = std::vector<Num>;
    constexpr Num inf = std::numeric_limits<Num>::max() / 2;
}

Distances dijkstra_algo(Num n_nodes, Num start, const Edges& edges) {
    Distances distances(n_nodes, inf);
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

Distances dijkstra_loop(Num n_nodes, Num origin, Num to_init, Num dist_init, const Edges& edges) {
    Distances distances(n_nodes, inf);
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Directed) {
    constexpr Num n_nodes = 5;
    const Edges edges {{}, {{2, 10}, {4, 40}}, {{1, 6}, {4, 20}}, {}, {{1, 5}, {2, 3}}};

    Distances distances1 = dijkstra_algo(n_nodes, 1, edges);
    const Distances expected1 {inf, 0, 10, inf, 30};
    ASSERT_EQ(expected1.size(), distances1.size());
    ASSERT_TRUE(std::equal(expected1.begin(), expected1.end(), distances1.begin()));

    Distances distances2 = dijkstra_algo(n_nodes, 2, edges);
    const Distances expected2 {inf, 6, 0, inf, 20};
    ASSERT_EQ(expected2.size(), distances2.size());
    ASSERT_TRUE(std::equal(expected2.begin(), expected2.end(), distances2.begin()));

    Distances distances3 = dijkstra_algo(n_nodes, 3, edges);
    const Distances expected3 {inf, inf, inf, 0, inf};
    ASSERT_EQ(expected3.size(), distances3.size());
    ASSERT_TRUE(std::equal(expected3.begin(), expected3.end(), distances3.begin()));

    Distances distances4 = dijkstra_algo(n_nodes, 4, edges);
    const Distances expected4 {inf, 5, 3, inf, 0};
    ASSERT_EQ(expected4.size(), distances4.size());
    ASSERT_TRUE(std::equal(expected4.begin(), expected4.end(), distances4.begin()));
}

TEST_F(TestAll, Loop) {
    constexpr Num n_nodes = 6;
    const Edges edges {{{1, 3}}, {{0, 5}}, {{0, 1}, {3, 11}}, {{4, 22}}, {{2, 33}}, {{1, 1}}};
    const std::vector<std::vector<Num>> expected {{8}, {8}, {inf, 66}, {66}, {66}, {inf}};

    for(Num x{0}; x<n_nodes; ++x) {
        for(size_t i{0}; i<expected.at(x).size(); ++i) {
            const auto [y, c] = edges.at(x).at(i);
            const auto actual = dijkstra_loop(n_nodes, x, y, c, edges);
            EXPECT_EQ(expected.at(x).at(i), actual.at(x));
        }
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
