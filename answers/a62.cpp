// https://atcoder.jp/contests/tessoku-book/submissions/38832235
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    using Edges = std::multimap<Num, Num>;
    using Visited = std::vector<bool>;
}

void visit_dfs(const Edges& edges, Visited& visited, Num node) {
    auto its = edges.equal_range(node);
    for(auto it = its.first; it != its.second; ++it) {
        const auto next = it->second;
        if (!visited.at(next)) {
            visited.at(next) = true;
            visit_dfs(edges, visited, next);
        }
    }
}

void visit(const Edges& edges, Visited& visited, Num node) {
    visited.at(node) = true;
    visit_dfs(edges, visited, node);
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

    Visited visited(n + 1, false);
    visit(edges, visited, 1);
    if (std::all_of(visited.begin() + 1, visited.end(),
                    [](const auto& x) { return x; })) {
        os << "The graph is connected.\n";
    } else {
        os << "The graph is not connected.\n";
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
