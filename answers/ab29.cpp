// https://atcoder.jp/contests/tessoku-book/submissions/38775054
// https://atcoder.jp/contests/tessoku-book/submissions/38775078
#include <iostream>

namespace {
    using Num = long long int;
    Num power(Num x, Num index, Num mod) {
        Num acc {1};
        Num powered = x % mod;

        for(Num bit=0; bit<64; ++bit) {
            Num mask {1};
            mask <<= bit;
            if ((index & mask) > 0) {
                acc = (acc * powered) % mod;
            }
            powered = (powered * powered) % mod;
        }
        return acc;
    }
}

void solve(std::istream& is, std::ostream& os) {
    Num a {0};
    Num b {0};
    constexpr Num mod {1000000007LL};
    is >> a >> b;
    auto acc = power(a, b, mod);
    os << acc << "\n";
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
