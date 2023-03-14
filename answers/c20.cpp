// https://atcoder.jp/contests/tessoku-book/submissions/39736350
#include <cmath>
#include <algorithm>
#include <bitset>
#include <chrono>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <boost/multi_array.hpp>

namespace {
    using Num = long long int;
    using Numeric = double;
    constexpr Num OUT_OF_CITY {0};
    constexpr Num CITY_MAX_WIDTH {50};
    constexpr Num CITY_MAX_HEIGHT {50};
    constexpr Num NORMALIZED_BASE {100000LL};
    constexpr Num INF {1000000000000LL};
}

struct Block {
    Num x {0};
    Num y {0};
};

struct Outline {
    inline static constexpr size_t size {64};
    static_assert(size >= static_cast<size_t>(CITY_MAX_WIDTH + 2));
    static_assert(size >= static_cast<size_t>(CITY_MAX_HEIGHT + 2));
    using Line = std::bitset<size>;

    std::vector<Line> v_lines;
    std::vector<Line> h_lines;
    Num min_x {static_cast<decltype(min_x)>(size)-1};
    Num max_x {0};
    Num min_y {static_cast<decltype(min_y)>(size)-1};
    Num max_y {0};

    Outline(Num max_x_pos, Num max_y_pos) {
        decltype(v_lines) v_zeros(size);
        std::swap(v_lines, v_zeros);

        decltype(h_lines) h_zeros(size);
        std::swap(h_lines, h_zeros);

        if ((v_lines.at(0).size() < static_cast<size_t>(max_x_pos + 2)) ||
            (v_lines.size() < static_cast<size_t>(max_y_pos + 1)) ||
            (h_lines.at(0).size() < static_cast<size_t>(max_y_pos + 2)) ||
            (h_lines.size() < static_cast<size_t>(max_x_pos + 1))) {
            std::terminate();
        }
    }

    void add(const Block& block) {
        const auto x = block.x;
        const auto y = block.y;
        const auto left = x;
        const auto right = x+1;
        const auto top = y;
        const auto bottom = y+1;

        Line vertical;
        vertical.set(left);
        vertical.set(right);

        Line horizontal;
        horizontal.set(top);
        horizontal.set(bottom);

        v_lines.at(top) ^= vertical;
        h_lines.at(left) ^= horizontal;
        min_x = std::min(min_x, left);
        max_x = std::max(max_x, right);
        min_y = std::min(min_y, top);
        max_y = std::max(max_y, bottom);
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
    }

    void remove(const Outline& rhs) {
        add(rhs);
    }

    bool adjacent(const Outline& rhs) const {
        auto top = std::max(min_y, rhs.min_y);
        auto bottom = std::min(max_y, rhs.max_y);
        auto left = std::max(min_x, rhs.min_x);
        auto right = std::min(max_x, rhs.max_x);

        for(decltype(top) y{top}; y<=bottom; ++y) {
            if ((v_lines.at(y) & rhs.v_lines.at(y)).count() > 0) {
                return true;
            }
        }

        for(decltype(left) x{left}; x<=right; ++x) {
            if ((h_lines.at(x) & rhs.h_lines.at(x)).count() > 0) {
                return true;
            }
        }

        return false;
    }
};

struct CityMap {
    using Element = Num;
    using Map2D = boost::multi_array<Element, 2>;
    using Map2DShape = boost::array<Map2D::index, 2>;
    Map2DShape shape;
    std::unique_ptr<Map2D> area_map;

    CityMap(const CityMap& source) {
        shape = source.shape;
        std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(source.shape);
        *new_area_map = *(source.area_map);
        std::swap(area_map, new_area_map);
    }

    CityMap& operator=(const CityMap& other) {
        if (this != &other) {
            shape = other.shape;
            std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(other.shape);
            *new_area_map = *(other.area_map);
            std::swap(area_map, new_area_map);
        }

        return *this;
    }

    CityMap(Num max_x_pos, Num max_y_pos) : shape({max_y_pos + 1, max_x_pos + 1}) {
        auto new_area_map = std::make_unique<Map2D>(shape);
        std::fill_n(new_area_map->data(), new_area_map->num_elements(), 0);
        std::swap(area_map, new_area_map);
    }

    size_t width() const {
        return shape[1];
    }

