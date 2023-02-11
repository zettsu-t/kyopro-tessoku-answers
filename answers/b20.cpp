// https://atcoder.jp/contests/tessoku-book/submissions/38769748
#include <algorithm>
#include <iostream>
#include <string>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    std::string source;
    std::string target;
    is >> source >> target;

    const boost::array<Matrix::index, 2> shape {{
            static_cast<Num>(source.size() + 1),
            static_cast<Num>(target.size() + 1)
        }};

    Matrix count(shape);
    std::fill_n(count.data(), count.num_elements(), 0LL);

    for(size_t t=0; t<=target.size(); ++t) {
        count[0][t] = t;
    }

    for(size_t s=0; s<=source.size(); ++s) {
        count[s][0] = s;
    }

    for(size_t t=1; t<=target.size(); ++t) {
        for(size_t s=1; s<=source.size(); ++s) {
            Num count_min = -1;
            if (source.at(s-1) != target.at(t-1)) {
                count_min = std::min(count[s-1][t], count[s][t-1]);
                count_min = std::min(count[s-1][t-1], count_min);
                count_min += 1;
            } else {
                count_min = std::min(count[s-1][t], count[s][t-1]) + 1;
                count_min = std::min(count[s-1][t-1], count_min);
            }
            count[s][t] = count_min;
        }
    }

    os << count[source.size()][target.size()] << "\n";
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
