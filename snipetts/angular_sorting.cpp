#include <cmath>
#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

using Num [[maybe_unused]] = long long int;

// Angular Sorting
// input (X,Y,index)[]
// out {(rank,X,Y):indexes}
std::map<std::tuple<Num,Num,Num>, std::vector<Num>> sort_angular(const std::vector<std::tuple<Num,Num,Num>>& points) {
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, DijkstraAlgo) {
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
