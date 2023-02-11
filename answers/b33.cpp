// https://atcoder.jp/contests/tessoku-book/submissions/38775659
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    Num h {0};
    Num w {0};
    is >> n >> h >> w;

    Num nim_sum {0};
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        nim_sum ^= (a - 1) ^ (b - 1);
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
