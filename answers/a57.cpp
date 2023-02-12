// https://atcoder.jp/contests/tessoku-book/submissions/38831252
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;

    Num n {0};
    size_t q {0};
    is >> n >> q;

    std::vector<Num> one_steps;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        one_steps.push_back(a);
    }

    constexpr Num n_doubling = 63;
    const MatrixShape shape {{n_doubling, n}};
    Matrix nums(shape);
    std::fill_n(nums.data(), nums.num_elements(), 0LL);
    Matrix multi_steps(shape);
    std::fill_n(multi_steps.data(), multi_steps.num_elements(), 0LL);

    for(decltype(n) x{0}; x < n; ++x) {
        multi_steps[0][x] = one_steps[x];
    }

    for(Num y{1}; y < n_doubling; ++y) {
        for(decltype(n) x{0}; x < n; ++x) {
            const auto next = multi_steps[y-1][x];
            multi_steps[y][x] = multi_steps[y-1][next-1];
        }
    }

    for(decltype(q) i{0}; i<q; ++i) {
        Num x {0};
        Num y {0};
        is >> x >> y;

        Num pos {x};
        Num rest {y};
        Num step {0};
        while(rest > 0) {
            if (rest & 1) {
                pos = multi_steps[step][pos-1];
            }
            ++step;
            rest = rest / 2;
        }

        os << pos << "\n";
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
