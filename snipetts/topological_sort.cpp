#include <iostream>
#include <queue>
#include <vector>
#include <gtest/gtest.h>

using Num = long long int;
using Nodes = std::vector<Num>;
using Degrees = std::vector<Num>;
using Graph = std::vector<Nodes>;

bool topological_sort(Num n, const Graph& graph, const Degrees& in_degrees, Nodes& answer) {
    auto updated_in_degrees = in_degrees;
    std::priority_queue<Num, std::vector<Num>, std::greater<Num>> candidates;

    for(decltype(n) i{0}; i<n; ++i) {
        if (updated_in_degrees.at(i) == 0) {
            candidates.push(i);
        }
    }

    Nodes sorted_nodes;
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, TreeForward) {
    constexpr Num n = 7;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{0, 5}, {0, 3}, {0, 4}, {3, 2}, {4, 6}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {0,1,3,2,4,5,6};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, TreeReverse) {
    constexpr Num n = 7;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{6, 1}, {6, 3}, {6, 2}, {3, 4}, {2, 0}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {5,6,1,2,0,3,4};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, PathGraph) {
    constexpr Num n = 5;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{4, 3}, {3, 2}, {2, 1}, {1, 0}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    EXPECT_TRUE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {4,3,2,1,0};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Merge) {
    constexpr Num n = 5;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{4, 1}, {1, 2}, {1, 0}, {2, 3}, {0, 3}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {4,1,0,2,3};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, NotConnected) {
    constexpr Num n = 2;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges;
    Nodes actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {0,1};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Loop) {
    constexpr Num n = 4;
    Graph graph(n);
    Degrees in_degrees(n, 0);

    std::vector<std::pair<Num, Num>> edges {{0, 1}, {1, 2}, {2, 3}, {3, 1}};
    for(const auto& edge : edges) {
        graph.at(edge.first).push_back(edge.second);
        in_degrees.at(edge.second) += 1;
    }

    Nodes actual;
    EXPECT_FALSE(topological_sort(n, graph, in_degrees, actual));
    const Nodes expected {0};
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
