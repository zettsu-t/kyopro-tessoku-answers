#include <bits/stdc++.h>
#include <atcoder/modint>

namespace {
    using ModInt [[maybe_unused]] = atcoder::modint998244353;
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
    std::string s;
    is >> n >> s;
    os << s << n << "\n";
}

int main(void) {
    solve(std::cin, std::cout);
    return 0;
}
