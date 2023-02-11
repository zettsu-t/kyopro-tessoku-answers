// https://atcoder.jp/contests/tessoku-book/submissions/38775860
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> nums(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    decltype(nums) scores = nums;
    for(decltype(n) turn=n-1; turn>=1; --turn) {
        decltype(scores) new_scores;
        if ((turn & 1) > 0) {
            for(decltype(turn) i{0}; i < turn; ++i) {
                new_scores.push_back(std::max(scores.at(i), scores.at(i+1)));
            }
        } else {
            for(decltype(turn) i{0}; i < turn; ++i) {
                new_scores.push_back(std::min(scores.at(i), scores.at(i+1)));
            }
        }
        std::swap(scores, new_scores);
    }

    os << scores.at(0) << "\n";
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
