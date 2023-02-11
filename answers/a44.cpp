// https://atcoder.jp/contests/tessoku-book/submissions/38827956
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

Num get_index(Num index, Num size, bool reverse) {
    if (reverse) {
        return size - index;
    }
    return index - 1;
}

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    size_t q {0};
    is >> n >> q;

    std::vector<Num> nums(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        nums.at(i) = i + 1;
    }

    bool reverse = false;
    for(decltype(n) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            Num x {0};
            Num y {0};
            is >> x >> y;
            nums.at(get_index(x, n, reverse)) = y;
        } else if (c == 2) {
            reverse = !reverse;
        } else if (c == 3) {
            Num x {0};
            is >> x;
            os << nums.at(get_index(x, n, reverse)) << "\n";
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
