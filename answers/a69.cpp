// https://atcoder.jp/contests/tessoku-book/submissions/38887525
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Network {
    using Num = long long int;
    using Path = std::vector<Num>;
    using Visited = std::vector<bool>;

    struct Reached {
        bool target {false};
        Path path;
    };

    using Edges = std::unordered_map<Num, std::unordered_map<Num, Num>>;

    Reached visit_dfs(Visited& visited, Num from, Num sink) {
        if (from == sink) {
            Reached reached {true, Path{sink}};
            return reached;
        }

        auto it_map = all_edges_.find(from);
        if (it_map == all_edges_.end()) {
            Reached unreachable;
            return unreachable;
        }

        Reached reached;
        for(auto it = it_map->second.begin(); it != it_map->second.end(); ++it) {
            const auto to = it->first;
            if (visited.at(to) || (it->second <= 0)) {
                continue;
            }

            const auto prev_visited = visited.at(to);
            visited.at(to) = true;
            auto deep_reached = visit_dfs(visited, to, sink);
            if (deep_reached.target) {
                deep_reached.path.push_back(from);
                std::swap(reached, deep_reached);
                break;
            }
            visited.at(to) = prev_visited;
        }

        return reached;
    }

    Num visit(Num source, Num sink) {
        Num total {0};

        for(;;) {
            Visited visited(sink+1, false);
            visited.at(source) = true;
            auto reached = visit_dfs(visited, source, sink);
            if (!reached.target) {
                break;
            }

            Num min_flow {0};
            for(size_t i=1; i<reached.path.size(); ++i) {
                const auto sink_side = reached.path.at(i-1);
                const auto source_side = reached.path.at(i);
                const auto max_flow = all_edges_[source_side][sink_side];
                if (i == 1) {
                    min_flow = max_flow;
                } else {
                    min_flow = std::min(min_flow, max_flow);
                }
            }

            for(size_t i=1; i<reached.path.size(); ++i) {
                const auto sink_side = reached.path.at(i-1);
                const auto source_side = reached.path.at(i);
                all_edges_[source_side][sink_side] -= min_flow;
                all_edges_[sink_side][source_side] += min_flow;
            }

            total += min_flow;
        }

        return total;
    }

    void add_edge(Num from, Num to, Num max_flow) {
        auto it = all_edges_.find(from);
        if (it == all_edges_.end()) {
            std::unordered_map<Num, Num> new_edges;
            new_edges[to] = max_flow;
            all_edges_[from] = new_edges;
        } else {
            auto it_to = it->second.find(to);
            if (it_to == it->second.end()) {
                it->second[to] = max_flow;
            } else {
                it->second[to] = std::max(it->second[to], max_flow);
            }
        }
    }

    void add(Num from, Num to, Num max_flow) {
        if (max_flow > 0) {
            add_edge(from, to, max_flow);
            add_edge(to, from, 0);
        }
    }

    Edges all_edges_;
};

void solve(std::istream& is, std::ostream& os) {
    using Num = Network::Num;
    Num n {0};
    is >> n;

    const Num source = 2 * n + 1;
    const Num sink = 2 * n + 2;
    Network network;

    for(decltype(n) i{1}; i<=n; ++i) {
        std::string s;
        is >> s;
        for(decltype(n) j{1}; j<=n; ++j) {
            if (s.at(j-1) == '#') {
                const Num from = i;
                const Num to = n + j;
                network.add(from, to, 1);
                network.add(source, from, 1);
                network.add(to, sink, 1);
            }
        }
    }

    const auto max_flow = network.visit(source, sink);
    os << max_flow << "\n";
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
