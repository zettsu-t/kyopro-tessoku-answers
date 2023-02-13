// https://atcoder.jp/contests/tessoku-book/submissions/38861584
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    using Edges = std::multimap<Num, Num>;
    using Ranks = std::vector<Num>;
    using Count = std::vector<Num>;
    using Visited = std::vector<bool>;
}

Num visit_dfs(const Edges& edges, Visited& visited, Ranks& ranks, Num node) {
    visited.at(node) = true;
    if (edges.count(node) == 0) {
        ranks.at(node) = 0;
        return 0;
    }

    Num rank {0};
    auto its = edges.equal_range(node);
    for(auto it = its.first; it != its.second; ++it) {
        const auto next = it->second;
        if (!visited.at(next)) {
            rank = std::max(rank, 1 + visit_dfs(edges, visited, ranks, next));
        }
    }
    ranks.at(node) = rank;
    return rank;
}

void visit(const Edges& edges, Visited& visited, Ranks& ranks, Num node) {
    visit_dfs(edges, visited, ranks, node);
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num t {0};
    is >> n >> t;

    Edges edges;
    for(decltype(n) i{2}; i<=n; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        edges.insert(std::make_pair(a, b));
        edges.insert(std::make_pair(b, a));
    }

    Count counts(n+1, 0);
    Visited visited(n+1, false);
    Ranks ranks(n+1, 0);
    visit(edges, visited, ranks, t);

    for(decltype(n) i{1}; i<=n; ++i) {
        os << ranks.at(i);
        if (i < n) {
            os << " ";
        }
    }
    os << "\n";

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
