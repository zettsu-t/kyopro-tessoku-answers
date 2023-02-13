// https://atcoder.jp/contests/tessoku-book/submissions/38861272
#include <algorithm>
#include <iostream>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    constexpr Num max_ab = 100;
    size_t n {0};
    Num k {0};
    is >> n >> k;

    struct Player {
        Num a {0};
        Num b {0};
    };

    std::vector<Player> players;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        players.push_back(Player{a,b});
    }

    Num count {0};
    for(Num a{0}; a <= max_ab; ++a) {
        const auto upper_a = a + k;
        for(Num b{0}; b <= max_ab; ++b) {
            const auto upper_b = b + k;
            Num local_count {0};
            for(const auto& player : players) {
                local_count += (player.a >= a) && (player.b >= b) &&
                    (player.a <= upper_a) && (player.b <= upper_b);
            }
            count = std::max(count, local_count);
        }
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