    size_t height() const {
        return shape[0];
    }

    void set(Num x, Num y, Element value) {
        (*area_map)[y][x] = value;
    }

    Element get(Num x, Num y) const {
        return (*area_map)[y][x];
    }

    static void visit(Map2D* visited, Num x, Num y, Num map_width, Num map_height) {
        if ((*visited)[y][x] == 0) {
            return;
        }

        (*visited)[y][x] = 0;

        if ((x+1) < map_width) {
            visit(visited, x+1, y, map_width, map_height);
        }

        if ((y+1) < map_height) {
            visit(visited, x, y+1, map_width, map_height);
        }

        if (x > 0) {
            visit(visited, x-1, y, map_width, map_height);
        }

        if (y > 0) {
            visit(visited, x, y-1, map_width, map_height);
        }

        return;
    }

    bool connected() const {
        std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(shape);
        *new_area_map = *area_map;
        auto map_width = shape[1];
        auto map_height = shape[0];

        auto visited = new_area_map.get();
        Num count{0};
        for(decltype(map_height) y{0}; y<map_height; ++y) {
            for(decltype(map_width) x{0}; x<map_width; ++x) {
                if ((*visited)[y][x] == 0) {
                    continue;
                }

                if (count > 0) {
                    return false;
                }

                visit(visited, x, y, map_width, map_height);
                ++count;
            }
        }

        return true;
    }
};

struct InputArea {
    Num population {0};
    Num n_employees {0};
};

struct Area {
    Num id {OUT_OF_CITY};
    Num population {0};
    Num n_employees {0};
    Num normalized {NORMALIZED_BASE};
    Numeric importance {0};
    Outline outline;
    std::vector<Block> blocks;

    Area(Num arg_id, Num arg_population, Num arg_n_employees, Num arg_normalized,
         Numeric arg_importance, Num max_x_pos, Num max_y_pos) :
        id(arg_id), population(arg_population), n_employees(arg_n_employees),
        normalized(arg_normalized), importance(arg_importance), outline(max_x_pos, max_y_pos) {
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

    void set_area_map(CityMap& city_map, Num value) const {
        for(const auto& block : blocks) {
            city_map.set(block.x, block.y, value);
        }
    }
};

struct AdjacentAreas {
    using AreaIds = std::vector<Num>;
    using Areas = std::vector<Area>;
    using Map2D = boost::multi_array<bool, 2>;
    using Map2DShape = boost::array<Map2D::index, 2>;

    Map2DShape shape;
    std::unique_ptr<Map2D> adj_matrix;
    std::unordered_map<Num, AreaIds> adj_map;
    std::unordered_map<Num, Areas> adj_normalized;
    std::unordered_map<Num, Areas> adj_importance;
    const AreaIds zero_area_ids;
    const Areas zero_areas;

    AdjacentAreas(const AdjacentAreas& source) {
        shape = source.shape;
        std::unique_ptr<Map2D> new_adj_matrix = std::make_unique<Map2D>(source.shape);
        *new_adj_matrix = *(source.adj_matrix);
        std::swap(adj_matrix, new_adj_matrix);
    }

    AdjacentAreas& operator=(const AdjacentAreas& other) {
        if (this != &other) {
            shape = other.shape;
            std::unique_ptr<Map2D> new_adj_matrix = std::make_unique<Map2D>(other.shape);
            *new_adj_matrix = *(other.adj_matrix);
            std::swap(adj_matrix, new_adj_matrix);
        }

        return *this;
    }

    AdjacentAreas(Num size) : shape({size + 1, size + 1}) {
        auto new_adj_matrix = std::make_unique<Map2D>(shape);
        std::fill_n(new_adj_matrix->data(), new_adj_matrix->num_elements(), 0);
        std::swap(adj_matrix, new_adj_matrix);
    }

    size_t width() const {
        return shape[1];
    }

    size_t height() const {
        return shape[0];
    }

    void set(const Area& left, const Area& right) {
        const auto left_id = left.id;
        const auto right_id = right.id;

        if (!left.adjacent(right)) {
            return;
        }

        (*adj_matrix)[left_id][right_id] = true;
        (*adj_matrix)[right_id][left_id] = true;

        if (adj_map.find(left_id) == adj_map.end()) {
            adj_map[left_id] = zero_area_ids;
        }
        adj_map[left_id].push_back(right_id);

        if (adj_map.find(right_id) == adj_map.end()) {
            adj_map[right_id] = zero_area_ids;
        }
        adj_map[right_id].push_back(left_id);
    }

