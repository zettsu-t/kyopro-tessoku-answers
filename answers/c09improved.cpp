// https://atcoder.jp/contests/tessoku-book/submissions/39807161
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

    std::vector<Num> sums_active(n, 0);
    std::vector<Num> sums_sleep(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        if (i < 1) {
            sums_active.at(i) = diffs.at(i);
            sums_sleep.at(i) = 0;
        } else {
            sums_active.at(i) = diffs.at(i) + sums_sleep.at(i-1);
            sums_sleep.at(i) = std::max(sums_active.at(i-1), sums_sleep.at(i-1));
        }
    }

    os << std::max(sums_active.at(n-1), sums_sleep.at(n-1)) << "\n";
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
