// https://atcoder.jp/contests/tessoku-book/submissions/38744315
#include <algorithm>
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    constexpr Num width = 1500;
    constexpr Num height = 1500;
    const boost::array<Matrix::index, 2> shape {{height+1, width+1}};
    Matrix board(shape);
    std::fill_n(board.data(), board.num_elements(), 0LL);

    size_t n {0};
    is >> n;
    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        Num y {0};
        is >> x;
        is >> y;
        board[y][x] += 1;
    }

    decltype(board) cumsum(shape);
    std::fill_n(cumsum.data(), cumsum.num_elements(), 0LL);
    for (Num y{1}; y<=height; ++y) {
        for (Num x{1}; x<=width; ++x) {
            cumsum[y][x] = cumsum[y][x-1] + board[y][x];
        }
    }

    for (Num y{1}; y<=height; ++y) {
        for (Num x{1}; x<=width; ++x) {
            cumsum[y][x] += cumsum[y-1][x];
        }
    }

    size_t q {0};
    is >> q;
    for (decltype(q) i{0}; i<q; ++i) {
        Num left {0};
        Num top {0};
        Num right {0};
        Num bottom {0};
        is >> left >> top >> right >> bottom;
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
