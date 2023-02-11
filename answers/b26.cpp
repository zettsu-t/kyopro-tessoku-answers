// https://atcoder.jp/contests/tessoku-book/submissions/38774367
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector non_primes(n+1, 0);
    non_primes.at(0) = 1;
    non_primes.at(1) = 1;
    Num p = 1;

    for(;;) {
        while(non_primes.at(++p));
        for(Num i=p+p; i<=n; i+=p) {
            non_primes.at(i) = 1;
        }

        if ((p*p) > n) {
            break;
        }
    }

    for(Num i=2; i<=n; ++i) {
        if (!non_primes.at(i)) {
            os << i << "\n";
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
