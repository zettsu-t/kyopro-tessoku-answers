// https://atcoder.jp/contests/tessoku-book/submissions/38765061
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
}

Num count_flyers(const std::vector<Num>& nums, Num period) {
    Num total {0};
    for(const auto& n : nums){
        total+= period / n;
    }

    return total;
}

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    Num k {0};
    is >> n >> k;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    Num left {0};
    Num right {1000000000};
    Num answer {right};

    while(left <= right) {
        const Num center {(left + right) / 2};
        const Num current = count_flyers(nums, center);

        if (count_flyers(nums, left) >= k) {
            answer = left;
            break;
        }

        if (((left + 1) == right) && (count_flyers(nums, right) >= k)) {
            answer = right;
            break;
        }

        if (current < k) {
            left = center + 1;
            continue;
        }

        right = center;
    }

    os << answer << "\n";
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
