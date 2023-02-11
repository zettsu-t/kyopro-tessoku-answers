// https://atcoder.jp/contests/tessoku-book/submissions/38765114
#include <cmath>
#include <iomanip>
#include <iostream>

namespace {
    using Numeric = double;
}

void solve(std::istream& is, std::ostream& os) {
    Numeric n {0.0};
    is >> n;

    constexpr Numeric tolerance {0.001};
    Numeric left {0.0};
    Numeric right {n};

    for(;;) {
        const Numeric center {(left + right) / 2.0};
        const Numeric diff = center * center * center + center - n;
        if (std::abs(diff) < tolerance) {
            os << std::setprecision(10) << center << "\n";
            break;
        }

        if (diff > 0.0) {
            right = center;
        } else {
            left = center;
        }
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
