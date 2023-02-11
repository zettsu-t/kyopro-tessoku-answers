// https://atcoder.jp/contests/tessoku-book/submissions/38827900
#include <algorithm>
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num l {0};
    is >> n >> l;

    Num max_distance {0};
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        std::string b;
        is >> a >> b;

        Num distance = 0;
        if (b == "W") {
            distance = a;
        } else {
            distance = l - a;
        }
        max_distance = std::max(max_distance, distance);
    }

    os << max_distance << "\n";
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
