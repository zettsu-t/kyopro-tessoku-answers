// https://atcoder.jp/contests/tessoku-book/submissions/38827222
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num m {0};
    Num b {0};
    is >> n >> m >> b;

    Num sum_stations {0};
    Num sum_busstops {0};
    for(decltype(n) i=0; i<n; ++i) {
        Num a {0};
        is >> a;
        sum_stations += a;
    }

    for(decltype(n) i=0; i<m; ++i) {
        Num c {0};
        is >> c;
        sum_busstops += c;
    }

    Num total = sum_stations * m + sum_busstops * n + b * m * n;
    os << total << "\n";
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
