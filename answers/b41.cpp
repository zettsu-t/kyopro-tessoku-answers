// https://atcoder.jp/contests/tessoku-book/submissions/38861459
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
    using Num = long long int;
    struct Op {
        bool set_x {false};
        Num  times {0};
    };
    using OpSeq = std::vector<Op>;
}

void solve_detail(Num in_x, Num in_y, std::ostream& os) {
    Num x {in_x};
    Num y {in_y};
    OpSeq ops;

    for(;;) {
        if ((x == y) || (x == 0) || (y == 0)) {
            break;
        }

        if (x == 1) {
            ops.push_back(Op{false, y - x});
            break;
        }

        if (y == 1) {
            ops.push_back(Op{true, x - y});
            break;
        }

        if (x > y) {
            ops.push_back(Op{true, x / y});
            x = x % y;
        } else if (x < y) {
            ops.push_back(Op{false, y / x});
            y = y % x;
        }
    }

    std::reverse(ops.begin(), ops.end());
    Num x_target {1};
    Num y_target {1};
    Num n_steps {0};
    for(const auto& op : ops) {
        n_steps += op.times;
    }
    os << n_steps << "\n";

    for(const auto& op : ops) {
        auto times = op.times;
        n_steps += times;
        for(decltype(times) i{0}; i<times; ++i) {
            if (op.set_x) {
                x_target += y_target;
            } else {
                y_target += x_target;
            }
            os << x_target << " " << y_target<< "\n";
        }
    }
    return;
}

void solve(std::istream& is, std::ostream& os) {
    Num x {0};
    Num y {0};
    is >> x >> y;
    solve_detail(x, y, os);
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
