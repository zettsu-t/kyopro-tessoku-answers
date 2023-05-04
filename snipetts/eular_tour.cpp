#include <iostream>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using Edges = std::vector<std::vector<Num>>;
    using Path = std::vector<Num>;
}

void eular_tour(Num current, Num prev, const Edges& edges, Path& path) {
    path.push_back(current);

    for(const auto& next : edges[current]) {
        if (next != prev) {
            eular_tour(next, current, edges, path);
            path.push_back(current);
        }
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
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
