// https://atcoder.jp/contests/tessoku-book/submissions/38831079
#include <algorithm>
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
            const Num c = (mod + static_cast<Num>(s.at(i))) % mod;
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
    std::string r = s;
    std::reverse(r.begin(), r.end());

    Num mod = 1;
    mod <<= 29;
    constexpr Num base = 256;
    Num prime1 = mod - 3;
    Num prime2 = mod - 33;
    Num prime3 = mod - 43;
    StringHash forward_hash1(s, base, prime1);
    StringHash forward_hash2(s, base, prime2);
    StringHash forward_hash3(s, base, prime3);
    StringHash reverse_hash1(r, base, prime1);
    StringHash reverse_hash2(r, base, prime2);
    StringHash reverse_hash3(r, base, prime3);

    for(decltype(q) i{0}; i<q; ++i) {
        Num forward_left {0};
        Num forward_right {0};
        is >> forward_left >> forward_right;

        const Num reverse_left = n + 1 - forward_right;
        const Num reverse_right = n + 1 - forward_left;

        if ((forward_hash1.digest(forward_left, forward_right) ==
             reverse_hash1.digest(reverse_left, reverse_right)) &&
            (forward_hash2.digest(forward_left, forward_right) ==
             reverse_hash2.digest(reverse_left, reverse_right))) {
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
