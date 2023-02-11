// https://atcoder.jp/contests/tessoku-book/submissions/38774223
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    for(decltype(q) i{0}; i<q; ++i) {
        Num n{0};
        is >> n;
        Num p = 2;
        bool prime {true};
        while((p*p) <= n) {
            if ((n % p) == 0) {
                prime = false;
                break;
            }
            ++p;
        }

        if (prime) {
            os << "Yes\n";
        } else {
            os << "No\n";
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
