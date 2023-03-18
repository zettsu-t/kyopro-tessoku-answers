// https://atcoder.jp/contests/tessoku-book/submissions/39826425
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
#include <variant>
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

struct UnionFindTree {
    using Num = long long int;
    struct None {};
    struct Root {
        Num id_ {0};
    };
    using Id = Num;
    using Node = std::variant<None, Root, Id>;
    using Tree = std::vector<Node>;
    using TreeSize = std::vector<size_t>;

    UnionFindTree(Num size) {
        Tree tree(size, None{});
        TreeSize tree_size(size, 0);
        std::swap(tree_, tree);
        std::swap(tree_size_, tree_size);
    }

    static bool compare(const Node& lhs, const Node& rhs) {
        if (std::holds_alternative<Root>(lhs) && std::holds_alternative<Root>(rhs)) {
            return std::get<Root>(lhs).id_ == std::get<Root>(rhs).id_;
        }

        return false;
    }

    Node find(Num x) const {
        auto current = x;

        for (;;) {
            const auto& cell = tree_.at(current);
            if (std::holds_alternative<None>(cell)) {
                return cell;
            }

            if (std::holds_alternative<Root>(cell)) {
                return cell;
            }

            current = std::get<Id>(cell);
        }

        Node none{None{}};
        return none;
    }

    void compress(Num x) {
        auto current = x;
        std::vector<Num> chain;

        for (;;) {
            const auto& cell = tree_.at(current);
            if (std::holds_alternative<None>(cell)) {
                return;
            }

            if (std::holds_alternative<Root>(cell)) {
                const auto root_id = std::get<Root>(cell).id_;
                for(const auto& i : chain) {
                    tree_.at(i) = root_id;
                }
                return;
            }

            chain.push_back(current);
            current = std::get<Id>(cell);
        }

        return;
    }

    bool common(Num x, Num y) const {
        const auto x_top = find(x);
        const auto y_top = find(y);
        return compare(x_top, y_top);
    }

    void connect(Num x, Num y) {
        auto x_top = find(x);
        auto y_top = find(y);
        if (compare(x_top, y_top)) {
            return;
        }

        if (std::holds_alternative<None>(x_top) && std::holds_alternative<None>(y_top)) {
            tree_.at(x) = Root{x};
            tree_.at(y) = x;
            tree_size_.at(x) = 2;
            tree_size_.at(y) = 1;
            return;
        }

        if (std::holds_alternative<None>(x_top) && !std::holds_alternative<None>(y_top)) {
            tree_.at(x) = y;
            tree_size_.at(x) = 1;
            tree_size_.at(y) += 1;
            compress(x);
            return;
        }

        if (!std::holds_alternative<None>(x_top) && std::holds_alternative<None>(y_top)) {
            tree_.at(y) = x;
            tree_size_.at(y) = 1;
            tree_size_.at(x) += 1;
            compress(y);
            return;
        }

        const auto x_root_id = std::get<Root>(x_top).id_;
        const auto y_root_id = std::get<Root>(y_top).id_;
        const auto x_size = tree_size_.at(x_root_id);
        const auto y_size = tree_size_.at(y_root_id);

        if (x_size < y_size) {
            tree_.at(x_root_id) = y_root_id;
            tree_size_.at(y_root_id) += x_size;
            compress(x_root_id);
        } else {
            tree_.at(y_root_id) = x_root_id;
            tree_size_.at(x_root_id) += y_size;
            compress(y_root_id);
        }
    }

