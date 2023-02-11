// https://atcoder.jp/contests/tessoku-book/submissions/38776981
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    Num n {0};
    Num m {0};
    is >> n >> m;

    Num expected {1};
    expected <<= n;
    --expected;

    Num all_coupons {0};
    std::vector<Num> coupons(m,0);
    for (decltype(m) i{0}; i<m; ++i) {
        Num s {0};
        for (decltype(n) j{0}; j<n; ++j) {
            Num a {0};
            is >> a;
            s <<= 1;
            s |= a;
        }
        coupons.at(i) = s;
        all_coupons |= s;
    }

    if (all_coupons != expected) {
        os << "-1\n";
        return;
    }

    std::unordered_multimap<Num, Num> popcnts;
    for(decltype(expected) i{0}; i <= expected; ++i) {
        popcnts.insert(std::pair<Num, Num>(__builtin_popcountll(i), i));
    }

    const boost::array<Matrix::index, 2> shape {{m+1, expected+1}};
    Matrix counts(shape);
    std::fill_n(counts.data(), counts.num_elements(), 1000000000LL);
    counts[0][0] = 0;

    for(decltype(n) i{1}; i<=m; ++i) {
        counts[i][0] = 0;
        for(decltype(n) n_bits{0}; n_bits <= n; ++n_bits) {
            auto its = popcnts.equal_range(n_bits);
            for(auto it = its.first; it != its.second; ++it) {
                const auto from = it->second;
                counts[i][from] = std::min(counts[i][from], counts[i-1][from]);
                const auto to = from | coupons.at(i-1);
                counts[i][to] = std::min(counts[i][to], counts[i-1][from] + 1);
            }
        }
    }

    os << counts[m][expected] << "\n";
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
