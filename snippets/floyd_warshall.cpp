#include <algorithm>
#include <limits>
#include <gtest/gtest.h>

using Num = long long int;
using Distance = long long int;
constexpr Num MaxWidth = 1000;
using Matrix = std::array<std::array<Num, MaxWidth>, MaxWidth>;

void floyd_warshall(Num n, Matrix& distances) {
    for(decltype(n) i{0}; i<n; ++i) {
        for(decltype(n) x{0}; x<n; ++x) {
            for(decltype(n) y{0}; y<n; ++y) {
                distances[x][y] = std::min(distances[x][y], distances[x][i] + distances[i][y]);
            }
        }
    }
}

void init_array(Matrix& distances, Num value) {
    for(Num i{0}; i<MaxWidth; ++i) {
        for(Num j{0}; j<MaxWidth; ++j) {
            distances[i][j] = value;
        }
    }
}

class TestAll : public ::testing::Test {
protected:
    Matrix actual;
    Matrix expected;
};

TEST_F(TestAll, All) {
    constexpr Num n = 5;
    static_assert(n < MaxWidth);
    constexpr Distance inf = 100000000000LL;

    init_array(actual, inf);
    actual[0][1] = 10;
    actual[1][3] = 20;
    actual[0][3] = 40;
    actual[1][1] = 0;
    actual[1][0] = 6;
    actual[3][1] = 3;
    actual[3][0] = 5;

    init_array(expected, inf);
    expected[0][0] = 16;
    expected[0][1] = 10;
    expected[0][2] = inf;
    expected[0][3] = 30;
    expected[1][0] = 6;
    expected[1][1] = 0;
    expected[1][2] = inf;
    expected[1][3] = 20;
    expected[2][0] = inf;
    expected[2][1] = inf;
    expected[2][2] = inf;
    expected[2][3] = inf;
    expected[3][0] = 5;
    expected[3][1] = 3;
    expected[3][2] = inf;
    expected[3][3] = 23;

    floyd_warshall(n, actual);
    for(Num x{1}; x<=n; ++x) {
        for(Num y{1}; y<=n; ++y) {
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
