// https://atcoder.jp/contests/tessoku-book/submissions/38735352
#include <algorithm>
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    is >> n >> k;

    Num n_combinations {0};
    const Num red_min = std::max(k - n * 2, 1LL);
    const Num red_max = std::min(n, k - 2LL);
    for(Num red = red_min; red <= red_max; ++red) {
        const auto remaining = k - red;
        if (remaining >= 2) {
            const Num blue_min = std::max(remaining - n, 1LL);
            const Num blue_max = std::min(n, remaining - 1LL);
            if (blue_min <= blue_max) {
                n_combinations += blue_max - blue_min + 1LL;
            }
        }
    }

    os << n_combinations << "\n";
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
