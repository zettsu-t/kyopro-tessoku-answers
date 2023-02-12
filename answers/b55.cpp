// https://atcoder.jp/contests/tessoku-book/submissions/38829543
#include <iostream>
#include <set>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    std::set<Num> nums;
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        Num x {0};
        is >> c >> x;
        if (c == 1) {
            nums.insert(x);
        } else if (c == 2) {
            auto it_equal_more = nums.lower_bound(x);
            auto it_less = nums.end();
            if (it_equal_more != nums.begin()) {
                it_less = it_equal_more;
                --it_less;
            }

            if ((it_equal_more != nums.end()) && (it_less != nums.end())) {
                os << std::min(*it_equal_more - x, x - *it_less) << "\n";
            } else if (it_equal_more != nums.end()) {
                os << (*it_equal_more - x) << "\n";
            } else if (it_less != nums.end()) {
                os << (x - *it_less) << "\n";
            } else {
                os << "-1\n";
            }
        }
    }

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
