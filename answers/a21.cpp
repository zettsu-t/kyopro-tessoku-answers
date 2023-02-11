// https://atcoder.jp/contests/tessoku-book/submissions/38769848
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    Num n {0};
    is >> n;

    std::vector<Num> blocks(n+2, 0);
    std::vector<Num> scores(n+2, 0);
    for (Num i=1; i<=n; ++i) {
        Num p {0};
        Num a {0};
        is >> p >> a;
        blocks.at(i) = p;
        scores.at(i) = a;
    }

    const boost::array<Matrix::index, 2> shape {{n+2, n+2}};
    Matrix value(shape);
    std::fill_n(value.data(), value.num_elements(), 0LL);

    for(Num len=n-1; len >= 0; --len) {
        for(Num left=1; left<= n-len; ++left) {
          const auto right = left + len;
          const auto left_prev = left - 1;
          const auto right_prev = right + 1;
          const auto left_score = ((left <= blocks[left_prev]) && (blocks[left_prev] <= right)) ? scores[left_prev] : 0;
          const auto right_score = ((left <= blocks[right_prev]) && (blocks[right_prev] <= right)) ? scores[right_prev] : 0;
          value[left][right] = std::max(value[left_prev][right] + left_score, value[left][right_prev] + right_score);
        }
    }

    Num max_score = 0;
    for(Num i=1; i<=n; ++i) {
        max_score = std::max(max_score, value[i][i]);
    }

    os << max_score << "\n";
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
