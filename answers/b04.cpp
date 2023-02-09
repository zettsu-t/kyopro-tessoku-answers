// https://atcoder.jp/contests/tessoku-book/submissions/38735224
#include <iostream>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    std::string s;
    is >> s;

    long long int n {0};
    for(const auto& c : s) {
        n <<= 1;
        n += (c == '0') ? 0 : 1;
    }

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
