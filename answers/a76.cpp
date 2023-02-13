// https://atcoder.jp/contests/tessoku-book/submissions/38861339
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num w {0};
    Num l {0};
    Num r {0};
    is >> n >> w >> l >> r;

    std::vector<Num> positions {0};
    for(decltype(n) i{1}; i<=n; ++i) {
        Num x {0};
        is >> x;
        positions.push_back(x);
    }
    positions.push_back(w);

    constexpr Num base {1000000007LL};
    std::vector<Num> n_combinations(n+2, 0);
    std::vector<Num> n_combi_cumsum(n+3, 0);
    n_combinations.at(0) = 1;
    n_combi_cumsum.at(1) = 1;

    for(decltype(n) i{1}; i<=(n+1); ++i) {
        auto it = positions.begin() + i;
        Num pos = *it;
        const Num left = pos - r;
        const Num right = pos - l;
        auto it_left = std::lower_bound(positions.begin(), positions.end(), left);
        auto it_right = std::lower_bound(positions.begin(), positions.end(), right);
        auto near_pos = *it_right;
        if ((it_right != positions.begin()) && (near_pos > right)) {
            --it_right;
            near_pos = *it_right;
        }

        Num local_n_combis {0};
        if ((*it_left >= left) && (near_pos <= right)) {
            auto i_left = std::distance(positions.begin(), it_left);
            auto i_right = std::distance(positions.begin(), it_right);
            local_n_combis = (base +
                              (n_combi_cumsum.at(i_right+1) % base) -
                              (n_combi_cumsum.at(i_left) % base)) % base;
            n_combinations.at(i) = local_n_combis;
        }
        n_combi_cumsum.at(i+1) = (n_combi_cumsum.at(i) + local_n_combis) % base;
    }

    os << n_combinations.at(n+1) << "\n";
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
