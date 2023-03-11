// https://atcoder.jp/contests/tessoku-book/submissions/39586949
#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr Num out_of_city {0};
    constexpr Num inf {1000000000000LL};
}

struct Block {
    Num x {0};
    Num y {0};
};

struct {
    bool operator()(const Block& lhs, const Block& rhs) const {
        return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }
} lessX;

struct {
    bool operator()(const Block& lhs, const Block& rhs) const {
        return std::tie(lhs.y, lhs.x) < std::tie(rhs.y, rhs.x);
    }
} lessY;

struct Area {
    using SortedBlocks = std::unordered_map<Num, std::vector<Block>>;
    Num id {out_of_city};
    Num population {0};
    Num n_employees {0};
    Num min_x {0};
    Num max_x {0};
    Num min_y {0};
    Num max_y {0};
    SortedBlocks x_blocks;
    SortedBlocks y_blocks;

    Area(Num arg_id, Num arg_population, Num arg_n_employees) :
        id(arg_id), population(arg_population), n_employees(arg_n_employees) {
        return;
    }

    void add(const Block& block) {
        if (x_blocks.empty()) {
            min_x = block.x;
            max_x = block.x;
            min_y = block.y;
            max_y = block.y;
        } else {
            min_x = std::min(min_x, block.x);
            max_x = std::max(max_x, block.x);
            min_y = std::min(min_y, block.y);
            max_y = std::max(max_y, block.y);
        }

        if (x_blocks.find(block.x) == x_blocks.end()) {
            std::vector<Block> zero;
            x_blocks[block.x] = zero;
        }
        x_blocks[block.x].push_back(block);

        if (y_blocks.find(block.y) == y_blocks.end()) {
            std::vector<Block> zero;
            y_blocks[block.y] = zero;
        }
        y_blocks[block.y].push_back(block);

        return;
    }

    void sort() {
        for(auto&& block : x_blocks) {
            std::sort(block.second.begin(), block.second.end(), lessY);
        }

        for(auto&& block : y_blocks) {
            std::sort(block.second.begin(), block.second.end(), lessX);
        }
    }

