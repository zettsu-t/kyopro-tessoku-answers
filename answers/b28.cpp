// https://atcoder.jp/contests/tessoku-book/submissions/38774822
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;
    n -= 2;

    Num acc1 {1};
    Num acc2 {1};
    constexpr Num base {1000000007};

    for(decltype(n) i{0}; i<n; ++i) {
        Num acc = ((acc1 % base) + (acc2 % base)) % base;
        acc2 = acc1;
        acc1 = acc;
    }

    os << acc1 << "\n";
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
