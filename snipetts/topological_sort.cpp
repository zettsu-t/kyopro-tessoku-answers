#include <iostream>
#include <queue>
#include <vector>
#include <gtest/gtest.h>

using Num = long long int;
using Nodes = std::vector<Num>;
using Degrees = std::vector<Num>;
using Graph = std::vector<Nodes>;

void topological_sort(Num n, const Graph& graph, const Degrees& in_degrees, Nodes& answer) {
    auto updated_in_degrees = in_degrees;
    std::priority_queue<Num, std::vector<Num>, std::greater<Num>> candidates;

    for(decltype(n) i{1}; i<=n; ++i) {
        if (updated_in_degrees.at(i) == 0) {
            candidates.push(i);
        }
    }

    Nodes sorted_nodes;
    while(!candidates.empty()) {
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
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    constexpr Num n = 7;
    Graph graph(n+1);
    Degrees in_degrees(n+1, 0);

    std::vector<std::pair<Num, Num>> edges {{1, 6}, {1, 4}, {1, 5}, {4, 3}, {5, 7}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    topological_sort(n, graph, in_degrees, actual);
    const Nodes expected {1,2,4,3,5,6,7};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Reverse) {
    constexpr Num n = 7;
    Graph graph(n+1);
    Degrees in_degrees(n+1, 0);

    std::vector<std::pair<Num, Num>> edges {{7, 2}, {7, 4}, {7, 3}, {4, 5}, {3, 1}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    topological_sort(n, graph, in_degrees, actual);
    const Nodes expected {6,7,2,3,1,4,5};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Loop) {
    constexpr Num n = 4;
    Graph graph(n+1);
    Degrees in_degrees(n+1, 0);

    std::vector<std::pair<Num, Num>> edges {{1, 2}, {2, 3}, {3, 4}, {4, 2}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    topological_sort(n, graph, in_degrees, actual);
    const Nodes expected {1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
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
