// https://atcoder.jp/contests/tessoku-book/submissions/38861423
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    struct Box {
        Num x {0};
        Num y {0};
    };
    std::vector<Box> boxes(n, Box{});

    struct {
        bool operator()(const Box& lhs, const Box& rhs) const {
            return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
        }
    } lessX;

    for (decltype(n) i{0}; i<n; ++i) {
        Num x {0};
        Num y {0};
        is >> x >> y;
        boxes.at(i) = Box{x, -y};
    }
    std::sort(boxes.begin(), boxes.end(), lessX);

    std::vector<Num> ys {};
    for(const auto& box : boxes) {
        ys.push_back(-box.y);
    }

    std::vector<Num> dps;
    std::vector<Num> lens;
    dps.push_back(0);
    lens.push_back(ys.at(0));

    for (decltype(n) i{1}; i<n; ++i) {
        const auto y = ys.at(i);
        const auto pos = std::distance(lens.begin(), std::lower_bound(lens.begin(), lens.end(), y));
        dps.push_back(pos);
        if (pos >= static_cast<Num>(lens.size())) {
            lens.push_back(y);
        } else {
            lens.at(pos) = y;
        }
    }

    os << lens.size() << "\n";
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
