// https://atcoder.jp/contests/tessoku-book/submissions/38775576
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    Num nim_sum {0};
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nim_sum ^= a;
    }

    if (!nim_sum) {
        os << "Second\n";
    } else {
        os << "First\n";
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
