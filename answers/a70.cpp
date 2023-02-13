// https://atcoder.jp/contests/tessoku-book/submissions/38861308
#include <algorithm>
#include <iostream>
#include <queue>
#include <map>
#include <utility>
#include <vector>

namespace {
    using Num = long long int;
    constexpr Num inf = 1000000000000000LL;

    struct Node {
        Num id {0};
        Num weight {0};
        bool operator<(const Node& rhs) const {
            return std::tie(weight, id) > std::tie(rhs.weight, rhs.id);
        }
    };

    struct Edge {
        Num node {0};
        Num weight {1};
    };

    using Edges = std::multimap<Num, Edge>;
    using State = std::vector<Num>;
}

Num state_to_num_msb_first(const State& state) {
    Num num {0};
    auto size = state.size();
    for(decltype(size) i{0}; i<size; ++i) {
        num <<= 1;
        num += (state.at(i) & 1);
    }
    return num;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num m {0};
    is >> n >> m;

    State state(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        state.at(i) = a;
    }
    std::reverse(state.begin(), state.end());
    const Num initial_state = state_to_num_msb_first(state);

    std::vector<Num> trans(m, 0);
    for(decltype(m) i{0}; i<m; ++i) {
        Num num_state {0};
        for(int i=0; i<3; ++i) {
            Num a{0};
            is >> a;
            Num mask {1};
            num_state |= (mask << (a - 1));
        }
        trans.at(i) = num_state;
    }

    Num n_nodes {1};
    n_nodes <<= n;
    const Num goal_state = n_nodes - 1;

    Edges edges;
    for(decltype(n_nodes) from{0}; from<n_nodes; ++from) {
        auto size = trans.size();
        for(decltype(size) j{0}; j<size; ++j) {
            const Num to = from ^ trans.at(j);
            if (to != from) {
                edges.insert(std::make_pair(from, Edge{to, 1}));
                edges.insert(std::make_pair(to, Edge{from, 1}));
            }
        }
    }

    std::vector<Num> distances(n_nodes+1, inf);
    std::priority_queue<Node> candidates;
    candidates.push(Node{initial_state, 0});
    distances.at(initial_state) = 0;

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

    const auto distance = distances.at(goal_state);
    const auto len = (distance >= inf) ? -1 : distance;
    os << len << "\n";
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
