// https://atcoder.jp/contests/tessoku-book/submissions/38832340
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    using Edges = std::multimap<Num, Num>;
    using Visited = std::vector<Num>;
}

void print(const Visited& path, std::ostream& os) {
    auto size = path.size();
    for(decltype(size) i{0}; i<size; ++i) {
        os << path.at(i);
        if ((i+1) < path.size()) {
            os << " ";
        }
    }
    os << "\n";
}

Visited visit_dfs(const Edges& edges, Visited& visited, Num node, Num target) {
    if (node == target) {
        Visited path {target};
        return path;
    }

    auto its = edges.equal_range(node);
    for(auto it = its.first; it != its.second; ++it) {
        auto next = it->second;
        if (!visited.at(next)) {
            visited.at(next) = visited.at(node) + 1;
            auto path = visit_dfs(edges, visited, next, target);
            if (!path.empty()) {
                path.push_back(node);
                return path;
            }
        }
    }

    Visited path;
    return path;
}

Visited visit(const Edges& edges, Visited& visited, Num node, Num target) {
    visited.at(1) = 1;
    return visit_dfs(edges, visited, node, target);
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
    auto path = visit(edges, visited, 1, n);
    std::reverse(path.begin(), path.end());
    print(path, os);
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
