// https://atcoder.jp/contests/tessoku-book/submissions/38735004
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = int;
    size_t n {0};
    is >> n;

    std::vector<Num> values;
    for(size_t i=0; i<n; ++i) {
        Num a {0};
        is >> a;
        values.push_back(a);
    }
    std::sort(values.begin(), values.end());

    constexpr Num total = 1000;
    for(auto it_first = values.begin(); it_first != values.end(); ++it_first) {
        auto remaining = total - *it_first;
        auto it_next = it_first;
        ++it_next;

        auto it_right = std::upper_bound(it_next, values.end(), remaining);
        for(auto it_second = it_next; it_second != it_right; ++it_second) {
            auto target = remaining - *it_second;
            auto it_left = it_second;
            ++it_left;
            auto it_third = std::find(it_left, values.end(), target);
            if ((it_third != values.end()) && (*it_third == target)) {
                os << "Yes\n";
                return;
            }
        }
    }

    os << "No\n";
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
