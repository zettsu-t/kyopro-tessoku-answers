// https://atcoder.jp/contests/tessoku-book/submissions/38829586
#include <iostream>
#include <unordered_map>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::unordered_map<Num, Num> counts;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        auto it = counts.find(a);
        if (it != counts.end()) {
            counts[a] += 1;
        } else {
            counts[a] = 1;
        }
    }

    Num total {0};
    for(const auto& e: counts) {
        const Num a = e.second;
        if (a > 1) {
            total += (a*(a-1)) / 2;
        }
    }

    os << total << "\n";
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
