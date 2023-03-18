// https://atcoder.jp/contests/tessoku-book/submissions/39814354
#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace {
    using Num = long long int;

    struct Flight {
        Num id {0};
        Num from {0};
        Num to {0};
        Num departure {0};
        Num arrival {0};
        Num timestamp {0};
    };
}

struct {
    bool operator()(const Flight& lhs, const Flight& rhs) const {
        return std::tie(lhs.timestamp, lhs.arrival, lhs.departure, lhs.from, lhs.to) <
            std::tie(rhs.timestamp, rhs.arrival, rhs.departure, rhs.from, rhs.to);
    }
} lessTimestamp;

void solve(std::istream& is, std::ostream& os) {
    Num n {0};
    Num m {0};
    Num k {0};
    is >> n >> m >> k;

    std::vector<Flight> flights;
    flights.reserve(m);
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num s {0};
        Num b {0};
        Num t {0};
        is >> a >> s >> b >> t;

        Num departure {s};
        Num arrival {t + k};
        Flight flight_departure {i, a, b, departure, arrival, departure};
        Flight flight_arrival {i, a, b, departure, arrival, arrival};
        flights.push_back(flight_departure);
        flights.push_back(flight_arrival);
    }

    std::sort(flights.begin(), flights.end(), lessTimestamp);
    std::vector<Num> airports(n+1, 0);
    std::vector<Num> counts(m+1, 0);

    Num answer {1};
    for(auto&& flight : flights) {
        const auto from = flight.from;
        const auto to = flight.to;

        if (flight.timestamp == flight.departure) {
            counts.at(flight.id) = airports.at(from) + 1;
        } else if (flight.timestamp == flight.arrival) {
            const auto count = std::max(airports.at(to), counts.at(flight.id));
            airports.at(to) = count;
            answer = std::max(answer, count);
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
