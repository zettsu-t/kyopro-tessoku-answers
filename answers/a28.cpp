// https://atcoder.jp/contests/tessoku-book/submissions/38774762
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    constexpr Num base {10000};
    Num acc {0};

    for(decltype(n) i{0}; i<n; ++i) {
        std::string op;
        Num a {0};
        is >> op;
        is >> a;

        if (op == "+") {
            acc = (acc + (a % base)) % base;
        } else if (op == "-") {
            acc = (acc + base - (a % base)) % base;
        } else if (op == "*") {
            acc = (acc * (a % base)) % base;
        }
        os << acc << "\n";
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
