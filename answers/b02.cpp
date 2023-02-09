// https://atcoder.jp/contests/tessoku-book/submissions/38734762
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = int;
    Num a {0};
    Num b {0};
    is >> a >> b;

    constexpr Num n {100};
    for(Num i=a; i<=b; ++i) {
        if (!(n % i)) {
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
