// https://atcoder.jp/contests/tessoku-book/submissions/38768851
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    Num s {0};
    is >> n >> s;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    std::set<Num> sums {0};
    for (decltype(n) i{0}; i<n; ++i) {
        std::vector<Num> added;
        for(const auto& e : sums) {
            added.push_back(e + nums.at(i));
        }
        for(const auto& e : added) {
            sums.insert(e);
        }
    }

    if (std::find(sums.begin(), sums.end(), s) != sums.end()) {
        os << "Yes\n";
    } else {
        os << "No\n";
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
