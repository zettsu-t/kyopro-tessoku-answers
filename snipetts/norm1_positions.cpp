#include <algorithm>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using Positions = std::vector<std::pair<Num, Num>>;
}

Positions norm1_positions(Num width, Num height, Num distance) {
    Positions ps;
    const auto max_distance = width + height - 2;
    if (distance > max_distance) {
        return ps;
    }

    Num start_y = std::min(height - 1, distance);
    Num start_x = std::max(0LL, distance - start_y);
    Num y {start_y};
    Num x {start_x};
    for(;;) {
        ps.push_back(std::make_pair(x, y));
        ++x;
        --y;
        if ((x >= width) || (y < 0)) {
            break;
        }
    }

    return ps;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Point) {
    const Positions expected {{0, 0}};
    const auto actual = norm1_positions(1, 1, 0);

    const auto size = expected.size();
    ASSERT_EQ(size, actual.size());

    for(size_t i{0}; i<size; ++i) {
        ASSERT_EQ(expected.at(i), actual.at(i));
    }

    ASSERT_TRUE(norm1_positions(1, 1, 1).empty());
}

TEST_F(TestAll, Square) {
    const Positions expected0 {{0, 0}};
    const auto actual0 = norm1_positions(3, 3, 0);
    const auto size0 = expected0.size();
    ASSERT_EQ(size0, actual0.size());
    for(size_t i{0}; i<size0; ++i) {
        ASSERT_EQ(expected0.at(i), actual0.at(i));
    }

    const Positions expected1 {{0, 1}, {1, 0}};
    const auto actual1 = norm1_positions(3, 3, 1);
    const auto size1 = expected1.size();
    ASSERT_EQ(size1, actual1.size());
    for(size_t i{0}; i<size1; ++i) {
        ASSERT_EQ(expected1.at(i), actual1.at(i));
    }

    const Positions expected2 {{0, 2}, {1, 1}, {2, 0}};
    const auto actual2 = norm1_positions(3, 3, 2);
    const auto size2 = expected2.size();
    ASSERT_EQ(size2, actual2.size());
    for(size_t i{0}; i<size2; ++i) {
        ASSERT_EQ(expected2.at(i), actual2.at(i));
    }

    const Positions expected3 {{1, 2}, {2, 1}};
    const auto actual3 = norm1_positions(3, 3, 3);
    const auto size3 = expected3.size();
    ASSERT_EQ(size3, actual3.size());
    for(size_t i{0}; i<size3; ++i) {
        ASSERT_EQ(expected3.at(i), actual3.at(i));
    }

    const Positions expected4 {{2, 2}};
    const auto actual4 = norm1_positions(3, 3, 4);
    const auto size4 = expected4.size();
    ASSERT_EQ(size4, actual4.size());
    for(size_t i{0}; i<size4; ++i) {
        ASSERT_EQ(expected4.at(i), actual4.at(i));
    }

    ASSERT_TRUE(norm1_positions(3, 3, 5).empty());
}

TEST_F(TestAll, Landscape) {
    const std::vector<size_t> expected_size {1, 2, 3, 3, 2, 1};
    for(Num distance{0}; distance<6; ++distance) {
        const auto start_x = (distance <= 2) ? 0 : (distance - 2);
        const auto start_y = (distance <= 2) ? distance : 2;
        const auto actual = norm1_positions(4, 3, distance);
        ASSERT_EQ(expected_size.at(distance), actual.size());

        Num expected_x {start_x};
        Num expected_y {start_y};
        for(size_t i{0}; i<actual.size(); ++i) {
            const auto actual_x = actual.at(i).first;
            const auto actual_y = actual.at(i).second;
            ASSERT_EQ(expected_x, actual_x);
            ASSERT_EQ(expected_y, actual_y);
            ASSERT_EQ(distance, actual_x + actual_y);
            ++expected_x;
            --expected_y;
        }
    }

    ASSERT_TRUE(norm1_positions(4, 3, 7).empty());
}

TEST_F(TestAll, Portlait) {
    const std::vector<size_t> expected_size {1, 2, 3, 3, 2, 1};
    for(Num distance{0}; distance<6; ++distance) {
        const auto start_x = (distance <= 3) ? 0 : (distance - 3);
        const auto start_y = (distance <= 3) ? distance : 3;
        const auto actual = norm1_positions(3, 4, distance);
        ASSERT_EQ(expected_size.at(distance), actual.size());

        Num expected_x {start_x};
        Num expected_y {start_y};
        for(size_t i{0}; i<actual.size(); ++i) {
            const auto actual_x = actual.at(i).first;
            const auto actual_y = actual.at(i).second;
            ASSERT_EQ(expected_x, actual_x);
            ASSERT_EQ(expected_y, actual_y);
            ASSERT_EQ(distance, actual_x + actual_y);
            ++expected_x;
            --expected_y;
        }
    }

    ASSERT_TRUE(norm1_positions(3, 4, 7).empty());
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
