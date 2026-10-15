// SPDX-FileCopyrightText: 2026 Zettsu Tatsuya
// SPDX-License-Identifier: MIT
#include <array>
#include <numeric>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>
#include <atcoder/modint.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using ModInt = atcoder::modint1000000007;
    using Matrix = boost::multi_array<ModInt, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;

    Num x {0};
    Num y {0};
    is >> x >> y;

    MatrixShape shape {{4, 1000009}};
    Matrix board(shape);
    ModInt zero;
    std::fill_n(board.data(), board.num_elements(), zero);
    board[0][0] = 1;

    Num line {0};
    for(Num s{0}; s<(x+y); ++s) {
        Num y_pos = std::max(0LL, s - x);
        Num x_pos = std::min(x, s);
        for(;;) {
            if ((y_pos > y) || (x_pos < 0)) {
                break;
            }
            const auto line1 = (line + 1) % 4;
            const auto line2 = (line + 2) % 4;
            board[line2][x_pos+1] += board[line][x_pos];
            board[line1][x_pos+2] += board[line][x_pos];
            ++y_pos;
            --x_pos;
        }
        line = (line + 1) % 4;
    }

    line = (x + y + 1) % 4;
    os << board[line][x].val() << "\n";
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
