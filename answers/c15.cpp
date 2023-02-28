// https://atcoder.jp/contests/tessoku-book/submissions/39312736
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    is >> n >> k;

    Num start_time = 0;
    Num end_time = (86400 + k) * 2;
    const Num n_ticks = end_time - start_time + 1;
    std::vector forward_count(n_ticks, 0);
    std::vector reverse_count(n_ticks, 0);

    struct Span {
        Num index {0};
        Num left {0};
        Num right {0};
    };

    struct {
        bool operator()(const Span& lhs, const Span& rhs) const {
            return std::tie(lhs.right, lhs.left, lhs.index) < std::tie(rhs.right, rhs.left, rhs.index);
        }
    } lessSpan;

    struct {
        bool operator()(const Span& lhs, const Span& rhs) const {
            return std::tie(lhs.left, lhs.right, lhs.index) > std::tie(rhs.left, rhs.right, rhs.index);
        }
    } moreSpan;

    std::vector<Span> spans(n);
    std::vector<Span> indexed_spans(n);
    for(decltype(n) i{0}; i<n; ++i) {
        Num l {0};
        Num r {0};
        is >> l >> r;
        const Span span{i, l*2, r*2+k*2};
        spans.at(i) = span;
        indexed_spans.at(i) = span;
    }

    std::sort(spans.begin(), spans.end(), lessSpan);
    Num current {-1};
    Num count {0};
    for(const auto& span : spans) {
        if (current <= span.left) {
            const auto next = span.right;
            for(auto t = current + 1LL; t < next; ++t) {
                forward_count.at(t) = count;
            }
            current = next - 1;
            ++count;
        }
    }

    for(auto t = current + 1LL; t <= end_time; ++t) {
        forward_count.at(t) = count;
    }

    std::sort(spans.begin(), spans.end(), moreSpan);
    current = end_time + 1LL;
    count = 0;
    for(const auto& span : spans) {
        if (current >= span.right) {
            const auto next = span.left;
            for(auto t = current - 1LL; t > next; --t) {
                reverse_count.at(t) = count;
            }
            current = next + 1;
            ++count;
        }
    }

    for(auto t = current - 1LL; t >= 0; --t) {
        reverse_count.at(t) = count;
    }

    for(decltype(n) i{0}; i<n; ++i) {
        const auto& span = indexed_spans.at(i);
        const auto n_meetings = 1LL + forward_count.at(span.left) + reverse_count.at(span.right);
        os << n_meetings << "\n";
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
