// https://atcoder.jp/contests/tessoku-book/submissions/38775247
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

    Num div(Num a, Num b, Num mod) {
        return ((a % mod) * power(b, mod-2, mod)) % mod;
    }
}

void solve(std::istream& is, std::ostream& os) {
    Num h {0};
    Num w {0};
    is >> h >> w;

    Num n_combi {1};
    Num mod {1000000007};
    Num n {h + w - 2};
    Num r {h - 1};

    for(Num i=0; i<(n-r); ++i) {
        n_combi = ((n_combi % mod) * (r + 1 + i)) % mod;
        n_combi = div(n_combi, i + 1, mod);
    }

    os << n_combi << "\n";
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
