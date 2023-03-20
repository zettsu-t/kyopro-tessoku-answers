// https://atcoder.jp/contests/tessoku-book/submissions/39897928
#include <iostream>
#include <map>
#include <vector>

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
    size_t n {0};
    Num p {0};
    is >> n >> p;
    constexpr Num mod {1000000007};

    std::multimap<Num, Num> nums;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        const auto key = a % mod;
        nums.insert(std::make_pair(key, a));
    }

    Num total{0};
    if (p == 0) {
        const auto n_zeros = nums.count(0);
        total = ((2 * n - n_zeros - 1) * n_zeros) / 2;
        os << total << "\n";
        return;
    }

    for(auto it = nums.begin(); it != nums.end(); it=nums.upper_bound(it->first)) {
        const auto a = it->first;
        const auto count_a = nums.count(a);
        const auto b = div(p, a, mod);
        if (a == b) {
            total += count_a * (count_a - 1);
        } else {
            const auto count_b = nums.count(b);
            total += count_a * count_b;
        }
    }

    os << (total / 2) << "\n";
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
