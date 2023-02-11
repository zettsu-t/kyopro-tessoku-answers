// https://atcoder.jp/contests/tessoku-book/submissions/38765451
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    Num k {0};
    is >> n >> k;

    std::vector<Num> a(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        is >> x;
        a.at(i) = x;
    }

    std::vector<Num> b(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        is >> x;
        b.at(i) = x;
    }

    std::vector<Num> c(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        is >> x;
        c.at(i) = x;
    }

    std::vector<Num> d(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        is >> x;
        d.at(i) = x;
    }

    std::vector<Num> ab;
    ab.reserve(n*n);
    for (decltype(n) i{0}; i<n; ++i) {
        for (decltype(n) j{0}; j<n; ++j) {
            ab.push_back(a.at(i) + b.at(j));
        }
    }
    std::sort(ab.begin(), ab.end());

    std::vector<Num> cd;
    cd.reserve(n*n);
    for (decltype(n) i{0}; i<n; ++i) {
        for (decltype(n) j{0}; j<n; ++j) {
            cd.push_back(c.at(i) + d.at(j));
        }
    }
    std::sort(cd.begin(), cd.end());

    for(const auto& e : ab) {
        if (std::binary_search(cd.begin(), cd.end(), k - e)) {
            os << "Yes\n";
            return;
        }
    }

    os << "No\n";
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
