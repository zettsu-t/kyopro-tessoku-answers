// https://atcoder.jp/contests/tessoku-book/submissions/38861481
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    struct Card {
        Num up {0};
        Num down {0};
    };

    std::vector<Card> cards;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        cards.push_back(Card{a, b});
    }

    Num score_up_pp {0};
    Num score_down_pp {0};
    Num score_up_pn {0};
    Num score_down_pn {0};
    Num score_up_np {0};
    Num score_down_np {0};
    Num score_up_nn {0};
    Num score_down_nn {0};

    for(const auto& e : cards) {
        const Num abs_up = std::abs(e.up);
        const Num abs_down = std::abs(e.down);

        if ((e.up > 0) && (e.down > 0)) {
            score_up_pp += e.up;
            score_down_pp += e.down;
            if (abs_up > abs_down) {
                score_up_pn += e.up;
                score_down_pn += e.down;
            } else {
                score_up_np += e.up;
                score_down_np += e.down;
            }
        }

        if ((e.up < 0) && (e.down < 0)) {
            score_up_nn += e.up;
            score_down_nn += e.down;
            if (abs_up > abs_down) {
                score_up_np += e.up;
                score_down_np += e.down;
            } else {
                score_up_pn += e.up;
                score_down_pn += e.down;
            }
        }

        if ((e.up > 0) && (e.down < 0)) {
            score_up_pn += e.up;
            score_down_pn += e.down;
            if (abs_up > abs_down) {
                score_up_pp += e.up;
                score_down_pp += e.down;
            } else {
                score_up_nn += e.up;
                score_down_nn += e.down;
            }
        }

        if ((e.up < 0) && (e.down > 0)) {
            score_up_np += e.up;
            score_down_np += e.down;
            if (abs_up > abs_down) {
                score_up_nn += e.up;
                score_down_nn += e.down;
            } else {
                score_up_pp += e.up;
                score_down_pp += e.down;
            }
        }
    }

    const auto score_pp = std::abs(score_up_pp) + std::abs(score_down_pp);
    const auto score_pn = std::abs(score_up_pn) + std::abs(score_down_pn);
    const auto score_np = std::abs(score_up_np) + std::abs(score_down_np);
    const auto score_nn = std::abs(score_up_nn) + std::abs(score_down_nn);
    const auto score = std::max(score_pp, std::max(score_pn, std::max(score_np, score_nn)));
    os << score << "\n";
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