    bool adjacent_detail(const Area& rhs) const {
        for(const auto& blocks : x_blocks) {
            for(const auto& block : blocks.second) {
                const auto target_x = block.x;
                const auto target_y = block.y;
                const auto left = target_x - 1LL;
                const auto right = target_x + 1LL;
                const auto top = target_y - 1LL;
                const auto bottom = target_y + 1LL;

                if (rhs.x_blocks.find(target_x) != rhs.x_blocks.end()) {
                    for(const auto& other : rhs.x_blocks.at(target_x)) {
                        if ((top == other.y) || (bottom == other.y)) {
                            return true;
                        }
                    }
                }

                if (rhs.y_blocks.find(target_y) != rhs.y_blocks.end()) {
                    for(const auto& other : rhs.y_blocks.at(target_y)) {
                        if ((left == other.x) || (right == other.x)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool adjacent(const Area& rhs) const {
        if ((max_x + 1) < rhs.min_x) {
            return false;
        }

        if ((rhs.max_x + 1) < min_x) {
            return false;
        }

        if ((max_y + 1) < rhs.min_y) {
            return false;
        }

        if ((rhs.max_y + 1) < min_y) {
            return false;
        }

        return adjacent_detail(rhs);
    }
};

struct Ward {
    using Areas = std::list<Area>;
    Num id {out_of_city};
    Num min_x {0};
    Num max_x {0};
    Num min_y {0};
    Num max_y {0};
    Num population {0};
    Num n_employees {0};
    Areas areas;

    Ward(Num ward_number) : id (ward_number) {
        return;
    }

    void add(const Area& area) {
        if (areas.empty()) {
            min_x = area.min_x;
            max_x = area.max_x;
            min_y = area.min_y;
            max_y = area.max_y;
        } else {
            min_x = std::min(min_x, area.min_x);
            max_x = std::max(max_x, area.max_x);
            min_y = std::min(min_y, area.min_y);
            max_y = std::max(max_y, area.max_y);
        }

        population += area.population;
        n_employees += area.n_employees;
        areas.push_back(area);
        return;
    }

    void remove(const Area& area) {
        auto it = std::find_if(areas.begin(), areas.end(), [&](const auto& x) { return x.id == area.id; });
        if (it == areas.end()) {
            return;
        }

        population -= area.population;
        n_employees -= area.n_employees;
        areas.erase(it);
        return;
    }

    bool adjacent_detail(const Ward& rhs) const {
        for(const auto& area : areas) {
            for(const auto& other : rhs.areas) {
                if (area.adjacent(other)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool adjacent(const Ward& rhs) const {
        if ((max_x + 1) < rhs.min_x) {
            return false;
        }

        if ((rhs.max_x + 1) < min_x) {
            return false;
        }

        if ((max_y + 1) < rhs.min_y) {
            return false;
        }

        if ((rhs.max_y + 1) < min_y) {
            return false;
        }

        return adjacent_detail(rhs);
    }

    bool connected() const {
        for(const auto& left : areas) {
            bool result {false};
            for(const auto& right : areas) {
                if (left.id == right.id) {
                    continue;
                }

                if (left.adjacent(right)) {
                    result = true;
                    break;
                }
            }

            if (!result) {
                return false;
            }
        }

        return true;
    }

    const Areas& get_areas() const {
        return areas;
    }
};

struct City {
    Num width {0};
    Num height {0};
    Num n_areas {0};
    Num n_wards {0};
    std::vector<Area> areas;
    std::vector<Ward> wards;

    City(std::istream& is) {
        Num n {0};
        is >> n >> n_areas >> n_wards;
        width = n;
        height = n;

        std::vector<Area> empty_area_set(n_areas + 1, Area{0,0,0});
        std::swap(areas, empty_area_set);
        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            Num a {0};
            Num b {0};
            is >> a >> b;
            Area area(i, a, b);
            areas[i] = area;
        }

        wards.reserve(n_wards);
        for(decltype(n_wards) i{1}; i<=n_wards; ++i) {
            Ward ward(i);
            wards.push_back(ward);
        }

        for(decltype(n) y{1}; y<=n; ++y) {
            for(decltype(n) x{1}; x<=n; ++x) {
                Num area_id {0};
                is >> area_id;
                if (area_id != out_of_city) {
                    Block block{x, y};
                    areas.at(area_id).add(block);
                }
            }
        }

        for(auto&& area : areas) {
            area.sort();
        }

        initialize();
    }

    void initialize() {
        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            const auto id = i % n_wards;
            wards.at(id).add(areas[i]);
        }
    }

    void print(std::ostream& os) const {
        std::vector<Num> area_to_ward(n_areas + 1, 0);

        for(const auto& ward : wards) {
            for(const auto& area : ward.get_areas()) {
                area_to_ward.at(area.id) = ward.id;
            }
        }

        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            os << area_to_ward.at(i) << "\n";
        }

        return;
    }

    Num score() const {
        using Score = double;
        Num max_population {0};
        Num min_population {inf};
        Num max_n_employees {0};
        Num min_n_employees {inf};

        bool connected = true;
        for(const auto& ward : wards) {
            max_population = std::max(max_population, ward.population);
            min_population = std::min(min_population, ward.population);
            max_n_employees = std::max(max_n_employees, ward.n_employees);
            min_n_employees = std::min(min_n_employees, ward.n_employees);
            connected &= ward.connected();
        }

        const auto score_population = static_cast<Score>(min_population) / static_cast<Score>(max_population);
        const auto score_employees = static_cast<Score>(min_n_employees) / static_cast<Score>(max_n_employees);
        const auto base = (connected) ? 1000000 : 1000;
        const auto score = base * std::min(score_population, score_employees);
        return static_cast<Num>(score);
    }
};

void solve(std::istream& is, std::ostream& os) {
    City city(is);
    city.print(os);
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
