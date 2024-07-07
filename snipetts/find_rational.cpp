#include <bits/stdc++.h>
#include <atcoder/modint>

namespace {
    using ModInt = atcoder::modint998244353;
    using Num = long long int;
}

void solve(std::istream& is, std::ostream& os) {
    constexpr Num mod = 998244353;
    const Num upper = std::floor(std::pow(mod,0.3));

    Num n {0};
    is >> n;

    std::set<std::tuple<Num,Num,Num>> ss;
    for(Num bottom{2}; bottom < upper; ++bottom) {
        for(Num top{-upper}; top < upper; ++top) {
            ModInt m = top;
            m /= bottom;
            if (m.val() != n) {
                continue;
            }

            const Num g = std::gcd(std::abs(bottom), std::abs(top));
            Num sum = std::abs(bottom) + std::abs(top);
            sum /= g;
            const auto b = bottom / g;
            const auto t = top / g;
            ss.insert(std::make_tuple(sum, b, t));
        }
    }

    Num rem = 10;
    for(const auto& [sum, bottom, top] : ss) {
        os << top << "/" << bottom << "\n";
        if (--rem <= 0) {
            break;
        }
    }
}

int main(void) {
    solve(std::cin, std::cout);
    return 0;
}
