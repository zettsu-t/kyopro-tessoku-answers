//
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr Num n_steps {1000};
    constexpr Num board_size {100};
    constexpr Num step_height {board_size};
    constexpr Num board_max_height {100000};
}

struct Step {
    Num x {0};
    Num y {0};
    Num h {0};
    void print(std::ostream& os) const {
        os << x << " " << y << " " << h << "\n";
    }
};

using Steps = std::vector<Step>;

struct Board {
    Num n_ {0};
    MatrixShape shape_;
    std::unique_ptr<Matrix> pMat_;
    std::vector<Step> heights_;

    Board(const Board& source) {
        n_ = source.n_;
        shape_ = source.shape_;
        std::unique_ptr<Matrix> pMat = std::make_unique<Matrix>(shape_);
        *pMat = *(source.pMat_);
        std::swap(pMat_, pMat);
        heights_ = source.heights_;
    }

    Board(Num n) : n_(n) {
        MatrixShape shape {{n, n}};
        std::unique_ptr<Matrix> pMat = std::make_unique<Matrix>(shape);
        std::fill_n(pMat->data(), pMat->num_elements(), 0LL);
        std::swap(shape_, shape);
        std::swap(pMat_, pMat);
    }

    void set(Num x, Num y, Num value) {
        (*pMat_)[y][x] += value;
    }

    Num get(Num x, Num y) {
        return (*pMat_)[y][x];
    }

    void step_impl(Num x, Num y, Num h, Num sign) {
        const Num left = std::max(0LL, x - (h - 1LL));
        const Num right = std::min(n_ - 1LL, x + (h - 1LL));
        for(Num ix = left; ix <= right; ++ix) {
            const Num h_rest = h - std::abs(x - ix);
            const Num top = std::max(0LL, y - (h_rest - 1LL));
            const Num bottom = std::min(n_ - 1LL, y + (h_rest - 1LL));
            for(Num iy = top; iy <= bottom; ++iy) {
                const Num value = std::max(0LL, h - std::abs(x - ix) - std::abs(y - iy));
                (*pMat_)[iy][ix] += value * sign;
            }
        }
    }

    void step(const Step& step) {
        step_impl(step.x, step.y, step.h, 1);
    }

    void undo(const Step& step) {
        step_impl(step.x, step.y, step.h, -1);
    }

    Num diff(const Board& other) const {
        Num score {0};
        for(decltype(n_) y{0}; y<n_; ++y) {
            for(decltype(n_) x{0}; x<n_; ++x) {
                score += std::abs((*pMat_)[y][x] - (*other.pMat_)[y][x]);
            }
        }
        return score;
    }
};

Step random_step(
    std::mt19937& engine,
    std::uniform_int_distribution<>& dist_xy,
    std::uniform_int_distribution<>& dist_height) {
    Num x = dist_xy(engine);
    Num y = dist_xy(engine);
    Num h = dist_height(engine);
    return Step{x, y, h};
}

void solve(std::istream& is, std::ostream& os) {
    constexpr double time_limit_msec = 5800.0;
    std::clock_t clock_start = std::clock();
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    Board expected(board_size);
    std::vector<Step> expected_steps;
    std::uniform_int_distribution<> dist_initial_xy(0, board_size-1);
    std::uniform_int_distribution<> dist_initial_height(1, step_height);

    for(Num y{0}; y<board_size; ++y) {
        for(Num x{0}; x<board_size; ++x) {
            Num a {0};
            is >> a;
            expected.set(x, y, a);
        }
    }

    Board actual(board_size);
    std::vector<Step> actual_steps(n_steps);
    for(Num i{0}; i<n_steps; ++i) {
        const auto step = random_step(engine, dist_initial_xy, dist_initial_height);
        actual.step(step);
        actual_steps.at(i) = step;
    }
    auto prev_score = actual.diff(expected);

    std::uniform_int_distribution<> dist_xy(-9, 9);
    std::uniform_int_distribution<> dist_height(-19, 19);
    for(;;) {
        std::clock_t clock_now = std::clock();
        const auto elapsed_msec = 1000.0 * ((clock_now - clock_start)) / CLOCKS_PER_SEC;
        if (elapsed_msec > time_limit_msec) {
            break;
        }

        for(Num i{0}; i<n_steps; ++i) {
            const auto currrent_step = actual_steps.at(i);
            const auto diff_x = dist_xy(engine);
            const auto diff_y = dist_xy(engine);
            const auto diff_h = dist_height(engine);
            const auto x = std::max(0LL, std::min(board_size-1LL, currrent_step.x + diff_x));
            const auto y = std::max(0LL, std::min(board_size-1LL, currrent_step.y + diff_y));
            const auto h = std::max(0LL, std::min(board_max_height, currrent_step.h + diff_h));

            Step new_step {x, y, h};
            actual.undo(currrent_step);
            actual.step(new_step);
            const auto new_score = actual.diff(expected);

            if (new_score > prev_score) {
                actual.undo(new_step);
                actual.step(currrent_step);
            } else {
                actual_steps.at(i) = new_step;
                prev_score = new_score;
            }
        }
    }

    os << actual_steps.size() << "\n";
    for(const auto& step : actual_steps) {
        step.print(os);
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
