// https://atcoder.jp/contests/tessoku-book/submissions/38778080
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    is >> n >> k;

    std::vector<Num> cumsum(n+1, 0LL);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        cumsum.at(i+1) = cumsum.at(i) + a;
    }

    Num count = 0;
    Num left = 0;
    Num right = 0;
    while(left < n) {
        while((right < n) && ((cumsum.at(right+1) - cumsum.at(left)) <= k)) {
            ++right;
        }
        count += right - left;
        ++left;
    }

    os << count << "\n";
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
