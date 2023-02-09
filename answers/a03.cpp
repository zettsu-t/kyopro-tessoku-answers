// https://atcoder.jp/contests/tessoku-book/submissions/38734828
#include <iostream>
#include <set>

void solve(std::istream& is, std::ostream& os) {
    using Num = int;
    size_t n {0};
    Num k {0};
    is >> n >> k;

    std::set<Num> ps;
    for(size_t i=0; i<n; ++i) {
        Num p {0};
        is >> p;
        ps.insert(p);
    }

    for(size_t i=0; i<n; ++i) {
        Num q {0};
        is >> q;
        const auto d = k - q;
        if (ps.find(d) != ps.end()) {
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
