// https://atcoder.jp/contests/tessoku-book/submissions/38839854
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<long long int, 2>;
    Num n {0};
    Num max_weight {0};
    is >> n >> max_weight;

    std::vector<Num> values(n, 0);
    std::vector<Num> weights(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num w {0};
        Num v {0};
        is >> w >> v;
        weights.at(i) = w;
        values.at(i) = v;
    }

    const boost::array<Matrix::index, 2> shape {{n+1, max_weight+1}};
    Matrix value_board(shape);
    std::fill_n(value_board.data(), value_board.num_elements(), 0);
    value_board[0][0] = 0;

    for(decltype(n) item{1}; item<=n; ++item) {
        for(decltype(max_weight) weight{0}; weight<=max_weight; ++weight) {
            value_board[item][weight] = std::max(value_board[item][weight], value_board[item-1][weight]);
            const auto new_weight = weight + weights.at(item-1);
            if (new_weight <= max_weight) {
                const auto value_sum = value_board[item-1][weight] + values.at(item-1);
                value_board[item][new_weight] = std::max(value_board[item][new_weight], value_sum);
            }
        }
    }

    Num total {0};
    for (decltype(max_weight) weight{0}; weight<=max_weight; ++weight) {
        total = std::max(total, value_board[n][weight]);
    }

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
