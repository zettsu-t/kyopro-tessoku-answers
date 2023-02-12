// https://atcoder.jp/contests/tessoku-book/submissions/38829300
#include <iostream>
#include <queue>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    std::queue<std::string> persons;
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            std::string x;
            is >> x;
            persons.push(x);
        } else if (c == 2) {
            os << persons.front() << "\n";
        } else if (c == 3) {
            persons.pop();
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