    void sort(const std::vector<Area>& areas) {
        for(auto&& neighbors : adj_map) {
            const auto from = neighbors.first;
            adj_normalized[from] = zero_areas;
            for(const auto& to : neighbors.second) {
                adj_normalized[from].push_back(areas.at(to));
            }
        }

        adj_importance = adj_normalized;

        for(auto&& neighbors : adj_normalized) {
            std::sort(neighbors.second.begin(), neighbors.second.end(),
                      [&](const auto& lhs, const auto& rhs) {
                          return std::tie(lhs.normalized, lhs.population, lhs.n_employees, lhs.importance, lhs.id) <
                              std::tie(rhs.normalized, rhs.population, rhs.n_employees, rhs.importance, rhs.id);
                      });
        }

        for(auto&& neighbors : adj_importance) {
            std::sort(neighbors.second.begin(), neighbors.second.end(),
                      [&](const auto& lhs, const auto& rhs) {
                          return std::tie(lhs.importance, lhs.normalized, lhs.population, lhs.n_employees, lhs.id) <
                              std::tie(rhs.importance, rhs.normalized, rhs.population, rhs.n_employees, rhs.id);
                      });
        }
    }

    bool adjacent(const Area& left, const Area& right) const {
        return (*adj_matrix)[left.id][right.id];
    }

    const AreaIds& adjacent_area_ids(const Num area_id) const {
        if (adj_map.find(area_id) == adj_map.end()) {
            return zero_area_ids;
        }
        return adj_map.at(area_id);
    }

    const Areas& adjacent_by_normalized(const Num area_id) const {
        if (adj_normalized.find(area_id) == adj_normalized.end()) {
            return zero_areas;
        }
        return adj_normalized.at(area_id);
    }

    const Areas& adjacent_by_importance(const Num area_id) const {
        if (adj_importance.find(area_id) == adj_importance.end()) {
            return zero_areas;
        }
        return adj_importance.at(area_id);
    }
};

struct Ward {
    using Areas = std::vector<Area>;
    Num id {OUT_OF_CITY};
    Num population {0};
    Num n_employees {0};
    Areas areas;
    Outline empty_outline;
    Outline outline;
    CityMap empty_area_map;
    CityMap area_map;

    Ward(Num ward_number, Num max_x_pos, Num max_y_pos) :
        id(ward_number), empty_outline(max_x_pos, max_y_pos), outline(max_x_pos, max_y_pos),
        empty_area_map(max_x_pos, max_y_pos), area_map(max_x_pos, max_y_pos) {
        return;
    }

    void add(const Area& area) {
        population += area.population;
        n_employees += area.n_employees;
        outline.add(area.outline);
        area.set_area_map(area_map, 1);
        areas.push_back(area);
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
        outline.remove(it->outline);
        it->set_area_map(area_map, 0);
        areas.erase(it);
        return;
    }

    bool adjacent(const Area& area) const {
        return outline.adjacent(area.outline);
    }

    bool adjacent(const Ward& rhs) const {
        return outline.adjacent(rhs.outline);
    }

    bool connected() const {
        return area_map.connected();
    }

    const Areas& get_areas() const {
        return areas;
    }
};

struct City {
    using RandomEngine = std::mt19937;
    using RandomDist = std::uniform_int_distribution<Num>;
    using ChosenAreas = std::vector<std::optional<Num>>;
    Num width {0};
    Num height {0};
    Num n_areas {0};
    Num n_wards {0};
    Num scale_population {NORMALIZED_BASE};
    Num scale_n_employees {NORMALIZED_BASE};
    Num average_population_area {0};
    Num average_n_employees_area {0};
    Num average_population_ward {0};
    Num average_n_employees_ward {0};
    std::vector<InputArea> input_areas;
    std::vector<Area> areas;
    std::vector<Num> sorted_area_ids;
    AdjacentAreas adj_areas;
    std::vector<Ward> wards;

