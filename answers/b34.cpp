// https://atcoder.jp/contests/tessoku-book/submissions/38861499
#include <iostream>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
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

    const std::vector<Num> gnums {0,0,1,1,2};
    Num gnum {0};
    for(const auto& height : heights) {
        const Num local_gnum = gnums.at(height % 5);
        gnum ^= local_gnum;
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
