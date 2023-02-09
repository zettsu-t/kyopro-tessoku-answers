// https://atcoder.jp/contests/tessoku-book/submissions/38735163
#include <algorithm>
#include <iostream>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    int n {0};
    is >> n;
    constexpr int max_n = 1000;
    auto width = sizeof(max_n) * 8 - __builtin_clz(max_n);

    std::string s;
    for(decltype(width) i{0}; i<width; ++i) {
        char c = (n & 1) ? '1' : '0';
        s.push_back(c);
        n >>= 1;
    }

    std::reverse(s.begin(), s.end());
    os << s << "\n";
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
