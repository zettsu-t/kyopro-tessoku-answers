// https://atcoder.jp/contests/tessoku-book/submissions/38828062
#include <iostream>

namespace {
    using Num = long long int;
}

Num to_num(char c) {
    Num num{0};
    switch(c) {
    case 'W':
        num = 0;
        break;
    case 'B':
        num = 1;
        break;
    case 'R':
        num = 2;
        break;
    default:
        break;
    }

    return num;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    std::string c {0};
    std::string seq {0};
    is >> n >> c >> seq;

    const Num target = to_num(c.at(0));
    Num sum {0};
    for(const auto& e : seq) {
        sum += to_num(e);
    }

    if (target == (sum % 3)) {
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
