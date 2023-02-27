// https://atcoder.jp/contests/tessoku-book/submissions/39301505
#include <iostream>
#include <string>
#include <utility>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Name = std::string;
    using Line = std::vector<Name>;

    Num q {0};
    is >> q;

    Line head(q*2, "");
    Line tail(q*2, "");
    Num head_left {q};
    Num head_right {q};
    Num tail_left {q};
    Num tail_right {q};

    for(decltype(q) i{0}; i<q; ++i) {
        auto head_size = head_right - head_left;
        auto tail_size = tail_right - tail_left;

        std::string c;
        is >> c;

        if (c == "A") {
            Name x;
            is >> x;
            tail.at(tail_right) = x;
            ++tail_right;
            ++tail_size;

            if (head_size < tail_size) {
                std::swap(head.at(head_right), tail.at(tail_left));
                ++head_right;
                ++head_size;
                ++tail_left;
                --tail_size;
            }
        } else if (c == "B") {
            Name x;
            is >> x;
            if (head_size == tail_size) {
                head.at(head_right) = x;
                ++head_right;
                ++head_size;
            } else {
                if (tail_size > 0) {
                    --tail_left;
                    tail.at(tail_left) = x;
                    ++tail_size;
                } else {
                    tail.at(tail_right) = x;
                    ++tail_right;
                    ++tail_size;
                }
            }
        } else if (c == "C") {
            ++head_left;
            --head_size;
            if ((head_size < tail_size) && (tail_size > 0)) {
                std::swap(head.at(head_right), tail.at(tail_left));
                ++head_right;
                ++head_size;
                ++tail_left;
                --tail_size;
            }
        } else {
            os << head.at(head_left) << "\n";
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
