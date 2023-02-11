// https://atcoder.jp/contests/tessoku-book/submissions/38828407
#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

namespace {
    using Num = long long int;
    using Numeric = double;
    using Positions = std::array<Num, 3>;
    using Moves = std::vector<Positions>;
}

struct Decision {
    Decision() = default;
    std::vector<Num> state;
    Num              score {0};
    std::vector<Num> choises;

    void set_size(Num n_nums) {
        std::vector<Num> new_state(n_nums, 0);
        std::swap(state, new_state);
    }

    void move_state(const Positions& positions, Num choise) {
        for(const auto& i : positions) {
            state.at(i) += choise;
        }
        score += std::count_if(state.begin(), state.end(), [&](Num x) { return x == 0; });
        choises.push_back(choise);
    }
};

struct {
    bool operator()(const Decision& lhs, const Decision& rhs) const {
        return std::tie(lhs.score) > std::tie(rhs.score);
    }
} compareDecision;

void solve(std::istream& is, std::ostream& os) {
    constexpr Num n_nums = 20;
    Num t {0};
    is >> t;

    Moves moves;
    for(decltype(t) i{0}; i<t; ++i) {
        Num p0{0};
        Num p1{0};
        Num p2{0};
        is >> p0 >> p1 >> p2;
        --p0;
        --p1;
        --p2;

        Positions positions;
        positions.at(0) = p0;
        positions.at(1) = p1;
        positions.at(2) = p2;
        moves.push_back(positions);
    }

    std::vector<Decision> decisions;
    Decision initial_a;
    Decision initial_b;
    initial_a.set_size(n_nums);
    initial_b.set_size(n_nums);
    initial_a.move_state(moves.at(0), 1);
    initial_b.move_state(moves.at(0), -1);
    decisions.push_back(initial_a);
    decisions.push_back(initial_b);

    size_t beam_length = 1;
    beam_length <<= 13;
    for(decltype(t) i{1}; i<t; ++i) {
        std::vector<Decision> next_decisions;
        for(const auto& d : decisions) {
            Decision a = d;
            Decision b = d;
            a.move_state(moves.at(i), 1);
            b.move_state(moves.at(i), -1);
            next_decisions.push_back(a);
            next_decisions.push_back(b);
        }
        if (next_decisions.size() > beam_length) {
            std::sort(next_decisions.begin(), next_decisions.end(), compareDecision);
            next_decisions.resize(beam_length);
        }
        std::swap(decisions, next_decisions);
    }

    std::sort(decisions.begin(), decisions.end(), compareDecision);
    if (!decisions.empty()) {
        const auto& best = decisions.at(0);
        for(const auto& e : best.choises) {
            char c = (e > 0) ? 'A' : 'B';
            os << c << "\n";
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
