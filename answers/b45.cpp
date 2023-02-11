// https://atcoder.jp/contests/tessoku-book/submissions/38828083
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num a {0};
    Num b {0};
    Num c {0};
    is >> a >> b >> c;

    if ((a + b + c) == 0) {
        os << "Yes\n";
    } else {
        os << "No\n";
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
