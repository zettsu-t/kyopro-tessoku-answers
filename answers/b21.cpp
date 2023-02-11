// https://atcoder.jp/contests/tessoku-book/submissions/38769932/
#include <algorithm>
#include <iostream>
#include <string>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    Num n {0};
    std::string s;
    is >> n >> s;

    const boost::array<Matrix::index, 2> shape {{n+2, n+2}};
    Matrix value(shape);
    std::fill_n(value.data(), value.num_elements(), 0LL);

    for(Num len=0; len<=(n-1); ++len) {
        for(Num left=1; left<=(n-len); ++left) {
            const auto right = left + len;
            Num count{0};
            if (len == 0) {
                count = 1;
            } else if (len == 1) {
                count = (s.at(left-1) == s.at(right-1)) ? 2 : 1;
            } else {
                count = std::max(value[left+1][right], value[left][right-1]);
                if (s.at(left-1) == s.at(right-1)) {
                    count = std::max(count, value[left+1][right-1] + 2);
                }
            }
            value[left][right] = count;
        }
    }

    const auto max_score = value[1][n];
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
