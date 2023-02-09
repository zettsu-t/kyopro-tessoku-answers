// https://atcoder.jp/contests/tessoku-book/submissions/38744014
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num d {0};
    size_t n {0};
    is >> d >> n;

    std::vector<Num> starts(d+1, 0);
    std::vector<Num> ends(d+1, 0);

    for (decltype(n) i{1}; i<=n; ++i) {
        Num from {0};
        Num to {0};
        is >> from >> to;
        starts.at(from) += 1;
        ends.at(to) += 1;
    }

    Num n_attendees {0};
    for (decltype(d) i{1}; i<=d; ++i) {
        n_attendees += starts.at(i) - ends.at(i-1);
        os << n_attendees << "\n";
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
