// https://atcoder.jp/contests/tessoku-book/submissions/38832143
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num m {0};
    is >> n >> m;

    std::multimap<Num, Num> edges;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        edges.insert(std::make_pair(a, b));
        edges.insert(std::make_pair(b, a));
    }

    std::vector<Num> counts(n+1, 0);
    for(decltype(n) i{1}; i<=n; ++i) {
        counts.at(i) = edges.count(i);
    }

    auto it = std::max_element(counts.begin(), counts.end());
    os << (it - counts.begin()) << "\n";
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
