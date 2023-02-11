// https://atcoder.jp/contests/tessoku-book/submissions/38775297
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    Num n_3 = n / 3;
    Num n_5 = n / 5;
    Num n_15 = n / 15;
    Num count = n_3 + n_5 - n_15;
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
