// https://atcoder.jp/contests/tessoku-book/submissions/38829505
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
            nums.erase(x);
        } else if (c == 3) {
            auto it = nums.lower_bound(x);
            const auto y = (it == nums.end()) ? -1 : *it;
            os << y << "\n";
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
