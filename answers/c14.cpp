// https://atcoder.jp/contests/tessoku-book/submissions/38997719
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
    using Path = std::vector<Num>;
    using UniqueNodes = std::set<Num>;
}

void traverse_detail(std::multimap<Num, Num>& route, UniqueNodes& nodes, Path& path, Num from, Num to) {
    auto its = route.equal_range(from);
    for(auto it=its.first; it!=its.second;) {
        if (it == route.end()) {
            break;
        }

        const auto next = it->second;
        if (next == to) {
            for(const auto node : path) {
                nodes.insert(node);
            }
            nodes.insert(to);
            break;
        }

        path.push_back(next);
        it = route.erase(it);
        traverse_detail(route, nodes, path, next, to);
        path.pop_back();
    }

    return;
}

void traverse(std::multimap<Num, Num>& route, UniqueNodes& nodes, Num from, Num to) {
    Path path {from};
    traverse_detail(route, nodes, path, from, to);
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

    Edges edges_dup = edges;
    std::priority_queue<Node> candidates_dup;

    std::vector<Num> distances(n+1, inf);
    std::priority_queue<Node> candidates;
    candidates.push(Node{1, 0});
    distances.at(1) = 0;

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

    std::swap(edges_dup, edges);
    std::swap(candidates_dup, candidates);
    candidates.push(Node{1, 0});
    std::multimap<Num, Num> route;

    while(!candidates.empty()) {
        const auto node = candidates.top();
        candidates.pop();
        const auto current = node.id;

        auto its_edge = edges.find(current);
        if (its_edge == edges.end()) {
            continue;
        }

        std::vector<Num> erased_nodes;
        auto& sub_edges = its_edge->second;
        for(auto it = sub_edges.begin(); it != sub_edges.end();) {
            const auto next = it->second.node;
            const auto dist = distances.at(current) + it->second.weight;

            if (distances.at(next) == dist) {
                distances.at(next) = dist;
                const Node next_node {next, dist};
                route.insert(std::make_pair(next, current));
                candidates.push(next_node);
            }
            erased_nodes.push_back(next);
            it = sub_edges.erase(it);
        }

        for(const auto& next : erased_nodes) {
            auto it = edges.find(next);
            if (it == edges.end()) {
                continue;
            }

            auto& next_edges = it->second;
            auto it_erased = next_edges.find(current);
            if (it_erased != next_edges.end()) {
                next_edges.erase(current);
            }
        }
    }

    UniqueNodes nodes {n};
    traverse(route, nodes, n, 1);
    os << nodes.size() << "\n";
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
