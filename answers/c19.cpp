// https://atcoder.jp/contests/tessoku-book/submissions/39390686
#include <algorithm>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;

    struct Station {
        Num position {0};
        Num price {0};
        Num min_fuel {0};
        Num index {0};
        bool operator<(const Station& rhs) const {
            return std::tie(position, price, min_fuel, index) <
                std::tie(rhs.position, rhs.price, rhs.min_fuel, rhs.index);
        }
    };

    struct StationPrice {
        Num price {0};
        Num index {0};
        bool operator<(const StationPrice& rhs) const {
            return std::tie(price, index) > std::tie(rhs.price, rhs.index);
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
    stations.reserve(l);
    Num prev{0};
    Num index{0};

    for(decltype(l) i{1}; i<=l; ++i) {
        if (!prices.at(i).has_value()) {
            continue;
        }

        Station station{i, prices.at(i).value(), 0, index};
        stations.push_back(station);
        if ((i - prev) > k) {
            os << "-1\n";
            return;
        }
        prev = i;
        ++index;
    }

    auto n_stations = static_cast<Num>(stations.size());
    for(decltype(n_stations) i{0}; (i+1)<n_stations; ++i) {
        stations.at(i).min_fuel = stations.at(i+1).position - stations.at(i).position;
    }

    Num cost {0};
    Num position {0};
    Num fuel {k};
    Num left {0};
    Num right {1};
    std::priority_queue<StationPrice> candidates;

    while((left + 1) < n_stations) {
        fuel -= stations.at(left).position - position;
        std::optional<Num> next;

        while(right < n_stations) {
            const auto distance = stations.at(right).position - stations.at(left).position;
            if (distance > k) {
                break;
            }

            const auto current_right = right;
            const auto current_right_price = stations.at(current_right).price;
            ++right;

            if (stations.at(left).price >= current_right_price) {
                next = current_right;
                break;
            }
            candidates.push(StationPrice{stations.at(current_right).price, stations.at(current_right).index});
        }

        Num next_station = right - 1;
        Num min_fuel = stations.at(left).min_fuel;
        if (next.has_value()) {
            next_station = next.value();
            min_fuel = stations.at(next.value()).position - stations.at(left).position;
            std::priority_queue<StationPrice> zero;
            std::swap(candidates, zero);
        } else {
            const auto min_price_station = candidates.top();
            candidates.pop();
            min_fuel = std::min(k, l - stations.at(left).position);
            next_station = min_price_station.index;
        }

        if (min_fuel > fuel) {
            cost += (min_fuel - fuel) * stations.at(left).price;
            fuel = min_fuel;
        }

        position = stations.at(left).position;
        left = next_station;
    }

    os << cost << "\n";
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
