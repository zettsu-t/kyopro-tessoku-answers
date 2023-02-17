// https://atcoder.jp/contests/tessoku-book/submissions/38943576
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    long long int n {0};
    is >> n;
    n *= 110;
    n /= 100;
    os << n << "\n";
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
