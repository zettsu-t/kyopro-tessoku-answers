// https://atcoder.jp/contests/tessoku-book/submissions/38753387
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    Num x {0};
    is >> n >> x;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    using Position = size_t;
    Position left {0};
    Position right {n-1};
    Position answer {n};

    while(left <= right) {
        const Position center {(left + right) / 2};
        const auto value = nums.at(center);
        if (value == x) {
            answer = center;
            break;
        }

        if (x < value) {
            if (center == 0) {
                break;
            }
            right = center - 1;
        } else {
            if (center == (n - 1)) {
                break;
            }
            left = center + 1;
        }
    }

    os << (answer + 1) << "\n";
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
