#include <vector>
#include <atcoder/all>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
    using ModInt = atcoder::modint1000000007;
}

ModInt calc_combi(Num n, Num k) {
    if (n < k) {
        return 0;
    }

    ModInt n_combi = 1;
    for(Num i{1}; i<=k; ++i) {
        n_combi *= (n + 1 - i);
    }

    for(Num i{1}; i<=k; ++i) {
        n_combi /= i;
    }

    return n_combi;
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Small) {
    EXPECT_EQ(1, calc_combi(0, 0).val());
    EXPECT_EQ(0, calc_combi(0, 1).val());

    const std::vector<std::vector<Num>> answers {
        {1, 1, 0},
        {1, 2, 1, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 6, 4, 1, 0},
        {1, 5, 10, 10, 5, 1, 0}
    };

    for(Num n{1}; n<=5; ++n) {
        for(Num k{0}; k<=(n+1); ++k) {
            const auto actual = calc_combi(n, k).val();
            const auto expected = answers[n-1][k];
            EXPECT_EQ(expected, actual);
        }
    }
}

TEST_F(TestAll, Large) {
    const auto actual10 = calc_combi(40, 10).val();
    const auto expected10 = 847660528;
    EXPECT_EQ(expected10, actual10);

    const auto actual11 = calc_combi(40, 11).val();
    const auto expected11 = 311801426;
    EXPECT_EQ(expected11, actual11);

    const auto actual8 = calc_combi(51, 8).val();
    const auto expected8 = 636763050;
    EXPECT_EQ(expected8, actual8);

    const auto actual9 = calc_combi(51, 9).val();
    const auto expected9 = 42312329;
    EXPECT_EQ(expected9, actual9);
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
