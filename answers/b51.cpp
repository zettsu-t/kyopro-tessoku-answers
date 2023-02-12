// https://atcoder.jp/contests/tessoku-book/submissions/38829259
#include <iostream>
#include <stack>
#include <string>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    std::string s;
    is >> s;

    using Index = decltype(s)::size_type;
    struct Span {
        Index left {0};
        Index right {0};
    };

    std::stack<size_t> marks;
    std::vector<Span> spans;

    for(Index i=0; i<s.size(); ++i) {
        auto c = s.at(i);
        if (c == '(') {
            marks.push(i);
        } else if (c == ')') {
            const auto left = marks.top() + 1;
            const auto right = i + 1;
            marks.pop();
            spans.push_back(Span{left, right});
        }
    }

    for(const auto& span : spans) {
        os << span.left << " " << span.right << "\n";
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
