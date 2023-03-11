// https://atcoder.jp/contests/tessoku-book/submissions/39594043
#include <algorithm>
#include <bitset>
#include <chrono>
#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
    constexpr Num OUT_OF_CITY {0};
    constexpr Num CITY_MAX_WIDTH {50};
    constexpr Num CITY_MAX_HEIGHT {50};
    constexpr Num INF {1000000000000LL};
}

struct Block {
    Num x {0};
    Num y {0};
};

struct Outline {
    inline static constexpr size_t size {64};
    using Line = std::bitset<size>;
    std::vector<Line> v_lines;
    std::vector<Line> h_lines;
    size_t min_x {size};
    size_t max_x {0};
    size_t min_y {size};
    size_t max_y {0};

    Outline(Num width, Num height) {
        decltype(v_lines) v_zeros(size);
        std::swap(v_lines, v_zeros);

        decltype(h_lines) h_zeros(size);
        std::swap(h_lines, h_zeros);

        if ((v_lines.at(0).size() < static_cast<size_t>(width + 2)) ||
            (v_lines.size() < static_cast<size_t>(height + 1)) ||
            (h_lines.at(0).size() < static_cast<size_t>(height + 2)) ||
            (h_lines.size() < static_cast<size_t>(width + 1))) {
            std::terminate();
        }
    }

    void add(const Block& block) {
        const auto x = block.x;
        const auto y = block.y;

        Line vertical;
        vertical.set(x);
        vertical.set(x+1);

        Line horizontal;
        horizontal.set(y);
        horizontal.set(y+1);

        size_t left = static_cast<decltype(min_x)>(x);
        size_t right = static_cast<decltype(max_x)>(x+1);
        size_t top = static_cast<decltype(min_y)>(y);
        size_t bottom = static_cast<decltype(max_y)>(y+1);

        v_lines.at(top) ^= vertical;
        h_lines.at(left) ^= horizontal;

        min_x = std::min(min_x, static_cast<decltype(min_x)>(left));
        max_x = std::max(max_x, static_cast<decltype(max_x)>(right));
        min_y = std::min(min_y, static_cast<decltype(min_y)>(top));
        max_y = std::max(max_y, static_cast<decltype(max_y)>(bottom));
    }

    void add(const Outline& rhs) {
        auto y_size = v_lines.size();
        for(decltype(y_size) y{0}; y < y_size; ++y) {
            v_lines.at(y) ^= rhs.v_lines.at(y);
        }

        auto x_size = h_lines.size();
        for(decltype(x_size) x{0}; x < x_size; ++x) {
            h_lines.at(x) ^= rhs.h_lines.at(x);
        }

        min_x = std::min(min_x, rhs.min_x);
        max_x = std::max(max_x, rhs.max_x);
        min_y = std::min(min_y, rhs.min_y);
        max_y = std::max(max_y, rhs.max_y);
        return;
    }

    bool adjacent(const Outline& rhs) const {
        for(decltype(min_y) y {min_y}; y <= max_y; ++y) {
            if ((v_lines.at(y) & rhs.v_lines.at(y)).count() > 0) {
                return true;
            }
        }

        for(decltype(min_x) x {min_x}; x <= max_x; ++x) {
            if ((h_lines.at(x) & rhs.h_lines.at(x)).count() > 0) {
                return true;
            }
        }

        return false;
    }
};

struct CityMap {
    using Map2D = boost::multi_array<Num, 2>;
    using Map2DShape = boost::array<Map2D::index, 2>;
    std::unique_ptr<Map2D> area_map;
    Map2DShape area_map_shape;

    CityMap(const CityMap& source) {
        area_map_shape = source.area_map_shape;
        std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(source.area_map_shape);
        *new_area_map = *(source.area_map);
        std::swap(area_map, new_area_map);
    }

    CityMap& operator=(const CityMap& other) {
        if (this != &other) {
            area_map_shape = other.area_map_shape;
            std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(other.area_map_shape);
            *new_area_map = *(other.area_map);
            std::swap(area_map, new_area_map);
        }

        return *this;
    }

    CityMap(Num max_width, Num max_height) : area_map_shape({max_height + 1, max_width + 1}) {
        auto new_area_map = std::make_unique<Map2D>(area_map_shape);
        std::fill_n(new_area_map->data(), new_area_map->num_elements(), 0);
        std::swap(area_map, new_area_map);
    }

