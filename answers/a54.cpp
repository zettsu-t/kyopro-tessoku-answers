// https://atcoder.jp/contests/tessoku-book/submissions/38829398
#include <iostream>
#include <string>
#include <unordered_map>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    std::unordered_map<std::string, Num> scores;
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            std::string name;
            Num x {0};
            is >> name >> x;
            scores[name] = x;
        } else if (c == 2) {
            std::string name;
            is >> name;
            auto it = scores.find(name);
            if (it != scores.end()) {
                os << it->second << "\n";
            }
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
