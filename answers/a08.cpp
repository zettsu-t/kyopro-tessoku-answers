// https://atcoder.jp/contests/tessoku-book/submissions/38744325
#include <algorithm>
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    Num h {0};
    Num w {0};
    is >> h >> w;

    const boost::array<Matrix::index, 2> shape {{h+1, w+1}};
    Matrix board(shape);
    std::fill_n(board.data(), board.num_elements(), 0LL);

    for (decltype(h) y{1}; y<=h; ++y) {
        for (decltype(w) x{1}; x<=w; ++x) {
            Num cell {0};
            is >> cell;
            board[y][x] = cell;
        }
    }

    decltype(board) cumsum(shape);
    std::fill_n(cumsum.data(), cumsum.num_elements(), 0LL);
    for (decltype(h) y{1}; y<=h; ++y) {
        for (decltype(w) x{1}; x<=w; ++x) {
            cumsum[y][x] = board[y][x] + cumsum[y-1][x] + cumsum[y][x-1] - cumsum[y-1][x-1];
        }
    }

    size_t q {0};
    is >> q;
    for (decltype(q) i{0}; i<q; ++i) {
        Num top {0};
        Num left {0};
        Num bottom {0};
        Num right {0};
        is >> top >> left >> bottom >> right;
        --top;
        --left;

        const auto sum = cumsum[bottom][right] - cumsum[bottom][left] - cumsum[top][right] + cumsum[top][left];
        os << sum << "\n";
    }

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
