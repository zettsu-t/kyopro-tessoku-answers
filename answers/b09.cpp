// https://atcoder.jp/contests/tessoku-book/submissions/38752574
#include <algorithm>
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    constexpr Num width = 1500;
    constexpr Num height = 1500;
    const boost::array<Matrix::index, 2> shape {{height+3, width+3}};
    Matrix area(shape);
    std::fill_n(area.data(), area.num_elements(), 0LL);

    size_t n {0};
    is >> n;
    for (decltype(n) i{0}; i<n; ++i) {
        Num left {0};
        Num top {0};
        Num right {0};
        Num bottom {0};
        is >> left >> top >> right >> bottom;
        ++top;
        ++left;
        ++bottom;
        ++right;
        area[top][left] += 1;
        area[top][right] -= 1;
        area[bottom][left] -= 1;
        area[bottom][right] += 1;
    }

    Matrix output(shape);
    std::fill_n(output.data(), output.num_elements(), 0LL);

    for (Num y{1}; y<=(height+1); ++y) {
        for (Num x{1}; x<=(width+1); ++x) {
            output[y][x] = output[y][x-1] + area[y][x];
        }
    }

    Num count {0};
    for (Num x{1}; x<=(width+1); ++x) {
        for (Num y{1}; y<=(height+1); ++y) {
            const auto sum = output[y-1][x] + output[y][x];
            output[y][x] = sum;
            count += (sum > 0) ? 1 : 0;
        }
    }

    os << count << "\n";
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
