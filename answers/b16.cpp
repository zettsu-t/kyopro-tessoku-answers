// https://atcoder.jp/contests/tessoku-book/submissions/38768461
#include <cmath>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> heights(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num h {0};
        is >> h;
        heights.at(i) = h;
    }

    std::vector<Num> costs(n, 0);
    costs.at(1) = std::abs(heights.at(1) - heights.at(0));
    for (decltype(n) i{2}; i<n; ++i) {
        costs.at(i) = std::min(
            costs.at(i-1) + std::abs(heights.at(i) - heights.at(i-1)),
            costs.at(i-2) + std::abs(heights.at(i) - heights.at(i-2)));
    }

    os << costs.at(n-1) << "\n";
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
