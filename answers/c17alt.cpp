// https://atcoder.jp/contests/tessoku-book/submissions/39311200
#include <deque>
#include <iostream>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    using Name = std::string;
    using Line = std::deque<Name>;

    size_t q {0};
    is >> q;

    Line top;
    Line bottom;

    for(decltype(q) i{0}; i<q; ++i) {
        std::string c;
        is >> c;

        if (c == "A") {
            Name x;
            is >> x;
            bottom.push_back(x);
            if (top.size() < bottom.size()) {
                top.push_back(bottom.front());
                bottom.pop_front();
            }
        } else if (c == "B") {
            Name x;
            is >> x;
            if (top.size() == bottom.size()) {
                top.push_back(x);
            } else {
                bottom.push_front(x);
            }
        } else if (c == "C") {
            top.pop_front();
            if (top.size() < bottom.size()) {
                top.push_back(bottom.front());
                bottom.pop_front();
            }
        } else {
            os << top.front() << "\n";
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