    size_t width() const {
        return area_map_shape[1];
    }

    size_t height() const {
        return area_map_shape[0];
    }

    void set(Num x, Num y, Num value) {
        (*area_map)[y][x] = value;
    }

    Num get(Num x, Num y) const {
        return (*area_map)[y][x];
    }
};

struct Area {
    using SortedBlocks = std::unordered_map<Num, std::vector<Block>>;
    Num id {OUT_OF_CITY};
    Num population {0};
    Num n_employees {0};
    Outline outline;
    std::vector<Block> blocks;

    Area(Num arg_id, Num arg_population, Num arg_n_employees, Num max_width, Num max_height) :
        id(arg_id), population(arg_population), n_employees(arg_n_employees),
        outline(max_width, max_height) {
        return;
    }

    void add(const Block& block) {
        outline.add(block);
        blocks.push_back(block);
        return;
    }

    bool adjacent(const Area& rhs) const {
        return outline.adjacent(rhs.outline);
    }

    void set_area_map(CityMap* city_map, Num value) const {
        for(const auto& block : blocks) {
            city_map->set(block.x, block.y, value);
        }
    }
};

struct AdjacentAreas {
    using Areas = std::vector<Num>;
    using Map2D = boost::multi_array<bool, 2>;
    using Map2DShape = boost::array<Map2D::index, 2>;
    std::unique_ptr<Map2D> adj_matrix;
    Map2DShape shape;
    std::unordered_map<Num, Areas> adj_map;
    Areas zero_areas;

    AdjacentAreas(const AdjacentAreas& source) {
        shape = source.shape;
        std::unique_ptr<Map2D> matrix = std::make_unique<Map2D>(source.shape);
        *matrix = *(source.adj_matrix);
        std::swap(adj_matrix, matrix);
    }

    AdjacentAreas& operator=(const AdjacentAreas& other) {
        if (this != &other) {
            shape = other.shape;
            std::unique_ptr<Map2D> matrix = std::make_unique<Map2D>(other.shape);
            *matrix = *(other.adj_matrix);
            std::swap(adj_matrix, matrix);
        }

        return *this;
    }

    AdjacentAreas(Num size) : shape({size + 1, size + 1}) {
        auto matrix = std::make_unique<Map2D>(shape);
        std::fill_n(matrix->data(), matrix->num_elements(), 0);
        std::swap(adj_matrix, matrix);
    }

    size_t width() const {
        return shape[1];
    }

    size_t height() const {
        return shape[0];
    }

    void set(const Area& left, const Area& right) {
        if (left.adjacent(right)) {
            (*adj_matrix)[left.id][right.id] = true;
            (*adj_matrix)[right.id][left.id] = true;

            if (adj_map.find(left.id) == adj_map.end()) {
                std::vector<Num> zero;
                adj_map[left.id] = zero_areas;
            }
            adj_map[left.id].push_back(right.id);

            if (adj_map.find(right.id) == adj_map.end()) {
                std::vector<Num> zero;
                adj_map[right.id] = zero_areas;
            }
            adj_map[right.id].push_back(left.id);
        }
    }

    bool adjacent(const Area& left, const Area& right) const {
        return (*adj_matrix)[left.id][right.id];
    }

    const Areas& adjacent_areas(const Num area_id) const {
        if (adj_map.find(area_id) == adj_map.end()) {
            return zero_areas;
        }
        return adj_map.at(area_id);
    }
};

struct Ward {
    using Areas = std::list<Area>;
    Num id {OUT_OF_CITY};
    Num population {0};
    Num n_employees {0};
    Outline outline;
    Areas areas;
    CityMap area_map;

    Ward(Num ward_number, Num max_width, Num max_height) :
        id(ward_number), outline(max_width, max_height), area_map(max_width, max_height) {
        return;
    }

    void add(const Area& area) {
        population += area.population;
        n_employees += area.n_employees;
        outline.add(area.outline);
        areas.push_back(area);
        area.set_area_map(&area_map, 1);
        return;
    }

    void remove(const Area& area) {
        auto it = std::find_if(areas.begin(), areas.end(),
                               [&](const auto& x) { return x.id == area.id; });
        if (it == areas.end()) {
            return;
        }

        population -= area.population;
        n_employees -= area.n_employees;
        outline.add(it->outline);
        it->set_area_map(&area_map, 0);
        areas.erase(it);
        return;
    }

