// https://atcoder.jp/contests/tessoku-book/submissions/38829004
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Numeric = double;
    constexpr Numeric inf = std::numeric_limits<Numeric>::max();

    Num n {0};
    is >> n;

    struct Position {
        Numeric x {0};
        Numeric y {0};
        Numeric distance(const Position& other) const {
            const Num diff_x = x - other.x;
            const Num diff_y = y - other.y;
            return diff_x * diff_x + diff_y * diff_y;
        }
    };

    std::vector<Num> visited;
    visited.reserve(n+1);
    std::set<Num> unvisited;
    std::vector<Position> cities(n);

    for(decltype(n) i{0}; i<n; ++i) {
        Numeric x {0.0};
        Numeric y {0.0};
        is >> x >> y;
        cities.at(i) = Position{x, y};
        unvisited.insert(i);
    }

    Num current {0};
    for(decltype(n) i{0}; i<n; ++i) {
        visited.push_back(current);
        unvisited.erase(current);

        Numeric min_dist = inf;
        Num next = current;
        for(const auto& e : unvisited) {
            const auto dist = cities.at(current).distance(cities.at(e));
            if (dist < min_dist) {
                next = e;
                min_dist = dist;
            }
        }
        current = next;
    }
    visited.push_back(0);

    current = visited.at(0);
    for(decltype(n) i{0}; i<n; ++i) {
        os << (current + 1) << "\n";
        current = visited.at(i+1);
    }

    os << (visited.at(n) + 1) << "\n";
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
