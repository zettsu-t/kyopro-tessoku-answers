// https://atcoder.jp/contests/tessoku-book/submissions/39360180
#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace {
    using Num = long long int;
    constexpr Num infinite_time = 1000000000000LL;
    constexpr Num upper_connections = 1000000LL;

    struct Flight {
        Num from {0};
        Num to {0};
        Num departure {0};
        Num arrival {0};
        bool operator<(const Flight& rhs) const {
            return std::tie(departure, arrival, from, to) <
                std::tie(rhs.departure, rhs.arrival, rhs.from, rhs.to);
        }
    };

    struct Direct {
        Num from {0};
        Num to {0};
    };

    struct Connection {
        Num timestamp {0};
        Num count {0};
        bool operator<(const Connection& rhs) const {
            return std::tie(timestamp, count) < std::tie(rhs.timestamp, rhs.count);
        }
    };
}

struct {
    bool operator()(const Flight& lhs, const Flight& rhs) const {
        return std::tie(lhs.arrival, lhs.departure, lhs.from, lhs.to) <
            std::tie(rhs.arrival, rhs.departure, rhs.from, rhs.to);
    }
} lessArrival;

template<>
struct std::hash<Direct> {
    size_t operator()(const Direct& x) const {
        size_t value {0};
        value = x.from * upper_connections + x.to;
        return value;
    }
};

template<>
struct std::equal_to<Direct> {
    bool operator()(const Direct& lhs, const Direct& rhs) const {
        return (lhs.from == rhs.from) && (lhs.to == rhs.to);
    }
};

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num m {0};
    Num k {0};
    is >> n >> m >> k;

    std::unordered_map<Direct, std::vector<Flight>> flights_direct;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num s {0};
        Num b {0};
        Num t {0};
        is >> a >> s >> b >> t;

        Direct direct{a, b};
        Flight flight {a, b, s, t};
        if (flights_direct.find(direct) != flights_direct.end()) {
            flights_direct[direct].push_back(flight);
        } else {
            std::vector<Flight> tos{flight};
            flights_direct[direct] = tos;
        }
    }

    std::vector<Flight> flights;
    for(auto&& element : flights_direct) {
        auto& fs = element.second;
        if (fs.empty()) {
            continue;
        }

        std::sort(fs.begin(), fs.end());
        auto size = fs.size();
        std::optional<Flight> prev = fs.at(0);

        for(decltype(size) i{1}; i<size; ++i) {
            const auto& current = fs.at(i);
            const auto prev_flight = prev.value();
            if (prev_flight.arrival < current.arrival) {
                flights.push_back(prev_flight);
                prev.reset();
            }
            prev = current;
        }

        if (prev.has_value()) {
            flights.push_back(prev.value());
        }
    }

    std::sort(flights.begin(), flights.end(), lessArrival);
    std::unordered_map<Num, std::vector<Connection>> arrivals;

    for(const auto& flight : flights) {
        const auto& from = flight.from;
        const auto& to = flight.to;
        const auto deadline = flight.departure - k;
        const Connection prev{deadline, infinite_time};

        Num count {0};
        if (arrivals.find(from) != arrivals.end()) {
            const auto& vec = arrivals[from];
            if (!vec.empty()) {
                auto it_upper = std::upper_bound(vec.begin(), vec.end(), prev);
                for(auto it = vec.begin(); (it != vec.end()) && (it != it_upper); ++it) {
                    if (it->timestamp > deadline) {
                        break;
                    }
                    count = std::max(count, it->count);
                }
            }
        }

        Connection arrival{flight.arrival, count + 1};
        if (arrivals.find(to) == arrivals.end()) {
            std::vector<Connection> vec{arrival};
            arrivals[to] = vec;
        } else {
            arrivals[to].push_back(arrival);
        }
    }

    Num answer {1};
    for(const auto& element : arrivals) {
        for(const auto& vec : element.second) {
            answer = std::max(answer, vec.count);
        }
    }
    os << answer << "\n";
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
