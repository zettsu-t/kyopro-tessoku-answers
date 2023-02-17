// https://atcoder.jp/contests/tessoku-book/submissions/38944476
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> price_set(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num c {0};
        is >> c;
        price_set.at(i) = c;
    }
    std::sort(price_set.begin(), price_set.end());

    std::vector<Num> cumsum(n+1, 0);
    for(decltype(n) i{1}; i<=n; ++i) {
        cumsum.at(i) = cumsum.at(i-1) + price_set.at(i-1);
    }

    size_t q {0};
    is >> q;
    for(decltype(n) i{0}; i<q; ++i) {
        Num x {0};
        is >> x;
        auto it = std::upper_bound(cumsum.begin(), cumsum.end(), x);
        if (it != cumsum.begin()) {
            --it;
        }
        auto count = it - cumsum.begin();
        os << count << "\n";
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
