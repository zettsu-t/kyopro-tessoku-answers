#include <algorithm>
#include <limits>
#include <boost/multi_array.hpp>
#include <gtest/gtest.h>

using Num = long long int;
using Distance = long long int;
using Matrix = boost::multi_array<Distance, 2>;
using MatrixShape = boost::array<Matrix::index, 2>;

void floyd_warshall(Num n, Matrix& distances) {
    for(decltype(n) i{1}; i<=n; ++i) {
        for(decltype(n) x{1}; x<=n; ++x) {
            for(decltype(n) y{1}; y<=n; ++y) {
                distances[x][y] = std::min(distances[x][y], distances[x][i] + distances[i][y]);
            }
        }
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    Num n = 4;
    const Num size = n + 1;
    constexpr Distance inf = 100000000000LL;

    MatrixShape shape {{size, size}};
    Matrix actual(shape);
    std::fill_n(actual.data(), actual.num_elements(), inf);
    actual[1][2] = 10;
    actual[2][4] = 20;
    actual[1][4] = 40;
    actual[2][2] = 0;
    actual[2][1] = 6;
    actual[4][2] = 3;
    actual[4][1] = 5;

    Matrix expected(shape);
    std::fill_n(expected.data(), expected.num_elements(), inf);
    expected[1][1] = 16;
    expected[1][2] = 10;
    expected[1][3] = inf;
    expected[1][4] = 30;
    expected[2][1] = 6;
    expected[2][2] = 0;
    expected[2][3] = inf;
    expected[2][4] = 20;
    expected[3][1] = inf;
    expected[3][2] = inf;
    expected[3][3] = inf;
    expected[3][4] = inf;
    expected[4][1] = 5;
    expected[4][2] = 3;
    expected[4][3] = inf;
    expected[4][4] = 23;

    floyd_warshall(n, actual);
    for(decltype(n) x{1}; x<=n; ++x) {
        for(decltype(n) y{1}; y<=n; ++y) {
            EXPECT_EQ(expected[y][x], actual[y][x]);
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
