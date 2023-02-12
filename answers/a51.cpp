// https://atcoder.jp/contests/tessoku-book/submissions/38829303
#include <iostream>
#include <stack>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    std::stack<std::string> books;
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            std::string x;
            is >> x;
            books.push(x);
        } else if (c == 2) {
            os << books.top() << "\n";
        } else if (c == 3) {
            books.pop();
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
