// https://atcoder.jp/contests/tessoku-book/submissions/38944225
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;
    --n;

    size_t n_bits = 10;
    std::vector<Num> digits;
    for(decltype(n_bits) i{0}; i<n_bits; ++i) {
        const auto digit = ((n & 1) > 0) ? 7 : 4;
        digits.push_back(digit);
        n >>= 1;
    }

    for(auto it = digits.rbegin(); it != digits.rend(); ++it) {
        os << *it;
    }
    os << "\n";

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
