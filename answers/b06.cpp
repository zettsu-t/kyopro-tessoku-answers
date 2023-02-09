// https://atcoder.jp/contests/tessoku-book/submissions/38743957
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> cumsum(n+1, 0);
    for (decltype(n) i{1}; i<=n; ++i) {
        Num a {0};
        is >> a;
        cumsum.at(i) = cumsum.at(i-1) + ((a > 0) ? 1 : -1);
    }

    size_t q {0};
    is >> q;

    for (decltype(n) i{1}; i<=q; ++i) {
        Num left {0};
        Num right {0};
        is >> left;
        is >> right;
        const auto diff = cumsum.at(right) - cumsum.at(left - 1);
        if (diff > 0) {
            os << "win\n";
        } else if (diff < 0) {
            os << "lose\n";
        } else {
            os << "draw\n";
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
