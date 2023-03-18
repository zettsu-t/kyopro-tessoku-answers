// https://atcoder.jp/contests/tessoku-book/submissions/39807550
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
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

    using SubEdges = std::unordered_map<Num, Edge>;
    using Edges = std::unordered_map<Num, SubEdges>;
    using Distances = std::vector<Num>;
}

void traverse(Edges& edges, Distances& distances, Num from) {
    std::priority_queue<Node> candidates;
    candidates.push(Node{from, 0});
    distances.at(from) = 0;

    while(!candidates.empty()) {
        const auto node = candidates.top();
        candidates.pop();
        const auto current = node.id;

        auto its_edge = edges.find(current);
        if (its_edge != edges.end()) {
            auto& sub_edges = its_edge->second;

            for(auto it = sub_edges.begin(); it != sub_edges.end();) {
                const auto next = it->second.node;
                const auto dist = distances.at(current) + it->second.weight;
                if (distances.at(next) > dist) {
                    distances.at(next) = dist;
                    const Node next_node {next, dist};
                    candidates.push(next_node);
                    it = sub_edges.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    return;
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

        if (edges.find(a) == edges.end()) {
            SubEdges sub_edges;
            edges[a] = sub_edges;
        }
        edges[a][b] = Edge{b, c};

        if (edges.find(b) == edges.end()) {
            SubEdges sub_edges;
            edges[b] = sub_edges;
        }
        edges[b][a] = Edge{a, c};
    }

    Edges edges_forward = edges;
    Edges edges_reverse = edges;
    Distances distances_forward(n+1, inf);
    Distances distances_reverse(n+1, inf);
    traverse(edges_forward, distances_forward, 1);
    traverse(edges_reverse, distances_reverse, n);

    Num count{0};
    for(Num i=1; i<=n; ++i) {
        if ((distances_forward.at(i) + distances_reverse.at(i)) == distances_forward.at(n)) {
            ++count;
        }
    }

    os << count << "\n";
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
