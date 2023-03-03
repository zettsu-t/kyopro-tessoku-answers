// https://atcoder.jp/contests/tessoku-book/submissions/39372638
#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <tuple>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    constexpr Num inf = std::numeric_limits<Num>::max() / 2;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;

    struct Station {
        Num position {0};
        Num price {0};
        bool operator<(const Station& rhs) const {
            return std::tie(position, price) < std::tie(rhs.position, rhs.price);
        }
    };

    Num n {0};
    Num l {0};
    Num k {0};
    is >> n >> l >> k;

    std::vector<std::optional<Num>> prices(l+1);
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        Num c {0};
        is >> a >> c;

        Num price {c};
        if (prices.at(a).has_value()) {
            price = std::min(price, prices.at(a).value());
        }
        prices.at(a) = price;
    }
    prices.at(l) = 0;

    std::vector<Station> stations;
    bool failed{false};
    Num prev{0};

    for(decltype(l) i{1}; i<=l; ++i) {
        if (!prices.at(i).has_value()) {
            continue;
        }

        Station station{i, prices.at(i).value()};
        stations.push_back(station);
        failed |= ((i - prev) > k);
        prev = i;
    }

    if (failed) {
        os << "-1\n";
        return;
    }

    Num n_stations = static_cast<Num>(stations.size());
    const MatrixShape shape {{n_stations, k+1}};
    Matrix cost(shape);
    std::fill_n(cost.data(), cost.num_elements(), inf);
    prev = 0;

    for(decltype(n_stations) i{0}; i<n_stations; ++i) {
        const auto& station = stations.at(i);
        const auto current = station.position;
        const auto price = station.price;
        const auto distance = current - prev;

        for(decltype(k) fuel{0}; fuel<=k; ++fuel) {
            if (i == 0) {
                const auto remaining = k - distance;
                cost[i][fuel] = (fuel < remaining) ? inf : ((fuel - remaining) * price);
            } else {
                Num min_cost{inf};
                for(decltype(k) prev_fuel{distance}; prev_fuel<=k; ++prev_fuel) {
                    const auto remaining = prev_fuel - distance;
                    if (fuel >= remaining) {
                        const Num local_cost = cost[i-1][prev_fuel] + (fuel - remaining) * price;
                        min_cost = std::min(min_cost, local_cost);
                    }
                }
                cost[i][fuel] = min_cost;
            }
        }
        prev = current;
    }

    os << cost[n_stations-1][0] << "\n";
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