    City(std::istream& is) : adj_areas(1) {
        setup(is);

        std::vector<Area> sorted_areas = areas;
        std::sort(sorted_areas.begin(), sorted_areas.end(),
                  [&](const auto& lhs, const auto& rhs) {
                      return std::tie(lhs.importance, lhs.normalized, lhs.population, lhs.n_employees, lhs.id) >
                          std::tie(rhs.importance, rhs.normalized, rhs.population, rhs.n_employees, rhs.id);
                  });

        for(const auto& area : sorted_areas) {
            const auto& id = area.id;
            if (id != OUT_OF_CITY) {
                sorted_area_ids.push_back(id);
            }
        }

        connect();
        gather();
    }

    void setup(std::istream& is) {
        Num n {0};
        is >> n >> n_areas >> n_wards;
        width = n;
        height = n;

        const InputArea zero_input_area{0, 0};
        decltype(input_areas) zero_input_areas(n_areas+1, zero_input_area);
        std::swap(input_areas, zero_input_areas);

        Num total_population {0};
        Num total_n_employees {0};
        Num min_population {INF};
        Num min_n_employees {INF};
        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            Num population {0};
            Num n_employees {0};
            is >> population >> n_employees;

            total_population += population;
            total_n_employees += n_employees;
            min_population = std::min(min_population, population);
            min_n_employees = std::min(min_n_employees, n_employees);

            InputArea input_area {population, n_employees};
            input_areas[i] = std::move(input_area);
        }

        if (min_population > min_n_employees) {
            scale_population = NORMALIZED_BASE;
            scale_n_employees = min_population * NORMALIZED_BASE;
            scale_n_employees /= min_n_employees;
        } else {
            scale_n_employees = NORMALIZED_BASE;
            scale_population = min_n_employees * NORMALIZED_BASE;
            scale_population /= min_population;
        }

        average_population_area = total_population * scale_population;
        average_population_area /= n_areas;
        average_n_employees_area = total_n_employees * scale_n_employees;
        average_n_employees_area /= n_areas;

        average_population_ward = total_population * scale_population;
        average_population_ward /= n_wards;
        average_n_employees_ward = total_n_employees * scale_n_employees;
        average_n_employees_ward /= n_wards;

