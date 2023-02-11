// https://atcoder.jp/contests/tessoku-book/submissions/38827922
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num m {0};
    is >> n >> m;

    std::vector<Num> counts(n, m);
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        is >> a;
        counts.at(a-1) -= 1;
    }

    for(const auto& a : counts) {
        os << a << "\n";
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
