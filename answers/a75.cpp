// https://atcoder.jp/contests/tessoku-book/submissions/38834107
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
}

struct Test {
    Num time_to_solve {0};
    Num deadline {0};
};

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;

    struct {
        bool operator()(const Test& lhs, const Test& rhs) const {
            return std::tie(lhs.deadline, lhs.time_to_solve) <
                std::tie(rhs.deadline, rhs.time_to_solve);
        }
    } lessDeadline;

    std::vector<Test> tests;
    for(decltype(n) i{0}; i<n; ++i) {
        Num t {0};
        Num d {0};
        is >> t >> d;
        tests.push_back(Test{t, d});
    }
    std::sort(tests.begin(), tests.end(), lessDeadline);

    constexpr Num max_time = 1440;
    const MatrixShape shape {{n + 1, max_time + 1}};
    Matrix counts(shape);
    std::fill_n(counts.data(), counts.num_elements(), 0LL);

    Num count {0};
    for(decltype(n) i{1}; i<=n; ++i) {
        for(Num t=0; t<=max_time; ++t) {
            const Num time_to_solve = tests.at(i-1).time_to_solve;
            Num local_count {0};
            if ((t < time_to_solve) || (t > tests.at(i-1).deadline)) {
                local_count = counts[i-1][t];
            } else {
                local_count = std::max(counts[i-1][t], counts[i-1][t-time_to_solve] + 1LL);
            }
            counts[i][t] = local_count;
            count = std::max(count, local_count);
        }
    }

    os << count << "\n";
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
