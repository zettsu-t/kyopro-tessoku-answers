// https://atcoder.jp/contests/tessoku-book/submissions/38861355
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

namespace {
    using Num = long long int;
    constexpr Num invalid = -1;
}

Num count_cuts(const std::vector<Num>& spans, Num score) {
    Num len {0};
    Num n_cuts {0};
    Num size = static_cast<Num>(spans.size());

    for(decltype(size) i{0}; (i+1) < size; ++i) {
        len += spans.at(i);
        if (len >= score) {
            ++n_cuts;
            len = 0;
        }

        if ((i+2) == size) {
            const auto last = spans.at(i+1);
            if ((len + last) < score) {
                n_cuts = std::max(0LL, n_cuts-1LL);
            }
        }
    }

    return n_cuts;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num l {0};
    Num k {0};
    is >> n >> l >> k;

    std::vector<Num> spans(n+1, 0LL);
    Num prev {0};
    for(Num i=0; i<n; ++i) {
        Num a {0};
        is >> a;
        spans.at(i) = a - prev;
        prev = a;
    }
    spans.at(n) = l - prev;

    Num left {0};
    Num right {l};
    while(left != right) {
        if ((left == right) || ((left+1) == right)) {
            const auto n_right = count_cuts(spans, right);
            if (n_right == k) {
                break;
            }

            const auto n_left = count_cuts(spans, left);
            if (n_left == k) {
                right = left;
                break;
            }
            break;
        }

        Num center = (left+right) / 2;
        const auto n_center = count_cuts(spans, center);
        if (n_center >= k) {
            left = std::min(center, l);
        } else {
            right = std::max(center-1, 0LL);
        }
    }

    os << right << "\n";
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
