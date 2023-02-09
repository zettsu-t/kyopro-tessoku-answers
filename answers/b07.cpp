// https://atcoder.jp/contests/tessoku-book/submissions/38744056
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num t {0};
    Num n {0};
    is >> t >> n;

    std::vector<Num> starts(t + 1, 0);
    std::vector<Num> ends(t + 1, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num from {0};
        Num to {0};
        is >> from >> to;
        starts.at(from) += 1;
        ends.at(to) += 1;
    }

    Num n_employees {0};
    for (decltype(t) i{0}; i<t; ++i) {
        n_employees += starts.at(i) - ends.at(i);
        os << n_employees << "\n";
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
