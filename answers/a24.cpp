// https://atcoder.jp/contests/tessoku-book/submissions/38773473
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::vector<Num> nums;
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.push_back(a);
    }

    std::vector<Num> dps;
    std::vector<Num> lens;
    dps.push_back(0);
    lens.push_back(nums.at(0));

    for (decltype(n) i{1}; i<n; ++i) {
        const auto a = nums.at(i);
        const auto pos = std::distance(lens.begin(), std::lower_bound(lens.begin(), lens.end(), a));
        dps.push_back(pos);
        if (pos >= static_cast<Num>(lens.size())) {
            lens.push_back(a);
        } else {
            lens.at(pos) = a;
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
