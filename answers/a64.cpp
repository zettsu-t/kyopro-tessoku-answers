// https://atcoder.jp/contests/tessoku-book/submissions/38861294
#include <iostream>
#include <queue>
#include <map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    constexpr Num inf = 100000000000000LL;

    struct Node {
        Num id {0};
        Num weight {0};
        bool operator<(const Node& rhs) const {
            return std::tie(weight, id) > std::tie(rhs.weight, rhs.id);
        }
    };

    struct Edge {
        Num node {0};
        Num weight {0};
    };

    using Edges = std::multimap<Num, Edge>;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num m {0};
    is >> n >> m;

    Edges edges;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        Num c {0};
        is >> a >> b >> c;

        edges.insert(std::make_pair(a, Edge{b, c}));
        edges.insert(std::make_pair(b, Edge{a, c}));
    }

    std::vector<Num> distances(n+1, inf);
    std::priority_queue<Node> candidates;
    candidates.push(Node{1, 0});
    distances.at(1) = 0;

    while(!candidates.empty()) {
        const auto node = candidates.top();
        candidates.pop();
        const auto current = node.id;

        auto its = edges.equal_range(current);
        for(auto it = its.first; it != its.second;) {
            const auto next = it->second.node;
            const auto dist = distances.at(current) + it->second.weight;
            if (distances.at(next) > dist) {
                distances.at(next) = dist;
                const Node next_node {next, dist};
                candidates.push(next_node);
                it = edges.erase(it);
            } else {
                ++it;
            }
        }
    }

    for(decltype(n) i{1}; i<=n; ++i) {
        const auto distance = distances.at(i);
        const auto len = (distance == inf) ? -1 : distance;
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
