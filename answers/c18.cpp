// https://atcoder.jp/contests/tessoku-book/submissions/39290438
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr Num inf = 1000000000000LL;

    Num n {0};
    is >> n;
    Num size = n * 2;

    std::vector<Num> nums(size, 0);
    for(decltype(size) i{0}; i<size; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    const MatrixShape shape {{size, size}};
    Matrix costs(shape);
    std::fill_n(costs.data(), costs.num_elements(), inf);

    for(decltype(size) dist{1}; dist<size; ++dist) {
        for(decltype(size) left{0}; left<(size-1); ++left) {
            const auto right = left + dist;
            if (right >= size) {
                continue;
            }

            Num min_cost = std::abs(nums.at(left) - nums.at(right));
            if ((left + 1) < right) {
                min_cost = costs[left+1][right-1] + std::abs(nums.at(left) - nums.at(right));
                for(decltype(size) i{left}; i<right; ++i) {
                    const auto cost = costs[left][i] + costs[i+1][right];
                    min_cost = std::min(min_cost, cost);
                }
            }
            costs[left][right] = min_cost;
        }
    }

    os << costs[0][size-1] << "\n";
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
