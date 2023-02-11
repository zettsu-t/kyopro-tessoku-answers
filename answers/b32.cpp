// https://atcoder.jp/contests/tessoku-book/submissions/38775513
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

std::vector<bool> solve_detail(Num n, std::vector<Num>& choises) {
    std::vector<bool> first(n+1, false);

    for(decltype(n) i{0}; i<=n; ++i) {
        bool win_first = false;
        for(const auto& c : choises) {
            if (i == c) {
                win_first = true;
                break;
            }
            if (i > c) {
                win_first |= !first.at(i - c);
            }
        }
        first.at(i) = win_first;
    }

    return first;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num k {0};
    is >> n >> k;

    std::vector<Num> choises(k, 0);
    for(decltype(k) i{0}; i<k; ++i) {
        Num a {0};
        is >> a;
        choises.at(i) = a;
    }

    const auto first = solve_detail(n, choises);
    if (first.at(n)) {
        os << "First\n";
    } else {
        os << "Second\n";
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