        const Area zero_area(0, 0, 0, 0, 0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
        decltype(areas) zero_areas(n_areas+1, zero_area);
        std::swap(areas, zero_areas);

        for(decltype(n_areas) id{1}; id<=n_areas; ++id) {
            const auto& input_area = input_areas.at(id);
            Num population {input_area.population};
            Num n_employees {input_area.n_employees};
            population *= scale_population;
            n_employees *= scale_n_employees;

            const Num normalized = std::min(population, n_employees);
            Numeric importance = population;
            importance *= n_employees;

            Area area(id, population, n_employees, normalized, importance, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            areas[id] = std::move(area);
        }

        Ward zero_ward(0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
        decltype(wards) zero_wards(n_wards+1, zero_ward);
        std::swap(wards, zero_wards);

        for(decltype(n_wards) i{1}; i<=n_wards; ++i) {
            wards.at(i).id = i;
        }

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
    }

    void connect() {
        for(decltype(n_areas) i{1}; i<=n_areas; ++i) {
            for(decltype(n_areas) j{i+1}; j<=n_areas; ++j) {
                if (i != j) {
                    adj_areas.set(areas.at(i), areas.at(j));
                }
            }
        }

        adj_areas.sort(areas);
    }

    void gather() {
        std::random_device seed_gen;
        RandomEngine engine(seed_gen());
        RandomDist dist_area(1, n_areas);
        ChosenAreas chosen_areas(n_areas + 1);

        auto area_count = initialize_wards(chosen_areas);
        area_count = collect_areas(area_count, chosen_areas, engine);
        area_count = gather_areas(area_count, chosen_areas, engine, dist_area);
        area_count = swap_areas(area_count, chosen_areas, engine);
        fill_areas(area_count, chosen_areas);
    }

    Num initialize_wards(ChosenAreas& chosen_areas) {
        Num area_count{0};
        for(decltype(n_wards) ward_id{1}; ward_id<=n_wards; ++ward_id) {
            const auto& area_id = sorted_area_ids.at(ward_id - 1);
            chosen_areas.at(area_id) = ward_id;
            wards.at(ward_id).add(areas[area_id]);
            ++area_count;
        }

        return area_count;
    }

    Num collect_areas(Num initial_area_count, ChosenAreas& chosen_areas, RandomEngine& engine) {
        auto area_count = initial_area_count;
        Num max_loop = 10 * n_areas;

        for(decltype(max_loop) loop{0}; loop<max_loop; ++loop) {
            for(decltype(n_wards) ward_id{1}; ward_id<=n_wards; ++ward_id) {
                auto& ward = wards.at(ward_id);
                Num size = static_cast<Num>(ward.areas.size());
                RandomDist dist(static_cast<decltype(size)>(1), size);
                auto& area = ward.areas.at(dist(engine) - 1);

                const auto& neighbors = adj_areas.adjacent_by_importance(area.id);
                if (neighbors.empty()) {
                    continue;
                }

                const Num required_population = average_population_area * (size + 1) - ward.population;
                const Num required_employees = average_n_employees_area * (size + 1) - ward.n_employees;
                const bool more_population = (required_population > average_population_area);
                const bool more_employees = (required_employees > average_n_employees_area);
                if ((required_population < 0) || (required_employees < 0) ||
                    ((average_population_ward * 1) < ward.population) || ((average_n_employees_ward * 1) < ward.n_employees)) {
                    continue;
                }

                std::optional<bool> more_less;
                if (more_population & more_employees) {
                    more_less = true;
                } else if (!more_population & !more_employees) {
                    more_less = false;
                }

                const Area target_p(0, required_population, 0, 0, 0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
                auto it_p = std::lower_bound(neighbors.begin(), neighbors.end(), target_p,
                                             [&](const auto& lhs, const auto& rhs) {
                    return std::tie(lhs.population, lhs.n_employees, lhs.normalized, lhs.importance, lhs.id) <
                        std::tie(rhs.population, rhs.n_employees, rhs.normalized, rhs.importance, rhs.id);
                });

                const Area target_n(0, 0, required_employees, 0, 0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
                auto it_n = std::lower_bound(neighbors.begin(), neighbors.end(), target_n,
                                           [&](const auto& lhs, const auto& rhs) {
                    return std::tie(lhs.n_employees, lhs.population, lhs.normalized, lhs.importance, lhs.id) <
                        std::tie(rhs.n_employees, rhs.population, rhs.normalized, rhs.importance, rhs.id);
                });

                std::optional<Num> chosen_area_id;
                while(!chosen_area_id.has_value() && (it_p != neighbors.end() || it_n != neighbors.end())) {
                    if (it_p != neighbors.end()) {
                        const auto area = *it_p;
                        ++it_p;
                        if (!chosen_areas.at(area.id).has_value()) {
                            const bool matched = more_population ^ (average_population_area < areas.at(area.id).population);
                            if (matched) {
                                chosen_area_id = area.id;
                                break;
                            } else {
                                chosen_area_id = area.id;
                            }
                        }
                    }

                    if (it_n != neighbors.end()) {
                        const auto area = *it_n;
                        ++it_n;
                        if (!chosen_areas.at(area.id).has_value()) {
                            const bool matched = more_employees ^ (average_n_employees_area < areas.at(area.id).n_employees);
                            if (matched) {
                                chosen_area_id = area.id;
                                break;
                            } else {
                                if (!chosen_area_id.has_value()) {
                                    chosen_area_id = area.id;
                                } else {
                                    bool replace {true};
                                    if (more_less.has_value() && more_less.value()) {
                                        replace = areas.at(area.id).importance < areas.at(area.id).importance;
                                    } else if (more_less.has_value() && !more_less.value()) {
                                        replace = areas.at(area.id).importance > areas.at(area.id).importance;
                                    }
                                    if (replace) {
                                        chosen_area_id = area.id;
                                    }
                                }
                            }
                        }
                    }
                }

                if (chosen_area_id.has_value()) {
                    const auto id = chosen_area_id.value();
                    if (!chosen_areas.at(id).has_value()) {
                        chosen_areas.at(id) = ward_id;
                        wards.at(ward_id).add(areas.at(id));
                        ++area_count;
                    }
                }
            }
        }

        return area_count;
    }

    Num attach_area(Num initial_area_count, Num area_id, ChosenAreas& chosen_areas) {
        auto area_count = initial_area_count;

        if (!chosen_areas.at(area_id).has_value()) {
            for(decltype(n_wards) ward_id{1}; ward_id<=n_wards; ++ward_id) {
                if (wards.at(ward_id).adjacent(areas.at(area_id))) {
                    chosen_areas.at(area_id) = ward_id;
                    wards.at(ward_id).add(areas[area_id]);
                    ++area_count;
                    break;
                }
            }
        }

        return area_count;
    }

    Num gather_areas(Num initial_area_count, ChosenAreas& chosen_areas,
                     RandomEngine& engine, RandomDist& dist_area) {
        auto area_count = initial_area_count;
        Num max_loop = 100 * n_areas;

        for(decltype(max_loop) loop{0}; (loop<max_loop) && (area_count < n_areas); ++loop) {
            const auto area_id = dist_area(engine);
            if (chosen_areas.at(area_id).has_value()) {
                continue;
            }

            area_count = attach_area(area_count, area_id, chosen_areas);
        }

        return area_count;
    }

    Num fill_areas(Num initial_area_count, ChosenAreas& chosen_areas) {
        auto area_count = initial_area_count;

        const auto ward_id {n_wards};
        for(decltype(n_areas) area_id{1}; area_id<=n_areas; ++area_id) {
            if (!chosen_areas.at(area_id).has_value()) {
                wards.at(ward_id).add(areas.at(area_id));
                chosen_areas.at(area_id) = ward_id;
                ++area_count;
            }
        }

        return area_count;
    }

    Num swap_areas(Num initial_area_count, ChosenAreas& chosen_areas, RandomEngine& engine) {
        auto area_count = initial_area_count;
        constexpr double time_limit_msec = 800.0;
        std::clock_t clock_start = std::clock();

        RandomDist dist_full(1, n_areas);
        bool running {true};

        while(running) {
            std::clock_t clock_now = std::clock();
            const auto elapsed_msec = 1000.0 * ((clock_now - clock_start)) / CLOCKS_PER_SEC;
            if (elapsed_msec > time_limit_msec) {
                running = false;
                break;
            }

            const bool choose_random = dist_full(engine) < ((n_areas) / 4);
            Num min_ward_id{0};
            Num min_ward_normalized{INF};

            if (choose_random) {
                const auto area_id = dist_full(engine);
                if (!chosen_areas.at(area_id).has_value()) {
                    area_count = attach_area(area_count, area_id, chosen_areas);
                    continue;
                }

                const auto& ward = wards.at(chosen_areas.at(area_id).value());
                if ((ward.population > average_population_ward) ||
                    (ward.n_employees > average_n_employees_ward)) {
                    continue;
                }

                min_ward_id = ward.id;
            } else {
                for(const auto& ward : wards) {
                    if (ward.id == 0) {
                        continue;
                    }

                    const auto local_min = std::min(ward.population, ward.n_employees);
                    if (min_ward_normalized > local_min) {
                        min_ward_id = ward.id;
                        min_ward_normalized = local_min;
                    }
                }
            }

            if (min_ward_id == 0) {
                break;
            }

            auto& ward_left = wards.at(min_ward_id);
            const auto& left_areas = ward_left.get_areas();
            auto left_size = left_areas.size();
            if (left_size == 0) {
                continue;
            }

            Num min_area_id{0};
            Num min_area_normalized{INF};
            for(const auto& area : left_areas) {
                const auto local_min = std::min(area.population, area.n_employees);
                if (min_area_normalized > local_min) {
                    min_area_id = area.id;
                    min_area_normalized = local_min;
                }
            }

            if (min_area_id == 0) {
                break;
            }
            const Area& area_left = areas.at(min_area_id);

            const auto& right_area_ids = adj_areas.adjacent_area_ids(area_left.id);
            Num right_area_id{0};
            Num right_area_max{0};
            for(const auto& area_id : right_area_ids) {
                if (!chosen_areas.at(area_id).has_value()) {
                    area_count = attach_area(area_count, area_id, chosen_areas);
                    break;
                }

                auto& ward_id = chosen_areas.at(area_id).value();
                if (ward_left.id == ward_id) {
                    continue;
                }

                const auto& local_area = areas.at(area_id);
                const auto local_max = std::max(local_area.population, local_area.n_employees);
                if (right_area_max < local_max) {
                    right_area_id = area_id;
                    right_area_max = local_max;
                }
            }

            if (choose_random || (right_area_id == 0)) {
                right_area_id = dist_full(engine);
            }

            const Area& area_right = areas.at(right_area_id);
            if (!chosen_areas.at(area_right.id).has_value()) {
                area_count = attach_area(area_count, area_right.id, chosen_areas);
                continue;
            }
            auto& ward_right = wards.at(chosen_areas.at(area_right.id).value());
            if ((ward_left.id == ward_right.id) || (ward_left.id == 0) || (ward_right.id == 0)) {
                continue;
            }

            if ((ward_right.population < average_population_ward) ||
                (ward_right.n_employees < average_n_employees_ward)) {
                continue;
            }

            const auto left_population = ward_left.population - area_left.population + area_right.population;
            const auto left_n_employees = ward_left.n_employees - area_left.n_employees + area_right.n_employees;
            const auto right_population = ward_right.population - area_right.population + area_left.population;
            const auto right_n_employees = ward_right.n_employees - area_right.n_employees + area_left.n_employees;

            const bool swapped =
                (std::min(left_population, left_n_employees) >
                 std::min(ward_left.population, ward_left.n_employees)) &&
                (std::max(right_population, right_n_employees) <
                 std::max(ward_right.population, ward_right.n_employees));

            if (!swapped) {
                continue;
            }

            ward_left.remove(area_left);
            ward_right.remove(area_right);
            ward_left.add(area_right);
            ward_right.add(area_left);

            if (ward_left.connected() && ward_right.connected()) {
                std::swap(chosen_areas[area_left.id], chosen_areas[area_right.id]);
            } else {
                ward_left.remove(area_right);
                ward_right.remove(area_left);
                ward_left.add(area_left);
                ward_right.add(area_right);
            }
        }

        return area_count;
    }

    void print(std::ostream& os) const {
        std::vector<Num> area_to_ward(n_areas+1, 0);

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
        std::ostringstream os;
        print(os);
        std::istringstream is(os.str());

        std::unordered_multimap<Num, Num> actual_wards;
        for(Num area{1}; area<=n_areas; ++area) {
            Num ward{0};
            is >> ward;

            if ((ward <= 0) || (ward > n_wards)) {
                return 0;
            }
            actual_wards.insert(std::make_pair(ward, area));
        }

        bool connected = true;
        Num max_population {0};
        Num min_population {INF};
        Num max_n_employees {0};
        Num min_n_employees {INF};

        for(Num ward_id{1}; ward_id<=n_wards; ++ward_id) {
            if (actual_wards.find(ward_id) == actual_wards.end()) {
                return 0;
            }

            Ward ward(ward_id, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            Num population {0};
            Num n_employees {0};
            auto its = actual_wards.equal_range(ward_id);
            for(auto it = its.first; it != its.second; ++it) {
                const auto area_id = it->second;
                const auto& input_area = input_areas.at(area_id);
                population += input_area.population;
                n_employees += input_area.n_employees;
                ward.add(areas.at(area_id));
            }

            connected &= ward.connected();
            max_population = std::max(max_population, population);
            min_population = std::min(min_population, population);
            max_n_employees = std::max(max_n_employees, n_employees);
            min_n_employees = std::min(min_n_employees, n_employees);
        }

        const Numeric score_population = static_cast<Numeric>(min_population) /
            static_cast<Numeric>(max_population);
        const Numeric score_employees = static_cast<Numeric>(min_n_employees) /
            static_cast<Numeric>(max_n_employees);
        const Numeric base = (connected) ? 1000000 : 1000;
        const Numeric score = base * std::min(score_population, score_employees);
        return static_cast<Num>(std::floor(score));
    }
};

void solve(std::istream& is, std::ostream& os, bool show_score) {
    City city(is);
    city.print(os);
    if (show_score) {
        os << "Score: " << city.score() << "\n";
    }
    return;
}

int main(int argc, char* argv[]) {
    bool show_score {false};
    if ((argc > 1) && (argv[1] != nullptr)) {
        const std::string command("score");
        const std::string arg(argv[1]);
        show_score = (arg == command);
    }
    solve(std::cin, std::cout, show_score);
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
