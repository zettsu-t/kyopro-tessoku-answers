// https://atcoder.jp/contests/tessoku-book/submissions/38832743
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    using Edges = std::multimap<Num, Num>;
    using Count = std::vector<Num>;
}

Num visit_dfs(const Edges& edges, Count& counts, Num node) {
    if (edges.count(node) == 0) {
        return 0;
    }

    auto its = edges.equal_range(node);
    Num count {0};
    for(auto it = its.first; it != its.second; ++it) {
        count += 1 + visit_dfs(edges, counts, it->second);
    }
    counts.at(node) = count;
    return count;
}

void visit(const Edges& edges, Count& counts) {
    visit_dfs(edges, counts, 1);
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;

    Edges edges;
    for(decltype(n) i{2}; i<=n; ++i) {
        Num a {0};
        is >> a;
        edges.insert(std::make_pair(a, i));
    }

    Count counts(n+1, 0);
    visit(edges, counts);

    for(decltype(n) i{1}; i<=n; ++i) {
        os << counts.at(i);
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
