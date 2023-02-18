// https://atcoder.jp/contests/tessoku-book/submissions/38958331
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

Num count(const std::vector<Num>& votes, Num border) {
    Num total{0};
    for(const auto& vote : votes) {
        total += vote / border;
    }
    return total;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num k {0};
    is >> n >> k;
    constexpr Num base = 10000000LL;

    Num total_votes{0};
    std::vector<Num> votes(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        a *= base;
        votes.at(i) = a;
        total_votes += a;
    }

    Num left {1};
    Num right {1000000000LL};
    right *= base;
    while(left <= right) {
        const auto center = (left + right) / 2;
        const auto seats = count(votes, center);
        if (seats == k) {
            left = center;
            break;
        } else if (seats > k) {
            left = center + 1;
        } else {
            right = center - 1;
        }
    }

    for(decltype(n) i{0}; i<n; ++i) {
        const auto seat = votes.at(i) / left;
        os << seat;
        if ((i + 1) < n) {
            os << " ";
        } else {
            os << "\n";
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
