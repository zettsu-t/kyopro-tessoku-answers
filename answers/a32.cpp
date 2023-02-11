// https://atcoder.jp/contests/tessoku-book/submissions/38775404
#include <iostream>

namespace {
    using Num = long long int;
}

bool solve_detail(Num n, Num a, Num b) {
    Num small = a;
    Num big = b;
    if (small > big) {
        std::swap(small, big);
    }

    const Num r = (n % (small + big));
    Num first = false;
    if (r < big) {
        first = ((r / small) & 1) > 0;
    } else {
        first = true;
    }

    return first;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num a {0};
    Num b {0};
    is >> n >> a >> b;

    const auto first = solve_detail(n, a, b);
    if (first) {
        os << "First\n";
    } else {
        os << "Second\n";
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
