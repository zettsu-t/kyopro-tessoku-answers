// https://atcoder.jp/contests/tessoku-book/submissions/38777578
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n{0};
    is >> n;

    std::vector<Num> score(n+1, -1000000000LL);
    std::vector<Num> trans_a(n, 0);
    std::vector<Num> trans_b(n, 0);
    score.at(1) = 0;

    for(decltype(n) i{1}; i<n; ++i) {
        Num a {0};
        is >> a;
        trans_a.at(i) = a;
    }

    for(decltype(n) i{1}; i<n; ++i) {
        Num b {0};
        is >> b;
        trans_b.at(i) = b;
    }

    for(decltype(n) i{1}; i<n; ++i) {
        const auto a = trans_a.at(i);
        const auto b = trans_b.at(i);
        if (a == b) {
            score.at(b) = std::max(score.at(b), score.at(i) + 150LL);
        } else {
            score.at(a) = std::max(score.at(a), score.at(i) + 100LL);
            score.at(b) = std::max(score.at(b), score.at(i) + 150LL);
        }
    }

    os << score.at(n) << "\n";
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
