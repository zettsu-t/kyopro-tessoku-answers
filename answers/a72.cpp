// https://atcoder.jp/contests/tessoku-book/submissions/38833295
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <boost/multi_array.hpp>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;

    Num h {0};
    Num w {0};
    Num k {0};
    is >> h >> w >> k;

    const MatrixShape shape {{h, w}};
    Matrix tiles(shape);
    std::fill_n(tiles.data(), tiles.num_elements(), 0LL);

    for(decltype(h) y{0}; y<h; ++y) {
        std::string s;
        is >> s;
        for(decltype(w) x{0}; x<w; ++x) {
            tiles[y][x] = (s.at(x) == '#') ? 1 : 0;
        }
    }

    Num total {0};
    Num n_combinations {1};
    n_combinations <<= h;

    for(decltype(n_combinations) masks{0}; masks<n_combinations; ++masks) {
        std::vector<Num> rows;
        for(decltype(h) i{0}; i<h; ++i) {
            Num mask {1};
            mask <<= i;
            if ((masks & mask) != 0) {
                rows.push_back(i);
            }
        }

        const Num size = static_cast<Num>(rows.size());
        if (size > k) {
            continue;
        }

        auto new_tiles = tiles;
        for (const auto& row : rows) {
            for(decltype(w) x{0}; x<w; ++x) {
                new_tiles[row][x] = 1;
            }
        }

        struct Count {
            Num x {0};
            Num count {0};
            bool operator<(const Count& rhs) const {
                return std::tie(count, x) < std::tie(rhs.count, rhs.x);
            }
        };

        std::vector<Count> counts;
        std::vector<Num> count_columns(w, 0);
        for(decltype(w) x{0}; x<w; ++x) {
            counts.push_back(Count{x, 0});
            for(decltype(h) y{0}; y<h; ++y) {
                counts.at(x).count += tiles[y][x];
            }
        }

        std::sort(counts.begin(), counts.end());
        for(Num i=0; i<(k-size); ++i) {
            Num x = counts.at(i).x;
            for(decltype(h) y{0}; y<h; ++y) {
                new_tiles[y][x] = 1;
            }
        }

        Num local_total {0};
        for(decltype(h) y{0}; y<h; ++y) {
            for(decltype(w) x{0}; x<w; ++x) {
                local_total += new_tiles[y][x];
            }
        }

        total = std::max(total, local_total);
    }

    os << total << "\n";
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
