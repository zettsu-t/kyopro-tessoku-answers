// https://atcoder.jp/contests/tessoku-book/submissions/38774547
#include <iostream>

namespace {
    using Num = long long int;
}

constexpr Num gcd(Num a, Num b) {
    Num big = a;
    Num small = b;
    Num r {1};

    if (big < small) {
        std::swap(big, small);
    }

    while(r > 0) {
        r = big % small;
        if (r == 0) {
            return small;
        }
        big = small;
        small = r;
    }

    return 0;
}

void solve(std::istream& is, std::ostream& os) {
    Num a {0};
    Num b {0};
    is >> a >> b;

    const Num ab = a * b;
    os << (ab / gcd(a, b)) << "\n";
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
