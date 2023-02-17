// https://atcoder.jp/contests/tessoku-book/submissions/38944317
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;
    os << n << "\n";

    for(decltype(n) i{0}; i<n; ++i) {
        Num to = (i + 1) % n;
        os << (i+1) << " " << (to+1) << "\n";
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
