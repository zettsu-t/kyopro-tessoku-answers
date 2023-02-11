// https://atcoder.jp/contests/tessoku-book/submissions/38769329
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    Num n {0};
    Num s {0};
    is >> n >> s;

    std::vector<Num> nums(n, 0);
    for (decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        nums.at(i) = a;
    }

    std::set<Num> sums {0};
    std::unordered_map<Num, std::vector<Num>> seqs;

    for (decltype(n) i{0}; i<n; ++i) {
        std::vector<Num> added;
        decltype(seqs) new_seqs;

        for(const auto& e : sums) {
            const auto total = e + nums.at(i);
            added.push_back(total);

            auto it = seqs.find(e);
            if (it == seqs.end()) {
                std::vector<Num> seq {i};
                new_seqs[total] = seq;
            } else {
                auto seq = it->second;
                seq.push_back(i);
                new_seqs[total] = seq;
            }
        }

        for(const auto& e : added) {
            if (e == s) {
                const auto& answer = new_seqs[e];
                auto size = answer.size();
                os << size << "\n";
                for (decltype(size) i{0}; i<size; ++i) {
                    os << (answer.at(i) + 1);
                    if ((i + 1) < size) {
                        os << " ";
                    }
                }
                os << "\n";
                return;
            }
            sums.insert(e);
            seqs[e] = new_seqs[e];
        }
    }

    os << "-1\n";
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
