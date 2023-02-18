// https://atcoder.jp/contests/tessoku-book/submissions/38953555
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num w {0};
    is >> w;
    --w;

    size_t n_bits = sizeof(Num) * 8;
    constexpr Num base {7};
    constexpr Num mod {1000000007};
    std::vector<Num> powered{base};
    for(decltype(n_bits) i{1}; i<n_bits; ++i) {
        powered.push_back((powered.at(i-1) * powered.at(i-1)) % mod);
    }

    Num answer {12};
    for(decltype(n_bits) i{0}; i<n_bits; ++i) {
        if ((w & 1) > 0) {
            answer = (answer * powered.at(i)) % mod;
        }
        w >>= 1;
    }

    os << answer << "\n";
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
