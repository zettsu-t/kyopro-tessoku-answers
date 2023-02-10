// https://atcoder.jp/contests/tessoku-book/submissions/38755069
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }
    std::sort(nums.begin(), nums.end());

    size_t q {0};
    is >> q;
    for (decltype(q) i{0}; i<q; ++i) {
        Num x {0};
        is >> x;
        const auto dist = std::lower_bound(nums.begin(), nums.end(), x) - nums.begin();
        os << dist << "\n";
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
