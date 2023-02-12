// https://atcoder.jp/contests/tessoku-book/submissions/38831920
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

    for(decltype(n) i{1}; i<=n; ++i) {
        os << i << ": {";
        auto its = edges.equal_range(i);

        std::vector<Num> nodes;
        for(auto it = its.first; it != its.second; ++it) {
            nodes.push_back(it->second);
        }
        std::sort(nodes.begin(), nodes.end());

        for(size_t i{0}; i<nodes.size(); ++i) {
            os << nodes.at(i);
            if ((i+1) < nodes.size()) {
                os << ", ";
            }
        }
        os << "}\n";
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
