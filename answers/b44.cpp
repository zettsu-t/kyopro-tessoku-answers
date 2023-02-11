// https://atcoder.jp/contests/tessoku-book/submissions/38828028
#include <algorithm>
#include <iostream>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;

    Num n {0};
    is >> n;
    const boost::array<Matrix::index, 2> shape {{n, n}};
    Matrix nums(shape);
    std::fill_n(nums.data(), nums.num_elements(), 0LL);

    std::vector<Num> row_aliases(n, 0);
    for(decltype(n) y{0}; y<n; ++y) {
        row_aliases.at(y) = y;
        for(decltype(n) x{0}; x<n; ++x) {
            Num v {0};
            is >> v;
            nums[y][x] = v;
        }
    }

    Num q {0};
    is >> q;

    for(decltype(n) i{0}; i<q; ++i) {
        Num c {0};
        Num x {0};
        Num y {0};
        is >> c >> x >> y;
        --x;
        --y;
        if (c == 1) {
            std::swap(row_aliases[x], row_aliases[y]);
        } else if (c == 2) {
            os << nums[row_aliases[x]][y] << "\n";
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
