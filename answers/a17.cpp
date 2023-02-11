// https://atcoder.jp/contests/tessoku-book/submissions/38768652
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> ones(n-1, 0);
    for (decltype(n) i{0}; i<(n-1); ++i) {
        Num a {0};
        is >> a;
        ones.at(i) = a;
    }

    std::vector<Num> twos(n-2, 0);
    for (decltype(n) i{0}; i<(n-2); ++i) {
        Num b {0};
        is >> b;
        twos.at(i) = b;
    }

    std::vector<Num> costs(n, 0);
    std::vector<Num> froms(n, 0);
    costs.at(1) = ones.at(0);
    froms.at(1) = 0;

    for (decltype(n) i{2}; i<=(n-1); ++i) {
        const auto cost_one = costs.at(i-1) + ones.at(i-1);
        const auto cost_two = costs.at(i-2) + twos.at(i-2);
        costs.at(i) = std::min(cost_one, cost_two);
        if (cost_one < cost_two) {
            froms.at(i) = i-1;
        } else {
            froms.at(i) = i-2;
        }
    }

    std::vector<size_t> route;
    size_t pos = n-1;
    do {
        route.push_back(pos);
        pos = froms.at(pos);
    } while(pos > 0);
    route.push_back(0);
    std::reverse(route.begin(), route.end());

    auto size = route.size();
    os << size << "\n";
    for (decltype(size) i{0}; i<size; ++i) {
        os << (route.at(i) + 1);
        if ((i + 1) < size) {
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
