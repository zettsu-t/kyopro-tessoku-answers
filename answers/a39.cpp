// https://atcoder.jp/contests/tessoku-book/submissions/38827378
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    struct Span {
        Num left {0};
        Num right {0};
    };

    struct {
        bool operator()(const Span& lhs, const Span& rhs) const {
            return std::tie(lhs.right, lhs.left) < std::tie(rhs.right, rhs.left);
        }
    } lessSpan;

    std::vector<Span> spans(n);
    for(decltype(n) i{0}; i<n; ++i) {
        Num l {0};
        Num r {0};
        is >> l >> r;
        spans.at(i) = Span{l, r};
    }
    std::sort(spans.begin(), spans.end(), lessSpan);

    Num current {-1};
    Num count {0};
    for(const auto& span : spans) {
        if (current <= span.left) {
            current = span.right;
            ++count;
        }
    }

    os << count << "\n";
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
