// https://atcoder.jp/contests/tessoku-book/submissions/38765168
#include <algorithm>
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num k {0};
    is >> n >> k;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }
    std::sort(nums.begin(), nums.end());

    Num count = 0;
    Num left = 0;
    Num right = 1;
    while(left < n) {
        while((right < n) && ((nums.at(left) + k) >= nums.at(right))) {
            ++right;
        }
        count += right - left - 1;
        ++left;
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
