// https://atcoder.jp/contests/tessoku-book/submissions/38861440
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    std::string s;
    is >> n >> s;
    std::string str{" "};
    str += s;
    str += "  ";

    std::vector<Num> heights_a(n, 0);
    for(decltype(n) i{1}; i<=(n-1); ++i) {
        const auto prev = str.at(i-1);
        const auto current = str.at(i);
        if (current == 'A') {
            if (prev == 'A') {
                heights_a.at(i) = heights_a.at(i-1) + 1;
            } else {
                heights_a.at(i) = 2;
                heights_a.at(i-1) = 1;
            }
        }
    }

    std::vector<Num> heights_b(n, 0);
    for(decltype(n) i{n-2}; i>=0; --i) {
        const auto prev = str.at(i+2);
        const auto current = str.at(i+1);
        if (current == 'B') {
            if (prev == 'B') {
                heights_b.at(i) = heights_b.at(i+1) + 1;
            } else {
                heights_b.at(i) = 2;
                heights_b.at(i+1) = 1;
            }
        }
    }

    Num total{0};
    for(decltype(n) i{0}; i<n; ++i) {
        const auto a = std::max(heights_a.at(i), heights_b.at(i));
        total += a;
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
