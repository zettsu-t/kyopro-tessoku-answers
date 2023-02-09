// https://atcoder.jp/contests/tessoku-book/submissions/38744388
#include <algorithm>
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    constexpr Num width = 1500;
    constexpr Num height = 1500;
    const boost::array<Matrix::index, 2> shape {{height+2, width+2}};
    Matrix area(shape);
    std::fill_n(area.data(), area.num_elements(), 0LL);

    Num h {0};
    Num w {0};
    Num n {0};
    is >> h >> w >> n;

    for (decltype(n) i=0; i<n; ++i) {
        Num top {0};
        Num left {0};
        Num bottom {0};
        Num right {0};
        is >> top >> left >> bottom >> right;
        bottom += 1;
        right += 1;
        area[top][left] += 1;
        area[top][right] -= 1;
        area[bottom][left] -= 1;
        area[bottom][right] += 1;
    }

    Matrix output(shape);
    std::fill_n(output.data(), output.num_elements(), 0LL);

    for (Num y{1}; y<=h; ++y) {
        for (Num x{1}; x<=w; ++x) {
            output[y][x] = output[y][x-1] + area[y][x];
        }
    }

    for (Num x{1}; x<=w; ++x) {
        for (Num y{1}; y<=h; ++y) {
            output[y][x] = output[y-1][x] + output[y][x];
        }
    }

    for (Num y{1}; y<=h; ++y) {
        for (Num x{1}; x<=w; ++x) {
            os << output[y][x];
            if (x < w) {
                os << " ";
            } else {
                os << "\n";
            }
        }
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
