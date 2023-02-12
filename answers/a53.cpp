// https://atcoder.jp/contests/tessoku-book/submissions/38829561
#include <iostream>
#include <queue>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t q {0};
    is >> q;

    std::priority_queue<Num, std::vector<Num>, std::greater<Num>> goods;
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            Num x {0};
            is >> x;
            goods.push(x);
        } else if (c == 2) {
            os << goods.top() << "\n";
        } else if (c == 3) {
            goods.pop();
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
