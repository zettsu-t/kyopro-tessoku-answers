// https://atcoder.jp/contests/tessoku-book/submissions/38832519
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

void visit(const Edges& edges, Visited& visited, Num start) {
    std::queue<Num> candidates;
    candidates.push(start);
    visited.at(start) = 1;

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
    Num r {0};
    Num c {0};
    Num sy {0};
    Num sx {0};
    Num gy {0};
    Num gx {0};
    is >> r >> c >> sy >> sx >> gy >> gx;
    --sy;
    --sx;
    --gy;
    --gx;

    Edges edges;
    Visited visited(r * c, 0);

    std::string top_wall;
    is >> top_wall;

    for(decltype(r) y{1}; y<r; ++y) {
        std::string s;
        is >> s;
        for(decltype(c) x{1}; x<c; ++x) {
            const Num pos = y * c + x;
            const Num left = pos - 1;
            const Num top = pos - c;

            visited.at(pos) = (s.at(x) == '#') ? -1 : 0;
            if ((visited.at(pos) == 0) && (visited.at(left) == 0)) {
                edges.insert(std::make_pair(left, pos));
                edges.insert(std::make_pair(pos, left));
            }

            if ((visited.at(pos) == 0) && (visited.at(top) == 0)) {
                edges.insert(std::make_pair(top, pos));
                edges.insert(std::make_pair(pos, top));
            }
        }
    }

    visit(edges, visited, sx + sy * c);
    os << (visited.at(gx + gy * c) - 1) << "\n";
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
