// https://atcoder.jp/contests/tessoku-book/submissions/39809003
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr Num inf = -1000000000000LL;

    Num n {0};
    Num m {0};
    Num k {0};
    is >> n >> m >> k;

    const MatrixShape score_shape {{n+1, n+1}};
    Matrix score(score_shape);
    std::fill_n(score.data(), score.num_elements(), 0);

    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        if (a > b) {
            std::swap(a, b);
        }
        for(decltype(a) from{1}; from<=n; ++from) {
            for(decltype(b) to{1}; to<=n; ++to) {
                if ((from <= a) & (to >= b)) {
                    score[from][to] += 1;
                    score[to][from] += 1;
                }
            }
        }
    }

    const MatrixShape board_shape {{k+1, n+1}};
    Matrix board(board_shape);
    std::fill_n(board.data(), board.num_elements(), inf);
    board[0][0] = 0;

    for(decltype(k) chapter{1}; chapter<=k; ++chapter) {
        for(decltype(n) last_page{1}; last_page<=n; ++last_page) {
            for(decltype(last_page) page{0}; page<last_page; ++page) {
                board[chapter][last_page] = std::max(
                    board[chapter][last_page],
                    board[chapter-1][page] + score[page+1][last_page]);
            }
        }
    }

    os << board[k][n] << "\n";
    return;
}

int main(void) {
    solve(std::cin, std::cout);
    return 0;
}

/*
Local Variables:
mode: c++
coding: utf-8-dos
tab-width: nil
c-file-style: "stroustrup"
End:
*/
