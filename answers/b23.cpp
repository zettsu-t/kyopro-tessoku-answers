// https://atcoder.jp/contests/tessoku-book/submissions/38828979
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Numeric = double;
    using Matrix = boost::multi_array<Numeric, 2>;
    long n {0};
    is >> n;

    long n_states = 1;
    n_states <<= n;

    const boost::array<Matrix::index, 2> shape {{n_states, n}};
    Matrix distances(shape);
    std::fill_n(distances.data(), distances.num_elements(), std::numeric_limits<Numeric>::max());

    struct City {
        Numeric x {0.0};
        Numeric y {0.0};
    };
    std::vector<City> cities;

    for (decltype(n) i={0}; i<n; ++i) {
        Numeric x {0};
        Numeric y {0};
        is >> x >> y;
        cities.push_back(City{x, y});
    }

    const boost::array<Matrix::index, 2> shape_city_pairs {{n, n}};
    Matrix city_pairs(shape_city_pairs);
    std::fill_n(city_pairs.data(), city_pairs.num_elements(), 0.0);
    for (decltype(n) i={0}; i<n; ++i) {
        for (decltype(n) j={0}; j<n; ++j) {
            const auto x_diff = cities.at(i).x - cities.at(j).x;
            const auto y_diff = cities.at(i).y - cities.at(j).y;
            const auto dist = std::sqrt(x_diff * x_diff + y_diff * y_diff);
            city_pairs[i][j] = dist;
            city_pairs[j][i] = dist;
        }
    }

    for (decltype(n) i={0}; i<n; ++i) {
        size_t state {1};
        state <<= i;
        distances[state][i] = city_pairs[0][i];
    }

    for (decltype(n) n_visited={1}; n_visited<n; ++n_visited) {
        for (decltype(n_states) state={1}; state<n_states; ++state) {
            const auto n_bits = __builtin_popcountll(state);
            if (n_bits != n_visited) {
                continue;
            }

            for (decltype(n) current={0}; current<n; ++current) {
                size_t mask {1};
                mask <<= current;
                if ((state & mask) == 0) {
                    continue;
                }

                for (decltype(n) next={0}; next<n; ++next) {
                    size_t next_state = 1;
                    next_state <<= next;
                    if ((current == next) || ((next_state & state) != 0)) {
                        continue;
                    }
                    next_state |= state;
                    const auto distance = std::min(
                        distances[next_state][next],
                        distances[state][current] + city_pairs[current][next]);
                    distances[next_state][next] = distance;
                }
            }
        }
    }

    os << std::setprecision(10) << distances[n_states-1][0] << "\n";
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