    bool adjacent(const Area& area) const {
        return outline.adjacent(area.outline);
    }

    bool adjacent(const Ward& rhs) const {
        return outline.adjacent(rhs.outline);
    }

    void visit(CityMap& visited, Num x, Num y, Num width, Num height) const {
        visited.set(x, y, 0);

        if ((x > 0) && (visited.get(x-1, y) > 0)) {
            visit(visited, x-1, y, width, height);
        }

        if (((x + 1) < width) && (visited.get(x+1, y) > 0)) {
            visit(visited, x+1, y, width, height);
        }

        if ((y > 0) && (visited.get(x, y-1) > 0)) {
            visit(visited, x, y-1, width, height);
        }

        if (((y + 1) < height) && (visited.get(x, y+1) > 0)) {
            visit(visited, x, y+1, width, height);
        }

        return;
    }

    bool connected() const {
        CityMap new_area_map(area_map);
        auto width = new_area_map.width();
        auto height = new_area_map.height();

        for(decltype(height) y{0}; y<height; ++y) {
            for(decltype(width) x{0}; x<width; ++x) {
                if (new_area_map.get(x, y) != 0) {
                    visit(new_area_map, x, y, width, height);
                    break;
                }
            }
        }

        for(decltype(height) y{0}; y<height; ++y) {
            for(decltype(width) x{0}; x<width; ++x) {
                if (new_area_map.get(x, y) != 0) {
                    return false;
                }
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
    AdjacentAreas adj_areas;

    City(std::istream& is) : adj_areas(1) {
        Num n {0};
        is >> n >> n_areas >> n_wards;
        width = n;
        height = n;

        Area zero_area {0, 0, 0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT};
        std::vector<Area> empty_area_set(n_areas + 1, zero_area);
        std::swap(areas, empty_area_set);

        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            Num a {0};
            Num b {0};
            is >> a >> b;
            Area area(i, a, b, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            areas[i] = area;
        }

        Ward zero_ward(0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
        std::vector<Ward> new_wards(n_wards+1, zero_ward);
        for(decltype(n_wards) i{1}; i<=n_wards; ++i) {
            new_wards.at(i).id = i;
        }
        std::swap(wards, new_wards);

        for(decltype(n) y{1}; y<=n; ++y) {
            for(decltype(n) x{1}; x<=n; ++x) {
                Num area_id {0};
                is >> area_id;
                if (area_id != OUT_OF_CITY) {
                    Block block{x, y};
                    areas.at(area_id).add(block);
                }
            }
        }

        AdjacentAreas new_adj_areas(n_areas);
        adj_areas = new_adj_areas;
        connect();
        gather();
    }

    void connect() {
        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            for(decltype(n_areas) j{i+1}; j<=n_areas; ++j) {
                if (i != j) {
                    adj_areas.set(areas.at(i), areas.at(j));
                }
            }
        }
    }

    void gather() {
        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::uniform_int_distribution<Num> dist_area(1, n_areas);

        std::vector<bool> chosen(n_areas + 1, false);
        Num ward_id {1};
        Num count{0};

        while(count < n_wards) {
            const auto area_id = dist_area(engine);
            if (chosen.at(area_id)) {
                continue;
            }
            chosen.at(area_id) = true;
            wards.at(ward_id).add(areas[area_id]);
            ++ward_id;
            ++count;
        }

        Num max_loop = 1000 * n_areas;
        for(decltype(max_loop) loop{0}; loop<max_loop; ++loop) {
            const auto area_id = dist_area(engine);
            if (chosen.at(area_id)) {
                continue;
            }

            for(decltype(n_wards) ward_id{1}; ward_id<=n_wards; ++ward_id) {
                if (wards.at(ward_id).adjacent(areas.at(area_id))) {
                    chosen.at(area_id) = true;
                    wards.at(ward_id).add(areas[area_id]);
                    ++count;
                    break;
                }
            }
        }

        for(decltype(n_areas) area_id{1}; area_id<=n_areas; ++area_id) {
            if (!chosen.at(area_id)) {
                wards.at(n_wards).add(areas[area_id]);
            }
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
        Num min_population {INF};
        Num max_n_employees {0};
        Num min_n_employees {INF};

        bool connected = true;
        for(decltype(n_wards) i{1}; i<=n_wards; ++i) {
            const auto& ward = wards.at(i);
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
