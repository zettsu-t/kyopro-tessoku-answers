// https://atcoder.jp/contests/tessoku-book/submissions/38770277
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> ones(n-1, 0);
    for (decltype(n) i{0}; i<(n-1); ++i) {
        Num a {0};
        is >> a;
        ones.at(i) = a;
    }

    std::vector<Num> twos(n-2, 0);
    for (decltype(n) i{0}; i<(n-2); ++i) {
        Num b {0};
        is >> b;
        twos.at(i) = b;
    }

    std::vector<Num> costs(n, 0);
    costs.at(1) = ones.at(0);
    for (decltype(n) i{0}; i<=(n-2); ++i) {
        if (i < (n-2)) {
            costs.at(i+2) = costs.at(i) + twos.at(i);
        }
        costs.at(i+1) = std::min(costs.at(i+1), costs.at(i) +  ones.at(i));
    }

    os << costs.at(n-1) << "\n";
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
