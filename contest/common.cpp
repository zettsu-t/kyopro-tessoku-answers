#include <bits/stdc++.h>

// #define USE_ACL 1
// #define USE_BIG_NUMBER 1

#ifdef USE_ACL
#include <atcoder/all>
#endif

#ifdef USE_BIG_NUMBER
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#endif

namespace {
#ifdef USE_ACL
    using ModInt [[maybe_unused]] = atcoder::modint998244353;
#endif

#ifdef USE_BIG_NUMBER
    using BigInt = __int128;
//  using BigInt = boost::multiprecision::cpp_int;
    using Rational = boost::rational<BigInt>;
    using BigIntVec = std::vector<BigInt>;
    using BigFloat [[maybe_unused]] = std::float128_t;
#endif

    using Num [[maybe_unused]] = long long int;
    using Vec [[maybe_unused]] = std::vector<Num>;
    using Set [[maybe_unused]] = std::set<Num>;
    using Mset [[maybe_unused]] = std::multiset<Num>;
    using Edges [[maybe_unused]] = std::vector<std::vector<Num>>;

    template<typename T>
    using Q [[maybe_unused]] = std::queue<T>;

    template<typename T>
    using PQ [[maybe_unused]] = std::priority_queue<T, std::vector<T>, std::greater<T>>;

    const std::vector<std::pair<Num, Num>> dyxs {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    std::map<char, std::pair<Num, Num>> directions {
        {'D', {1, 0}}, {'U', {-1, 0}}, {'R', {0, 1}}, {'L', {0, -1}}
    };

    template<typename T>
    void print_oneline(const std::vector<T>& vec, std::ostream& os) {
        const auto size = vec.size();
        for(size_t i{0}; i<size; ++i) {
            os << vec.at(i) << (((i+1) == size) ? '\n' : ' ');
        }
    }

    template<typename T>
    void print_each(const std::vector<T>& vec, std::ostream& os) {
        const auto size = vec.size();
        for(size_t i{0}; i<size; ++i) {
            os << vec.at(i) << '\n';
        }
    }
}

void solve(std::istream& is, std::ostream& os) {
#ifdef USE_ACL
    atcoder::dsu tree(1024);
#endif

#ifdef USE_BIG_NUMBER
    Num a_raw;
    is >> a_raw;
    const BigInt a = a_raw;
    Rational r {6};
    r /= a;
    os << static_cast<Num>(r.numerator()) << "/" << static_cast<Num>(r.denominator()) << "*\n";
    return;
#endif

    Num n {0};
    std::string s;
    is >> n >> s;

    os << s << n << "\n";

    const Vec v {1,2,3};
    print_oneline(v, os);
    print_each(v, os);
}

int main(void) {
    solve(std::cin, std::cout);
    return 0;
}
