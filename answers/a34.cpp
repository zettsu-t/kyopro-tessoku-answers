// https://atcoder.jp/contests/tessoku-book/submissions/38861225
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

namespace {
    using Num = long long int;
}

std::vector<Num> grundy_numbers(Num height, Num x, Num y) {
    std::vector<Num> gnums {0};
    for(decltype(height) i{1}; i<=height; ++i) {
        std::set<Num> local_gnums;
        if (i >= x) {
            local_gnums.insert(gnums.at(i-x));
        }

        if (i >= y) {
            local_gnums.insert(gnums.at(i-y));
        }

        Num gnum {0};
        if (!local_gnums.empty()) {
            for(Num i=0; i<=3; ++i) {
                if (local_gnums.find(i) == local_gnums.end()) {
                    gnum = i;
                    break;
                }
            }
        }
        gnums.push_back(gnum);
    }

    return gnums;
}

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    Num x {0};
    Num y {0};
    is >> n >> x >> y;

    std::vector<Num> heights(n, 0);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        heights.at(i) = a;
    }

    const auto max_height = *std::max_element(heights.begin(), heights.end());
    const auto gnums = grundy_numbers(max_height, x, y);

    Num gnum {0};
    for(const auto& height : heights) {
        gnum ^= gnums.at(height);
    }

    if (gnum > 0) {
        os << "First\n";
    } else {
        os << "Second\n";
    }
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
