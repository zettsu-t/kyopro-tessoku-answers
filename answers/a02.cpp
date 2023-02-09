// https://atcoder.jp/contests/tessoku-book/submissions/38734675
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    int x {0};
    is >> n >> x;

    for(size_t i=0; i<n; ++i) {
        decltype(x) a {0};
        is >> a;
        if (a == x) {
            os << "Yes\n";
            return;
        }
    }

    os << "No\n";
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
