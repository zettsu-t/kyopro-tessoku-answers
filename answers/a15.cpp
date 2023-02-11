// https://atcoder.jp/contests/tessoku-book/submissions/38765626
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    struct Cell {
        Num value {0};
        size_t position {0};
        Num rank {1};
    };

    struct {
        bool operator()(const Cell& a, const Cell& b) const {
            return (a.value < b.value);
        }
    } lessValue;

    struct {
        bool operator()(const Cell& a, const Cell& b) const {
            return (a.position < b.position);
        }
    } lessPosition;

    std::vector<Cell> cells(n, Cell{});
    for (decltype(n) position{0}; position<n; ++position) {
        Num value {0};
        is >> value;
        cells.at(position) = Cell{value, position, 1};
    }
    std::sort(cells.begin(), cells.end(), lessValue);

    for (decltype(n) i{1}; i<n; ++i) {
        cells.at(i).rank = cells.at(i-1).rank + (cells.at(i).value > cells.at(i-1).value);
    }
    std::sort(cells.begin(), cells.end(), lessPosition);

    for (decltype(n) i{0}; i<n; ++i) {
        os << cells.at(i).rank;
        if ((i + 1) < n) {
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
