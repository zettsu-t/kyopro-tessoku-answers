// https://atcoder.jp/contests/tessoku-book/submissions/38833167
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> nums_a;
    std::vector<Num> nums_b;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums_a.push_back(a);
    }
    std::sort(nums_a.begin(), nums_a.end(), std::greater<Num>{});

    for(decltype(n) i{0}; i<n; ++i) {
        Num b {0};
        is >> b;
        nums_b.push_back(b);
    }
    std::sort(nums_b.begin(), nums_b.end());

    Num total {0};
    for(decltype(n) i{0}; i<n; ++i) {
        total += nums_a.at(i) * nums_b.at(i);
    }

    os << total << "\n";
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
