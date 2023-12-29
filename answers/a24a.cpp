// https://atcoder.jp/contests/tessoku-book/submissions/48915178
#include <bits/stdc++.h>

namespace {
    using Num [[maybe_unused]] = long long int;
    using Vec [[maybe_unused]] = std::vector<Num>;
    using Set [[maybe_unused]] = std::set<Num>;
    using Mset [[maybe_unused]] = std::multiset<Num>;
    using Edges [[maybe_unused]] = std::vector<std::vector<Num>>;

    template<typename T>
    using Q [[maybe_unused]] = std::queue<T>;

    template<typename T>
    using PQ [[maybe_unused]] = std::priority_queue<T, std::vector<T>, std::greater<T>>;
}

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    is >> n;

    Vec ns(n);
    for(auto&& a : ns) {
        is >> a;
    }

    Vec lis;
    for (const auto& a : ns) {
        auto it = std::lower_bound(lis.begin(), lis.end(), a);
        if (it == lis.end()) {
            lis.push_back(a);
        } else {
            *it = a;
        }
    }

    os << lis.size() << "\n";
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