    Tree tree_;
    TreeSize tree_size_;
};

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
    Outline outline;
    std::vector<Block> blocks;

    Area(Num arg_id, Num arg_population, Num arg_n_employees, Num arg_normalized,
         Num max_x_pos, Num max_y_pos) :
        id(arg_id), population(arg_population), n_employees(arg_n_employees),
        normalized(arg_normalized), outline(max_x_pos, max_y_pos) {
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
    std::unordered_map<Num, Areas> adj_population;
    std::unordered_map<Num, Areas> adj_n_employees;
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

        for(auto&& neighbors : adj_normalized) {
            std::sort(neighbors.second.begin(), neighbors.second.end(),
                      [&](const auto& lhs, const auto& rhs) {
                          return std::tie(lhs.normalized, lhs.population, lhs.n_employees, lhs.id) <
                              std::tie(rhs.normalized, rhs.population, rhs.n_employees, rhs.id);
                      });
        }

        adj_population = adj_normalized;
        for(auto&& neighbors : adj_population) {
            std::sort(neighbors.second.begin(), neighbors.second.end(),
                      [&](const auto& lhs, const auto& rhs) {
                          return std::tie(lhs.population, lhs.n_employees, lhs.normalized, lhs.id) <
                              std::tie(rhs.population, rhs.n_employees, rhs.normalized, rhs.id);
                      });
        }

        adj_n_employees = adj_normalized;
        for(auto&& neighbors : adj_n_employees) {
            std::sort(neighbors.second.begin(), neighbors.second.end(),
                      [&](const auto& lhs, const auto& rhs) {
                          return std::tie(lhs.n_employees, lhs.population, lhs.normalized, lhs.id) <
                              std::tie(rhs.n_employees, rhs.population, rhs.normalized, rhs.id);
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

    const Areas& adjacent_by_population(const Num area_id) const {
        if (adj_population.find(area_id) == adj_population.end()) {
            return zero_areas;
        }
        return adj_population.at(area_id);
    }

    const Areas& adjacent_by_n_employees(const Num area_id) const {
        if (adj_n_employees.find(area_id) == adj_n_employees.end()) {
            return zero_areas;
        }
        return adj_n_employees.at(area_id);
    }

    const Areas& adjacent_by_normalized(const Num area_id) const {
        if (adj_normalized.find(area_id) == adj_normalized.end()) {
            return zero_areas;
        }
        return adj_normalized.at(area_id);
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

    void merge(const Ward& rhs) {
        for(const auto& area : rhs.areas) {
            add(area);
        }
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
    Num width_ {0};
    Num height_ {0};
    Num n_areas_ {0};
    Num n_wards_ {0};
    std::vector<InputArea> input_areas_;
    std::vector<Area> areas_;
    std::vector<Ward> wards_;
    AdjacentAreas adj_areas_;

    City(std::istream& is) : adj_areas_(1) {
        setup(is);
        connect();
    }

    void setup(std::istream& is) {
        Num n {0};
        is >> n >> n_areas_ >> n_wards_;
        width_ = n;
        height_ = n;

        const InputArea zero_input_area{0, 0};
        decltype(input_areas_) zero_input_areas(n_areas_ + 1, zero_input_area);
        std::swap(input_areas_, zero_input_areas);

        Num total_population {0};
        Num total_n_employees {0};
        Num min_population {INF};
        Num min_n_employees {INF};
        for(decltype(n_areas_) i{1}; i<=n_areas_; ++i) {
            Num population {0};
            Num n_employees {0};
            is >> population >> n_employees;

            total_population += population;
            total_n_employees += n_employees;
            min_population = std::min(min_population, population);
            min_n_employees = std::min(min_n_employees, n_employees);

            InputArea input_area {population, n_employees};
            input_areas_.at(i) = std::move(input_area);
        }

        Num scale_population {NORMALIZED_BASE};
        Num scale_n_employees {NORMALIZED_BASE};
        if (min_population > min_n_employees) {
            scale_population = NORMALIZED_BASE;
            scale_n_employees = min_population * NORMALIZED_BASE;
            scale_n_employees /= min_n_employees;
        } else {
            scale_n_employees = NORMALIZED_BASE;
            scale_population = min_n_employees * NORMALIZED_BASE;
            scale_population /= min_population;
        }

        const Area zero_area(0, 0, 0, 0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
        decltype(areas_) zero_areas(n_areas_ + 1, zero_area);
        std::swap(areas_, zero_areas);

        for(decltype(n_areas_) id{1}; id<=n_areas_; ++id) {
            const auto& input_area = input_areas_.at(id);
            Num population {input_area.population};
            Num n_employees {input_area.n_employees};
            population *= scale_population;
            n_employees *= scale_n_employees;

            const Num normalized = std::min(population, n_employees);
            Area area(id, population, n_employees, normalized, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            areas_.at(id) = std::move(area);
        }

        Ward zero_ward(0, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
        decltype(wards_) zero_wards(n_wards_, zero_ward);
        std::swap(wards_, zero_wards);

        for(decltype(n_wards_) i{0}; i<n_wards_; ++i) {
            wards_.at(i).id = i+1;
        }

        for(decltype(n) y{1}; y<=n; ++y) {
            for(decltype(n) x{1}; x<=n; ++x) {
                Num area_id {0};
                is >> area_id;
                if (area_id != OUT_OF_CITY) {
                    Block block{x, y};
                    areas_.at(area_id).add(block);
                }
            }
        }

        AdjacentAreas new_adj_areas(n_areas_);
        adj_areas_ = new_adj_areas;
    }

    void connect() {
        using Map2D = boost::multi_array<bool, 2>;
        using Map2DShape = boost::array<Map2D::index, 2>;
        const Map2DShape adj_wards_shape({n_areas_ + 1, n_areas_ + 1});
        Map2D adj_wards(adj_wards_shape);
        std::fill_n(adj_wards.data(), adj_wards.num_elements(), 0);

        std::vector<Ward> sub_wards;
        sub_wards.reserve(n_areas_);
        for(decltype(n_areas_) area_id{1}; area_id <= n_areas_; ++area_id) {
            const auto& area = areas_.at(area_id);
            Ward ward(area_id, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            ward.add(area);
            sub_wards.push_back(ward);
        }

        Num n_sub_wards = static_cast<decltype(n_wards_)>(sub_wards.size());
        for(decltype(n_sub_wards) left_index{0}; left_index < n_sub_wards; ++left_index) {
            for(decltype(n_sub_wards) right_index{0}; right_index < n_sub_wards; ++right_index) {
                const auto left_id = sub_wards.at(left_index).id;
                const auto right_id = sub_wards.at(right_index).id;
                if (left_id == right_id) {
                    continue;
                }

                const bool adj = areas_.at(left_id).adjacent(areas_.at(right_id));
                adj_wards[left_id][right_id] = adj;
                adj_wards[right_id][left_id] = adj;
            }
        }

        UnionFindTree ward_tree(n_areas_ * 2);
        while(n_sub_wards > n_wards_) {
            for(decltype(n_wards_) source_index {0};
                (source_index < n_areas_) && (n_sub_wards > n_wards_);
                ++source_index) {
                auto& source = sub_wards.at(source_index);
                if (source.id == OUT_OF_CITY) {
                    continue;
                }

                Num min_normalized {1000000000000000LL};
                std::optional<Num> ward_index;
                for(decltype(source_index) other_index {0};
                    other_index < n_areas_;
                    ++other_index) {

                    auto& other = sub_wards.at(other_index);
                    if ((other.id == OUT_OF_CITY) || (source.id == other.id)) {
                        continue;
                    }

                    if (ward_tree.common(source.id, other.id)) {
                        continue;
                    }

                    if (!adj_wards[source.id][other.id] & !adj_wards[other.id][source.id]) {
                        continue;
                    }

                    const Num normalized = std::min(other.population, other.n_employees);
                    if (min_normalized > normalized) {
                        ward_index = other_index;
                        min_normalized = normalized;
                    }
                }

                if (!ward_index.has_value()) {
                    continue;
                }

                auto& target = sub_wards.at(ward_index.value());
                ward_tree.connect(source.id, target.id);
                source.merge(target);
                for(decltype(n_areas_) i{0}; i<n_areas_; ++i) {
                    const auto id = sub_wards.at(i).id;
                    if (id == OUT_OF_CITY) {
                        continue;
                    }
                    const bool connected = adj_wards[source.id][id] | adj_wards[id][source.id] |
                        adj_wards[target.id][id] | adj_wards[id][target.id];
                    adj_wards[source.id][id] |= connected;
                    adj_wards[target.id][id] |= connected;
                    adj_wards[id][source.id] |= connected;
                    adj_wards[id][target.id] |= connected;
                }

                target.id = OUT_OF_CITY;
                --n_sub_wards;
            }
        }

        std::vector<Ward> wards;
        Num ward_serial{1};
        for(decltype(n_areas_) i{0}; i<n_areas_; ++i) {
            auto ward = sub_wards.at(i);
            if (ward.id == OUT_OF_CITY) {
                continue;
            }
            ward.id = ward_serial;
            wards.push_back(ward);
            ++ward_serial;
        }
        std::swap(wards_, wards);
    }

    void print(std::ostream& os) const {
        std::vector<Num> area_to_ward(n_areas_ + 1, 0);

        for(const auto& ward : wards_) {
            for(const auto& area : ward.get_areas()) {
                area_to_ward.at(area.id) = ward.id;
            }
        }

        for(decltype(n_areas_) i{1}; i<=n_areas_; ++i) {
            os << area_to_ward.at(i) << "\n";
        }

        return;
    }

    Num score() const {
        std::ostringstream os;
        print(os);
        std::istringstream is(os.str());

        std::unordered_multimap<Num, Num> actual_wards;
        for(Num area_id{1}; area_id<=n_areas_; ++area_id) {
            Num ward_id{0};
            is >> ward_id;

            if ((ward_id <= 0) || (ward_id > n_wards_)) {
                return 0;
            }
            actual_wards.insert(std::make_pair(ward_id, area_id));
        }

        bool connected = true;
        Num max_population {0};
        Num min_population {INF};
        Num max_n_employees {0};
        Num min_n_employees {INF};

        for(Num ward_id{1}; ward_id<=n_wards_; ++ward_id) {
            if (actual_wards.find(ward_id) == actual_wards.end()) {
                return 0;
            }

            Ward ward(ward_id, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            Num population {0};
            Num n_employees {0};
            auto its = actual_wards.equal_range(ward_id);
            for(auto it = its.first; it != its.second; ++it) {
                const auto area_id = it->second;
                const auto& input_area = input_areas_.at(area_id);
                population += input_area.population;
                n_employees += input_area.n_employees;
                ward.add(areas_.at(area_id));
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
