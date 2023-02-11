// https://atcoder.jp/contests/tessoku-book/submissions/38774074
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
    Matrix count(shape);
    Matrix map(shape);
    std::fill_n(count.data(), count.num_elements(), 0LL);
    std::fill_n(map.data(), map.num_elements(), 0LL);

    for (decltype(h) y{0}; y<h; ++y) {
        std::string s;
        is >> s;
        for (decltype(w) x{0}; x<w; ++x) {
            map[y+1][x+1] = (s.at(x) == '.') ? 1 : 0;
        }
    }

    count[1][1] = 1;
    for (decltype(h) y{1}; y<=h; ++y) {
        for (decltype(w) x{1}; x<=w; ++x) {
            if ((y > 1) || (x > 1)) {
                count[y][x] += map[y-1][x] ? count[y-1][x] : 0;
                count[y][x] += map[y][x-1] ? count[y][x-1] : 0;
            }
        }
    }

    os << count[h][w] << "\n";
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
