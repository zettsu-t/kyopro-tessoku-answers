#include <iostream>
#include <gtest/gtest.h>

using Num = long long int;
using Matrix = std::vector<std::vector<Num>>;

Matrix rotate_board(const Matrix& input) {
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

Matrix shrink_board(const Matrix& input) {
    const Matrix zero;
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

    Matrix board(bottom - top + 1);
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

class TestRotate : public ::testing::Test {};

TEST_F(TestRotate, ToRow) {
    Matrix input {{4, 5, 7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 3);
    for(Num y{0}; y<3; ++y) {
        ASSERT_EQ(input.at(0).at(y), actual.at(y).at(0));
    }
}

TEST_F(TestRotate, ToColumn) {
    Matrix input {{4}, {5}, {7}};
    const auto actual = rotate_board(input);

    ASSERT_TRUE(actual.size() == 1);
    const auto& column = actual.at(0);
    const Matrix expected {{7, 5, 4}};
    ASSERT_TRUE(std::equal(column.begin(), column.end(), expected.at(0).begin()));
    ASSERT_TRUE(are_equal(expected, actual));
    ASSERT_FALSE(are_equal(expected, input));
}

TEST_F(TestRotate, Square) {
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

TEST_F(TestRotate, Rectangular) {
    const Matrix expected1 {{3, 4, 5}, {6, 7, 8}};
    const Matrix expected2 {{6, 3}, {7, 4}, {8, 5}};
    const Matrix expected3 {{8, 7, 6}, {5, 4, 3}};
    const Matrix expected4 {{5, 8}, {4, 7}, {3, 6}};

    ASSERT_TRUE(are_equal(expected2, rotate_board(expected1)));
    ASSERT_TRUE(are_equal(expected3, rotate_board(expected2)));
    ASSERT_TRUE(are_equal(expected4, rotate_board(expected3)));
    ASSERT_TRUE(are_equal(expected1, rotate_board(expected4)));
}

TEST_F(TestRotate, Imbalanced) {
    const Matrix input1 {{3, 4}, {6, 7, 8}};
    const Matrix expected1 {{6, 3}, {7, 4}, {8, 0}};
    ASSERT_TRUE(are_equal(expected1, rotate_board(input1)));

    const Matrix input2 {{3, 4, 5}, {6, 7}};
    const Matrix expected2 {{6, 3}, {7, 4}, {0, 5}};
    ASSERT_TRUE(are_equal(expected2, rotate_board(input2)));
}

class TestShrink : public ::testing::Test {};

TEST_F(TestShrink, Empty) {
    const Matrix input;
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal(input, actual));
}

TEST_F(TestShrink, Zero) {
    const Matrix input {{0}};
    const Matrix expected;
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal(expected, actual));
}

TEST_F(TestShrink, One) {
    const Matrix input {{1}};
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal(input, actual));
}

TEST_F(TestShrink, Zeros) {
    const Matrix input {{0,0,0}, {0,0,0}, {0,0,0}};
    const auto actual = shrink_board(input);
    const Matrix expected;
    ASSERT_TRUE(are_equal(expected, actual));
}

TEST_F(TestShrink, Center) {
    const Matrix input {{0,0,0}, {0,1,0}, {0,0,0}};
    const auto actual = shrink_board(input);
    const Matrix expected {{1}};
    ASSERT_TRUE(are_equal(expected, actual));
}

TEST_F(TestShrink, Filled) {
    const Matrix input {{1,1,1}, {1,1,1}, {1,1,1}};
    const auto actual = shrink_board(input);
    ASSERT_TRUE(are_equal(input, actual));
}

TEST_F(TestShrink, Corner) {
    const Matrix input1 {{1,0,0}, {0,0,0}, {0,0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(input1, actual1));

    const Matrix input2 {{0,0,1}, {0,0,0}, {1,0,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(input2, actual2));
}

TEST_F(TestShrink, LeftTop) {
    const Matrix input1 {{1,0,0}, {0,1,0}, {0,0,0}};
    const Matrix expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(expected1, actual1));

    const Matrix input2 {{0,1,0}, {1,0,0}, {0,0,0}};
    const Matrix expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(expected2, actual2));
}

TEST_F(TestShrink, LeftBottom) {
    const Matrix input1 {{0,0,0}, {1,0,0}, {0,1,0}};
    const Matrix expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(expected1, actual1));

    const Matrix input2 {{0,0,0}, {0,1,0}, {1,0,0}};
    const Matrix expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(expected2, actual2));
}

TEST_F(TestShrink, RightTop) {
    const Matrix input1 {{0,1,0}, {0,0,1}, {0,0,0}};
    const Matrix expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(expected1, actual1));

    const Matrix input2 {{0,0,1}, {0,1,0}, {0,0,0}};
    const Matrix expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(expected2, actual2));
}

TEST_F(TestShrink, RightBottom) {
    const Matrix input1 {{0,0,0}, {0,1,0}, {0,0,1}};
    const Matrix expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(expected1, actual1));

    const Matrix input2 {{0,0,0}, {0,0,1}, {0,1,0}};
    const Matrix expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(expected2, actual2));
}

TEST_F(TestShrink, Imbalanced) {
    const Matrix input1 {{0}, {0,1,0}, {0,0,1}};
    const Matrix expected1 {{1,0}, {0,1}};
    const auto actual1 = shrink_board(input1);
    ASSERT_TRUE(are_equal(expected1, actual1));

    const Matrix input2 {{0,0,1}, {0,1,0}, {0}};
    const Matrix expected2 {{0,1}, {1,0}};
    const auto actual2 = shrink_board(input2);
    ASSERT_TRUE(are_equal(expected2, actual2));
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
