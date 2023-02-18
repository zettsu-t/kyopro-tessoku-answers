// https://atcoder.jp/contests/tessoku-book/submissions/38957511
#include <algorithm>
#include <array>
#include <iostream>

namespace {
    using Num = long long int;
    constexpr size_t N_DIGITS = 4;
    using LotNumber = std::array<Num, N_DIGITS>;
}

LotNumber to_digits(Num num) {
    LotNumber numbers;
    Num x {num};

    for(size_t i{0}; i<N_DIGITS; ++i) {
        numbers.at(i) = x % 10;
        x /= 10;
    }

    std::reverse(numbers.begin(), numbers.end());
    return numbers;
}

std::string to_string(const LotNumber& number) {
    std::string s;

    for(auto it = number.begin(); it != number.end(); ++it) {
        const char c = *it + '0';
        s.push_back(c);
    }
    return s;
}

Num count_diff(const LotNumber& lhs, const LotNumber& rhs) {
    Num count{0};
    for(size_t i{0}; i<N_DIGITS; ++i) {
        count += (lhs.at(i) != rhs.at(i));
    }
    return count;
}

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    is >> n;

    std::vector<LotNumber> first_prize;
    std::vector<LotNumber> second_prize;
    std::vector<LotNumber> third_prize;

    for(decltype(n) i{0}; i<n; ++i) {
        Num s {0};
        Num t {0};
        is >> s >> t;

        const auto number = to_digits(s);
        if (t == 1) {
            first_prize.push_back(number);
        } else if (t == 2) {
            second_prize.push_back(number);
        } else {
            third_prize.push_back(number);
        }
    }

    if (first_prize.size() > 1) {
        os << "Can't Solve\n";
        return;
    }

    if (first_prize.size() == 1) {
        os << to_string(first_prize.at(0)) << "\n";
        return;
    }

    std::vector<Num> candidates;
    for(Num i{0}; i<=9999; ++i) {
        bool failed {false};
        const auto number = to_digits(i);

        for(const auto& other : second_prize) {
            if (count_diff(number, other) != 1) {
                failed = true;
                break;
            }
        }

        for(const auto& other : third_prize) {
            if (count_diff(number, other) < 2) {
                failed = true;
                break;
            }
        }

        if (!failed) {
            candidates.push_back(i);
        }
    }

    if (candidates.size() == 1) {
        os << to_string(to_digits(candidates.at(0))) << "\n";
        return;
    }

    os << "Can't Solve\n";
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
