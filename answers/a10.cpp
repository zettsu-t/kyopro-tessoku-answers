// https://atcoder.jp/contests/tessoku-book/submissions/38744479
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> rooms(n+2, 0);
    for (decltype(n) i=1; i<=n; ++i) {
        Num a {0};
        is >> a;
        rooms.at(i) = a;
    }

    decltype(rooms) max_left(n+2, 0);
    for (decltype(n) i=1; i<=n; ++i) {
        max_left.at(i) = std::max(max_left.at(i-1), rooms.at(i));
    }

    decltype(rooms) max_right(n+2, 0);
    for (decltype(n) i=n; i>0; --i) {
        max_right.at(i) = std::max(max_right.at(i+1), rooms.at(i));
    }

    size_t d {0};
    is >> d;
    for (decltype(d) i=0; i<d; ++i) {
        Num left {0};
        Num right {0};
        is >> left >> right;
        const auto capacity = std::max(max_left.at(left - 1), max_right.at(right + 1));
        os << capacity << "\n";
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
