// https://atcoder.jp/contests/tessoku-book/submissions/38765559
#include <algorithm>
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

std::vector<Num> enumulate_sums(const std::vector<Num>& nums) {
    Num n_bits = static_cast<Num>(nums.size());
    Num size = 1;
    size <<= n_bits;
    std::vector<Num> sums;
    sums.reserve(size);

    for(decltype(size) combi{0}; combi<size; ++combi) {
        Num total {0};
        for(decltype(size) item{0}; item<n_bits; ++item) {
            Num mask {1};
            mask <<= item;
            total += (combi & mask) ? nums.at(item) : 0;
        }
        sums.push_back(total);
    }

    std::sort(sums.begin(), sums.end());
    return sums;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num k {0};
    is >> n >> k;

    if (n == 1) {
        Num a {0};
        is >> a;
        if (a == k) {
            os << "Yes\n";
        } else {
            os << "No\n";
        }
        return;
    }

    Num n_top = n / 2;
    Num n_bottom = n - n_top;
    std::vector<Num> top(n_top, 0);
    for (decltype(n_top) i=0; i<n_top; ++i) {
        Num a {0};
        is >> a;
        top.at(i) = a;
    }

    std::vector<Num> bottom(n_bottom, 0);
    for (decltype(n_bottom) i=0; i<n_bottom; ++i) {
        Num a {0};
        is >> a;
        bottom.at(i) = a;
    }

    const auto sums_top = enumulate_sums(top);
    const auto sums_bottom = enumulate_sums(bottom);
    for(const auto& e : sums_top) {
        if (std::binary_search(sums_bottom.begin(), sums_bottom.end(), k - e)) {
            os << "Yes\n";
            return;
        }
    }

    os << "No\n";
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
