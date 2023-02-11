// https://atcoder.jp/contests/tessoku-book/submissions/38827248
#include <iostream>

namespace {
    using Num = long long int;
}

Num sum_digits(Num n) {
    Num sum {0};
    Num rest {n};
    while(rest > 0) {
        sum += rest % 10;
        rest /= 10;
    }
    return sum;
}

Num accum_numbers(Num n) {
    return (n * (n + 1LL)) / 2LL;
}

Num solve_naive(Num n) {
    Num sum {0};
    for(decltype(n) i{1}; i<=n; ++i) {
        sum += sum_digits(i);
    };
    return sum;
}

Num solve_detail(Num n) {
    Num index {1};
    Num total {0};
    Num rest {n};
    while(rest > 0) {
        const Num next_index = index * 10LL;
        const Num digit= (n % next_index) / index;
        const Num n_more = accum_numbers(9) * ((n / next_index) * index);
        const Num n_less = accum_numbers(std::max(0LL, digit - 1LL)) * index;
        const Num n_equal = digit * ((n % index) + 1);
        total += n_more + n_less + n_equal;
        index = next_index;
        rest /= 10;
    }

    return total;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;
    const auto total = solve_detail(n);
    os << total << "\n";
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
