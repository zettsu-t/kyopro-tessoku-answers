#include <iostream>
#include <gtest/gtest.h>

using Num = long long int;
using Matrix = std::vector<std::vector<Num>>;

Matrix rotate_board(const Matrix& input) {
    auto input_height = input.size();
    auto input_width = input.at(0).size();
    auto target_height = input_width;
    auto target_width = input_height;

    std::vector<std::vector<Num>> board(target_height);
    for(decltype(target_height) y{0}; y<target_height; ++y) {
        for(decltype(target_width) x{0}; x<target_width; ++x) {
            board.at(y).push_back(input[input_height-1-x][y]);
        }
    }

    return board;
}

bool are_equal(const Matrix& lhs, const Matrix& rhs) {
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

class TestAll : public ::testing::Test {};

TEST_F(TestAll, ToRow) {
    Matrix input {{4, 5, 7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 3);
    for(Num y{0}; y<3; ++y) {
        ASSERT_EQ(input.at(0).at(y), actual.at(y).at(0));
    }
}

TEST_F(TestAll, ToColumn) {
    Matrix input {{4}, {5}, {7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 1);
    const auto& column = actual.at(0);
    const Matrix expected {{7, 5, 4}};
    ASSERT_TRUE(std::equal(column.begin(), column.end(), expected.at(0).begin()));
    ASSERT_TRUE(are_equal(expected, actual));
    ASSERT_FALSE(are_equal(expected, input));
}

TEST_F(TestAll, Square) {
    Matrix input1 {{3}};
    const auto actual1 = rotate_board(input1);
    ASSERT_TRUE(actual1.size() == 1);
    ASSERT_TRUE(actual1.at(0).size() == 1);
    ASSERT_TRUE(actual1.at(0).at(0) == 3);

    Matrix input2 {{3, 4}, {5, 6}};
    const auto actual2 = rotate_board(input2);
    ASSERT_TRUE(actual2.size() == 2);
    ASSERT_TRUE(actual2.at(0).size() == 2);
    ASSERT_TRUE(actual2.at(1).size() == 2);

    const Matrix expected2 {{5, 3}, {6, 4}};
    ASSERT_TRUE(std::equal(actual2.at(0).begin(), actual2.at(0).end(), expected2.at(0).begin()));
    ASSERT_TRUE(std::equal(actual2.at(1).begin(), actual2.at(1).end(), expected2.at(1).begin()));
    ASSERT_TRUE(are_equal(expected2, actual2));
    ASSERT_FALSE(are_equal(expected2, input2));
}

TEST_F(TestAll, Rectangular) {
    const Matrix expected1 {{3, 4, 5}, {6, 7, 8}};
    const Matrix expected2 {{6, 3}, {7, 4}, {8, 5}};
    const Matrix expected3 {{8, 7, 6}, {5, 4, 3}};
    const Matrix expected4 {{5, 8}, {4, 7}, {3, 6}};

    ASSERT_TRUE(are_equal(expected2, rotate_board(expected1)));
    ASSERT_TRUE(are_equal(expected3, rotate_board(expected2)));
    ASSERT_TRUE(are_equal(expected4, rotate_board(expected3)));
    ASSERT_TRUE(are_equal(expected1, rotate_board(expected4)));
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
