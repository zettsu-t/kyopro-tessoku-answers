// https://atcoder.jp/contests/tessoku-book/submissions/39068688
#include <iostream>
#include <map>
#include <vector>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
}

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr Num inf = 1000000000LL;

    Num n {0};
    Num m {0};
    Num k {0};
    is >> n >> m >> k;

    if (k == 1) {
        os << m << "\n";
        return;
    }

    const MatrixShape shape {{n, k}};
    Matrix board(shape);
    std::fill_n(board.data(), board.num_elements(), inf);

    std::multimap<Num, Num> from_links;
    std::multimap<Num, Num> to_links;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        if (a > b) {
            std::swap(a, b);
        }
        from_links.insert(std::make_pair(a, b));
        to_links.insert(std::make_pair(b, a));
    }

    for(decltype(n) page{1}; page<n; ++page) {
        Num n_cuts = {0};
        for(decltype(n) from{1}; from<=page; ++from) {
            auto its = from_links.equal_range(from);
            for(auto it = its.first; it != its.second; ++it) {
                const auto to = it->second;
                if (to > page) {
                    ++n_cuts;
                }
            }
        }
        board[page][1] = n_cuts;
    }

    if (k == 2) {
        Num score{inf};
        for(decltype(n) i{1}; i<n; ++i) {
            score = std::min(score, board[i][1]);
        }
        os << (m - score) << "\n";
        return;
    }

    for(decltype(k) cut{2}; cut<k; ++cut) {
        const Num prev_cut{cut-1};
        for(decltype(n) page{1}; page<n; ++page) {
            Num min_score{inf};

            for(decltype(n) prev{prev_cut}; prev<page; ++prev) {
                Num n_cuts{0};
                for(decltype(n) from{prev+1}; from<=page; ++from) {
                    auto its = from_links.equal_range(from);
                    for(auto it = its.first; it != its.second; ++it) {
                        const auto to = it->second;
                        if (to > page) {
                            ++n_cuts;
                        }
                    }
                }
                const auto local_score = board[prev][prev_cut] + n_cuts;
                min_score = std::min(min_score, local_score);
            }

            board[page][cut] = min_score;
        }
    }

    Num score{inf};
    for(decltype(n) i{1}; i<n; ++i) {
        score = std::min(score, board[i][k-1]);
    }
    score = std::max(m - score, 0LL);

    os << score << "\n";
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
