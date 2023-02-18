// https://atcoder.jp/contests/tessoku-book/submissions/38953189
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> diffs(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        diffs.at(i) = a;
    }

    std::vector<Num> sums(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        if (i <= 1) {
            sums.at(i) = diffs.at(i);
        } else if (i == 2) {
            sums.at(i) = std::max(sums.at(i-2) + diffs.at(i), sums.at(i-1));
        } else {
            sums.at(i) = std::max(sums.at(i-2) + diffs.at(i), sums.at(i-1));
            sums.at(i) = std::max(sums.at(i-3) + diffs.at(i), sums.at(i));
        }
    }

    os << sums.at(n-1) << "\n";
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
