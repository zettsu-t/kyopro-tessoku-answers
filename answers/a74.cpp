// https://atcoder.jp/contests/tessoku-book/submissions/38834015
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

struct Bit {
    std::vector<Num> bits_;
    Num max_ {0};
    Bit(Num size) : bits_(size+1, 0), max_(size) {}

    void increment(Num pos) {
        for(auto i=pos; i<=max_; i+=(i&-i)) {
            bits_.at(i) += 1;
        }
    }

    Num sum(Num right) {
        Num total {0};
        for (auto i=right; i>0; i-=(i&-i)) {
            total += bits_.at(i);
        }
        return total;
    }
};

Num count_inversions(const std::vector<Num>& vs, Num n) {
    Bit bit(n);
    Num count{0};
    for(decltype(n) i{0}; i<n; ++i) {
        const auto v = vs.at(i);
        count += i - bit.sum(v);
        bit.increment(v);
    }
    return count;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;

    std::vector<Num> xs(n, 0);
    std::vector<Num> ys(n, 0);
    for(decltype(n) y{0}; y<n; ++y) {
        for(decltype(n) x{0}; x<n; ++x) {
            Num p {0};
            is >> p;
            if (p > 0) {
                xs.at(x) = p;
                ys.at(y) = p;
            }
        }
    }

    const auto total = count_inversions(xs, n) + count_inversions(ys, n);
    os << total << "\n";
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
