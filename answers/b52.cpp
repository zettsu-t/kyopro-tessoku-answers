// https://atcoder.jp/contests/tessoku-book/submissions/38829666
#include <iostream>
#include <queue>
#include <string>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num x {0};
    std::string s;
    is >> n >> x >> s;
    --x;

    std::queue<Num> positions;
    positions.push(x);
    s[x] = '@';

    do {
        const auto p = positions.front();
        positions.pop();
        Num left = p - 1;
        Num right = p + 1;
        bool add_left = ((left >= 0) && (s.at(left) == '.'));
        bool add_right = ((right < n) && (s.at(right) == '.'));

        if (add_left) {
            s[left] = '@';
            positions.push(left);
        }

        if (add_right) {
            s[right] = '@';
            positions.push(right);
        }
    } while(!positions.empty());

    os << s << "\n";
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
