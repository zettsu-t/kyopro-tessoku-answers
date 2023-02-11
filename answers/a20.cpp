// https://atcoder.jp/contests/tessoku-book/submissions/38769682
#include <algorithm>
#include <iostream>
#include <string>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Matrix = boost::multi_array<long long int, 2>;
    std::string target;
    std::string sub;
    is >> target >> sub;

    if (sub.size() > target.size()) {
        std::swap(sub, target);
    }

    const boost::array<Matrix::index, 2> shape {{
            static_cast<long long int>(target.size() + 1),
            static_cast<long long int>(sub.size() + 1)
        }};
    Matrix count(shape);
    std::fill_n(count.data(), count.num_elements(), 0LL);

    for(size_t t=1; t<=target.size(); ++t) {
        for(size_t s=1; s<=sub.size(); ++s) {
            bool matched = (target.at(t-1) == sub.at(s-1));
            auto count_max = std::max(count[t-1][s], count[t][s-1]);
            if (matched) {
                count_max = std::max(1 + count[t-1][s-1], count_max);
            }
            count[t][s] = count_max;
        }
    }

    os << count[target.size()][sub.size()] << "\n";
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
