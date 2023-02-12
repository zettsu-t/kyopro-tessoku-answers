// https://atcoder.jp/contests/tessoku-book/submissions/38830563
#include <iostream>
#include <string>
#include <vector>

namespace {
    using Num = long long int;
}

struct StringHash {
    std::string s;
    Num b {0};
    Num mod {0};
    std::vector<Num> digests;
    std::vector<Num> multipliers;

    StringHash(const std::string& arg_s, Num arg_b, Num arg_mod) :
        s(arg_s), b(arg_b), mod(arg_mod) {
        auto size = s.size();
        Num digest = 0;
        Num multiplier = 1;
        digests.push_back(digest);
        multipliers.push_back(multiplier);

        for(decltype(size) i{0}; i<size; ++i) {
            Num c = (mod + static_cast<Num>(s.at(i))) % mod;
            digest = ((digest * b) + c) % mod;
            digests.push_back(digest);
            multiplier = (multiplier * b) % mod;
            multipliers.push_back(multiplier);
        }
    }

    Num digest(Num left, Num right) {
        return (mod + digests.at(right) -
                (multipliers.at(right - left + 1) * digests.at(left - 1)) % mod)
            % mod;
    }
};

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    size_t q {0};
    std::string s;
    is >> n >> q >> s;

    Num mod = 1;
    mod <<= 29;
    constexpr Num base = 256;
    Num prime1 = mod - 3;
    Num prime2 = mod - 33;
    Num prime3 = mod - 43;
    StringHash hash1(s, base, prime1);
    StringHash hash2(s, base, prime2);
    StringHash hash3(s, base, prime3);

    for(decltype(q) i{0}; i<q; ++i) {
        Num a {0};
        Num b {0};
        Num c {0};
        Num d {0};
        is >> a >> b >> c >> d;

        if ((hash1.digest(a, b) == hash1.digest(c, d)) &&
            (hash2.digest(a, b) == hash2.digest(c, d)) &&
            (hash3.digest(a, b) == hash3.digest(c, d))) {
            os << "Yes\n";
        } else {
            os << "No\n";
        }
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
