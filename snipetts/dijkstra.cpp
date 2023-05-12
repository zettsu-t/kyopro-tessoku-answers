#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

using Num = long long int;
constexpr Num inf = std::numeric_limits<Num>::max() / 2;

struct Edge {
    Num to {0};
    Num distance {0};
};
using Edges = std::vector<std::vector<Edge>>;
using Distances = std::vector<Num>;

void dijkstra_algo(Num n_nodes, Num start, const Edges& edges, Distances& distances) {
    Distances zeros(n_nodes+1, inf);
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    Num n = 4;
    const Edges edges {{}, {{2, 10}, {4, 40}}, {{1, 6}, {4, 20}}, {}, {{1, 5}, {2, 3}}};

    Distances distances1;
    dijkstra_algo(n, 1, edges, distances1);
    const Distances expected1 {inf, 0, 10, inf, 30};
    ASSERT_EQ(expected1.size(), distances1.size());
    ASSERT_TRUE(std::equal(expected1.begin(), expected1.end(), distances1.begin()));

    Distances distances2;
    dijkstra_algo(n, 2, edges, distances2);
    const Distances expected2 {inf, 6, 0, inf, 20};
    ASSERT_EQ(expected2.size(), distances2.size());
    ASSERT_TRUE(std::equal(expected2.begin(), expected2.end(), distances2.begin()));

    Distances distances3;
    dijkstra_algo(n, 3, edges, distances3);
    const Distances expected3 {inf, inf, inf, 0, inf};
    ASSERT_EQ(expected3.size(), distances3.size());
    ASSERT_TRUE(std::equal(expected3.begin(), expected3.end(), distances3.begin()));

    Distances distances4;
    dijkstra_algo(n, 4, edges, distances4);
    const Distances expected4 {inf, 5, 3, inf, 0};
    ASSERT_EQ(expected4.size(), distances4.size());
    ASSERT_TRUE(std::equal(expected4.begin(), expected4.end(), distances4.begin()));
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
