// https://atcoder.jp/contests/tessoku-book/submissions/38832428
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    using Edges = std::multimap<Num, Num>;
    using Visited = std::vector<Num>;
}

void visit(const Edges& edges, Visited& visited) {
    std::queue<Num> candidates;
    candidates.push(1);
    visited.at(1) = 1;

    while(!candidates.empty()) {
        const auto current = candidates.front();
        candidates.pop();

        auto its = edges.equal_range(current);
        for(auto it = its.first; it != its.second; ++it) {
            const Num candidate = it->second;
            if (!visited.at(candidate)) {
                visited.at(candidate) = visited.at(current) + 1;
                candidates.push(candidate);
            }
        }
    }
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num m {0};
    is >> n >> m;

    Edges edges;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        edges.insert(std::make_pair(a, b));
        edges.insert(std::make_pair(b, a));
    }

    Visited visited(n + 1, 0);
    visit(edges, visited);
    for(decltype(n) i{1}; i<=n; ++i) {
        const auto distance = visited.at(i);
        const auto len = (distance > 0) ? (distance - 1) : -1;
        os << len << "\n";
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
