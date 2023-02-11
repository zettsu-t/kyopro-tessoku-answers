// https://atcoder.jp/contests/tessoku-book/submissions/38827558
#include <iostream>
#include <unordered_map>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    constexpr Num mod = 100;
    constexpr Num half_mod = mod / 2;
    size_t n {0};
    is >> n;

    std::unordered_map<Num, Num> bins;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        a %= mod;
        if (bins.find(a) != bins.end()) {
            bins[a] += 1;
        } else {
            bins[a] = 1;
        }
    }

    Num count {0};
    for(Num a{0}; a<=half_mod; ++a) {
        Num count_a {0};
        Num count_b {0};
        Num b = mod - a;
        if ((a == half_mod) || (a == 0)) {
            if (bins.find(a) != bins.end()) {
                count_a = bins[a];
                if (count_a == 2) {
                    count += 1;
                } else if (count_a > 2) {
                    Num local_count = count_a * (count_a - 1);
                    local_count /= 2;
                    count += local_count;
                }
            }
        } else {
            if (bins.find(a) != bins.end()) {
                count_a = bins[a];
            }
            if (bins.find(b) != bins.end()) {
                count_b = bins[b];
            }
            count += count_a * count_b;
        }
    }

    os << count << "\n";
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
