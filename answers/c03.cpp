// https://atcoder.jp/contests/tessoku-book/submissions/38943758
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t d {0};
    Num x {0};
    is >> d >> x;

    std::vector<Num> chart(d, 0);
    chart.at(0) = x;
    for(decltype(d) i{1}; i<d; ++i) {
        Num diff {0};
        is >> diff;
        chart.at(i) += chart.at(i-1) + diff;
    }

    size_t q {0};
    is >> q;
    for(decltype(d) i{0}; i<q; ++i) {
        Num s {0};
        Num t {0};
        is >> s >> t;
        if (chart.at(s-1) > chart.at(t-1)) {
            os << s;
        } else if (chart.at(s-1) < chart.at(t-1)) {
            os << t;
        } else {
            os << "Same";
        }
        os << "\n";
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
