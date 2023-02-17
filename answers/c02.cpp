// https://atcoder.jp/contests/tessoku-book/submissions/38943633
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    Num top {0};
    Num second {0};

    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        if (a > top) {
            std::swap(second, top);
            std::swap(top, a);
        } else if (a > second) {
            std::swap(second, a);
        }
    }

    os << (top + second) << "\n";
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
