// https://atcoder.jp/contests/tessoku-book/submissions/38827196
#include <iostream>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    std::string s {0};
    is >> n >> k >> s;

    Num n_on {0};
    for(const auto& c : s) {
        n_on += (c != '0');
    }

    if (((k ^ n_on) & 1) == 0) {
        os << "Yes\n";
    } else {
        os << "No\n";
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
