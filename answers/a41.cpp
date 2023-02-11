// https://atcoder.jp/contests/tessoku-book/submissions/38827623
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    constexpr char term = ' ';
    Num n {0};
    std::string s;
    is >> n >> s;
    s.push_back(term);

    auto prev {term};
    Num run_len {1};
    bool found {false};
    for(const auto& c : s) {
        if (c == prev) {
            ++run_len;
        } else {
            run_len = 1;
        }
        found |= (run_len >= 3);
        prev = c;
    }

    if (found) {
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
