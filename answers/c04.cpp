// https://atcoder.jp/contests/tessoku-book/submissions/38943965
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> factors;
    Num x {1};
    while((x * x) <= n) {
        if ((n % x) == 0) {
            os << x << "\n";
            factors.push_back(x);
        }
        ++x;
    }

    for(auto it = factors.rbegin(); it != factors.rend(); ++it) {
        const auto y = *it;
        if ((y * y) < n) {
            os << (n / y) << "\n";
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
