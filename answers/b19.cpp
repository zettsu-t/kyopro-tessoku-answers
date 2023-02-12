// https://atcoder.jp/contests/tessoku-book/submissions/38841409
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
    constexpr Num max_value {100000};

    std::vector<Num> values(n, 0);
    std::vector<Num> weights(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num w {0};
        Num v {0};
        is >> w >> v;
        weights.at(i) = w;
        values.at(i) = v;
    }

    const boost::array<Matrix::index, 2> shape {{n+1, max_value+1}};
    Matrix weight_board(shape);
    std::fill_n(weight_board.data(), weight_board.num_elements(), 1000000000000000);
    weight_board[0][0] = 0;

    for(decltype(n) item{1}; item<=n; ++item) {
        weight_board[item][0] = 0;
        for(Num value{0}; value<=max_value; ++value) {
            weight_board[item][value] = std::min(weight_board[item][value], weight_board[item-1][value]);
            if (value >= values.at(item-1)) {
                const auto value_prev = value - values.at(item-1);
                const auto new_weight = weight_board[item-1][value_prev] + weights.at(item-1);
                if (new_weight <= max_weight) {
                    weight_board[item][value] = std::min(weight_board[item][value], new_weight);
                }
            }
        }
    }

    Num total {0};
    for(Num value{0}; value<=max_value; ++value) {
        if (weight_board[n][value] <= max_weight) {
            total = value;
        }
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
