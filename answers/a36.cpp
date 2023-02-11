// https://atcoder.jp/contests/tessoku-book/submissions/38827104
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    is >> n >> k;

    const Num min_len = (n - 1) * 2;
    const Num odd_detour = (k - min_len) & 1;

    if ((k < min_len) || (odd_detour > 0)) {
        os << "No\n";
    } else {
        os << "Yes\n";
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
