// https://atcoder.jp/contests/tessoku-book/submissions/38775341
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    const Num n_3 = n / 3;
    const Num n_5 = n / 5;
    const Num n_7 = n / 7;
    const Num n_35 = n / 15;
    const Num n_37 = n / 21;
    const Num n_57 = n / 35;
    const Num n_357 = n / 105;
    const Num count = n_3 + n_5 + n_7 - n_35 - n_37 - n_57 + n_357;
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
