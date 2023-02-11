// https://atcoder.jp/contests/tessoku-book/submissions/38827289
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num d {0};
    size_t n {0};
    is >> d >> n;

    struct Span {
        Num left {0};
        Num right {0};
        Num limit {0};
    };

    struct {
        bool operator()(const Span& lhs, const Span& rhs) const {
            return std::tie(lhs.left, lhs.right, lhs.limit) < std::tie(rhs.left, rhs.right, rhs.limit);
        }
    } lessSpan;

    std::vector<Span> spans(n);
    for(decltype(n) i{0}; i<n; ++i) {
        Num l {0};
        Num r {0};
        Num h {0};
        is >> l >> r >> h;
        spans.at(i) = Span{l, r, h};
    }
    std::sort(spans.begin(), spans.end(), lessSpan);

    std::vector<Num> hours(d, 24);
    for(const auto& span : spans) {
        for(auto i = span.left; i <= span.right; ++i) {
            hours.at(i-1) = std::min(hours.at(i-1), span.limit);
        }
    }

    const auto sum = std::accumulate(hours.begin(), hours.end(), 0);
    os << sum << "\n";
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
