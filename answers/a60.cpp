// https://atcoder.jp/contests/tessoku-book/submissions/38831788
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    is >> n;

    std::vector<Num> nums;
    for(decltype(n) i{0}; i<n; ++i) {
        Num c {0};
        is >> c;
        nums.push_back(c);
    }

    std::reverse(nums.begin(), nums.end());
    std::stack<Num> days;
    std::stack<Num> heights;
    std::vector<Num> results(n, -1);

    for(decltype(n) i{0}; i<n; ++i) {
        while(!heights.empty() && (nums.at(i) > heights.top())) {
            Num day_index = n - 1 - days.top();
            results.at(day_index) = n - i;
            days.pop();
            heights.pop();
        }
        days.push(i);
        heights.push(nums.at(i));
    }

    std::reverse(nums.begin(), nums.end());
    for(decltype(n) i{0}; i<n; ++i) {
        os << results.at(i);
        if (i < (n-1)) {
            os << " ";
        }
    }
    os << "\n";

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
