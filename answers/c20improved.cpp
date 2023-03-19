// https://atcoder.jp/contests/tessoku-book/submissions/39837418
#include <algorithm>
#include <bitset>
#include <chrono>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <set>
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
    using ArticulationPoints = std::vector<bool>;

    Num normalized_score(Num population, Num n_employees) {
        return std::min(population, n_employees);
    }
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

    Tree tree_;
    TreeSize tree_size_;

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
};

struct Block {
    Num x_ {0};
    Num y_ {0};
};

struct Outline {
    inline static constexpr size_t size {64};
    static_assert(size >= static_cast<size_t>(CITY_MAX_WIDTH + 2));
    static_assert(size >= static_cast<size_t>(CITY_MAX_HEIGHT + 2));
    using Line = std::bitset<size>;

    std::vector<Line> v_lines_;
    std::vector<Line> h_lines_;
    Num min_x_ {static_cast<decltype(min_x_)>(size)-1};
    Num max_x_ {0};
    Num min_y_ {static_cast<decltype(min_y_)>(size)-1};
    Num max_y_ {0};

    Outline(Num max_x_pos, Num max_y_pos) {
        decltype(v_lines_) v_zeros(size);
        std::swap(v_lines_, v_zeros);

        decltype(h_lines_) h_zeros(size);
        std::swap(h_lines_, h_zeros);

        if ((v_lines_.at(0).size() < static_cast<size_t>(max_x_pos + 2)) ||
            (v_lines_.size() < static_cast<size_t>(max_y_pos + 1)) ||
            (h_lines_.at(0).size() < static_cast<size_t>(max_y_pos + 2)) ||
            (h_lines_.size() < static_cast<size_t>(max_x_pos + 1))) {
            std::terminate();
        }
    }

    void add(const Block& block) {
        const auto x = block.x_;
        const auto y = block.y_;
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

        v_lines_.at(top) ^= vertical;
        h_lines_.at(left) ^= horizontal;
        min_x_ = std::min(min_x_, left);
        max_x_ = std::max(max_x_, right);
        min_y_ = std::min(min_y_, top);
        max_y_ = std::max(max_y_, bottom);
    }

    void add(const Outline& rhs) {
        auto y_size = v_lines_.size();
        for(decltype(y_size) y{0}; y < y_size; ++y) {
            v_lines_.at(y) ^= rhs.v_lines_.at(y);
        }

        auto x_size = h_lines_.size();
        for(decltype(x_size) x{0}; x < x_size; ++x) {
            h_lines_.at(x) ^= rhs.h_lines_.at(x);
        }

        min_x_ = std::min(min_x_, rhs.min_x_);
        max_x_ = std::max(max_x_, rhs.max_x_);
        min_y_ = std::min(min_y_, rhs.min_y_);
        max_y_ = std::max(max_y_, rhs.max_y_);
    }

    void remove(const Outline& rhs) {
        add(rhs);
    }

    bool adjacent(const Outline& rhs) const {
        auto top = std::max(min_y_, rhs.min_y_);
        auto bottom = std::min(max_y_, rhs.max_y_);
        auto left = std::max(min_x_, rhs.min_x_);
        auto right = std::min(max_x_, rhs.max_x_);

        for(decltype(top) y{top}; y<=bottom; ++y) {
            if ((v_lines_.at(y) & rhs.v_lines_.at(y)).count() > 0) {
                return true;
            }
        }

        for(decltype(left) x{left}; x<=right; ++x) {
            if ((h_lines_.at(x) & rhs.h_lines_.at(x)).count() > 0) {
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
    Map2DShape shape_;
    std::unique_ptr<Map2D> area_map_;

    CityMap(const CityMap& source) {
        shape_ = source.shape_;
        std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(source.shape_);
        *new_area_map = *(source.area_map_);
        std::swap(area_map_, new_area_map);
    }

    CityMap& operator=(const CityMap& other) {
        if (this != &other) {
            shape_ = other.shape_;
            std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(other.shape_);
            *new_area_map = *(other.area_map_);
            std::swap(area_map_, new_area_map);
        }

        return *this;
    }

    CityMap(Num max_x_pos, Num max_y_pos) : shape_({max_y_pos + 1, max_x_pos + 1}) {
        auto new_area_map = std::make_unique<Map2D>(shape_);
        std::fill_n(new_area_map->data(), new_area_map->num_elements(), 0);
        std::swap(area_map_, new_area_map);
    }

    size_t width() const {
        return shape_[1];
    }

    size_t height() const {
        return shape_[0];
    }

    void set(Num x, Num y, Element value) {
        (*area_map_)[y][x] = value;
    }

    Element get(Num x, Num y) const {
        return (*area_map_)[y][x];
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
        std::unique_ptr<Map2D> new_area_map = std::make_unique<Map2D>(shape_);
        *new_area_map = *area_map_;
        auto map_width = shape_[1];
        auto map_height = shape_[0];

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
    Num population_ {0};
    Num n_employees_ {0};
};

struct Area {
    Num id_ {OUT_OF_CITY};
    Num population_ {0};
    Num n_employees_ {0};
    Num normalized_ {NORMALIZED_BASE};
    Outline outline_;
    std::vector<Block> blocks_;

    Area(Num id, Num population, Num n_employees, Num normalized,
         Num max_x_pos, Num max_y_pos) :
        id_(id), population_(population), n_employees_(n_employees),
        normalized_(normalized), outline_(max_x_pos, max_y_pos) {
        return;
    }

    void add(const Block& block) {
        outline_.add(block);
        blocks_.push_back(block);
        return;
    }

    bool adjacent(const Area& rhs) const {
        return outline_.adjacent(rhs.outline_);
    }

    void set_area_map(CityMap& city_map, Num value) const {
        for(const auto& block : blocks_) {
            city_map.set(block.x_, block.y_, value);
        }
    }
};

struct Ward {
    using Areas = std::unordered_map<Num, const Area*>;
    using AreaIds = std::vector<Num>;
    Num id_ {OUT_OF_CITY};
    Num population_ {0};
    Num n_employees_ {0};
    Num normalized_ {0};
    Areas areas_;
    Outline outline_;
    CityMap area_map_;

    Ward(Num ward_number, Num max_x_pos, Num max_y_pos) :
        id_(ward_number), outline_(max_x_pos, max_y_pos), area_map_(max_x_pos, max_y_pos) {
        update();
        return;
    }

    void update() {
        normalized_  = normalized_score(population_, n_employees_);
    }

    void set_articulation_points(const std::vector<Area>& areas, ArticulationPoints& points) {
        std::vector<Num> area_id_set;
        for(const auto& left : areas_) {
            area_id_set.push_back(left.first);
        }

        std::vector<Num> non_articulation_area_ids;
        auto size = static_cast<Num>(area_id_set.size());
        for(decltype(size) excluded{0}; excluded<size; ++excluded) {
            UnionFindTree tree(size);
            for(decltype(size) left{0}; left<size; ++left) {
                if (left == excluded) {
                    continue;
                }
                for(decltype(size) right{left+1}; right<size; ++right) {
                    if (right == excluded) {
                        continue;
                    }
                    if (areas.at(area_id_set[left]).adjacent(areas.at(area_id_set[right]))) {
                        tree.connect(left, right);
                    }
                }
            }

            bool disjoint {false};
            std::optional<Num> root;
            for(decltype(size) left{0}; left<size; ++left) {
                if (left == excluded) {
                    continue;
                }

                auto node = tree.find(left);
                if (!std::holds_alternative<UnionFindTree::Root>(node)) {
                    disjoint = true;
                    break;
                }

                auto id = std::get<UnionFindTree::Root>(node).id_;
                if (root.has_value() && (root.value() != id)) {
                    disjoint = true;
                    break;
                }
                root = id;
            }

            points.at(area_id_set.at(excluded)) = disjoint;
        }
    }

    void add(const Area* area) {
        population_ += area->population_;
        n_employees_ += area->n_employees_;
        outline_.add(area->outline_);
        area->set_area_map(area_map_, 1);
        areas_[area->id_] = area;
        update();
        return;
    }

    void merge(const Ward& rhs) {
        for(const auto& area : rhs.areas_) {
            add(area.second);
        }
        return;
    }

    void remove(const Area* area) {
        auto it = areas_.find(area->id_);
        if (it == areas_.end()) {
            return;
        }

        population_ -= area->population_;
        n_employees_ -= area->n_employees_;
        outline_.remove(it->second->outline_);
        it->second->set_area_map(area_map_, 0);
        areas_.erase(it);
        update();
        return;
    }

    bool adjacent(const Area& area) const {
        return outline_.adjacent(area.outline_);
    }

    bool adjacent(const Ward& rhs) const {
        return outline_.adjacent(rhs.outline_);
    }

    bool connected() const {
        return area_map_.connected();
    }

    const Areas& get_areas() const {
        return areas_;
    }
};

struct AdjacentWards {
    using WardIds = std::vector<Num>;
    using Map2D = boost::multi_array<bool, 2>;
    using Map2DShape = boost::array<Map2D::index, 2>;

    Num size_ {0};
    Map2DShape shape_{{1, 1}};
    std::unique_ptr<Map2D> adj_matrix_;
    std::unique_ptr<UnionFindTree> adj_tree_;

    AdjacentWards(const AdjacentWards& source) {
        size_ = source.size_;
        shape_ = source.shape_;

        std::unique_ptr<Map2D> new_adj_matrix = std::make_unique<Map2D>(source.shape_);
        *new_adj_matrix = *(source.adj_matrix_);
        std::swap(adj_matrix_, new_adj_matrix);

        std::unique_ptr<UnionFindTree> new_adj_tree = std::make_unique<UnionFindTree>(size_);
        *new_adj_tree = *(source.adj_tree_);
        std::swap(adj_tree_, new_adj_tree);
    }

    AdjacentWards& operator=(const AdjacentWards& other) {
        if (this != &other) {
            size_ = other.size_;
            shape_ = other.shape_;

            std::unique_ptr<Map2D> new_adj_matrix = std::make_unique<Map2D>(other.shape_);
            *new_adj_matrix = *(other.adj_matrix_);
            std::swap(adj_matrix_, new_adj_matrix);

            std::unique_ptr<UnionFindTree> new_adj_tree = std::make_unique<UnionFindTree>(size_);
            *new_adj_tree = *(other.adj_tree_);
            std::swap(adj_tree_, new_adj_tree);
        }

        return *this;
    }

    AdjacentWards(Num size) : size_(size), shape_({size + 1, size + 1}) {
        auto new_adj_matrix = std::make_unique<Map2D>(shape_);
        std::fill_n(new_adj_matrix->data(), new_adj_matrix->num_elements(), 0);
        std::swap(adj_matrix_, new_adj_matrix);

        std::unique_ptr<UnionFindTree> new_adj_tree = std::make_unique<UnionFindTree>(size_);
        std::swap(adj_tree_, new_adj_tree);
    }

    void set_cross_areas(const std::vector<Ward>& wards) {
        for(const auto& left : wards) {
            const auto left_id = left.id_;
            for(const auto& right : wards) {
                const auto right_id = right.id_;
                if (left_id != right_id) {
                    const bool adj = left.adjacent(right);
                    (*adj_matrix_)[left_id][right_id] = adj;
                    (*adj_matrix_)[right_id][left_id] = adj;
                }
            }
        }
    }

    void connect(const std::vector<Ward>& wards, Num left_ward_id, Num right_ward_id) {
        for(const auto& other : wards) {
            const auto other_wards = other.id_;
            if (other_wards == OUT_OF_CITY) {
                continue;
            }

            const bool connected =
                (*adj_matrix_)[left_ward_id][other_wards] |
                (*adj_matrix_)[other_wards][left_ward_id] |
                (*adj_matrix_)[right_ward_id][other_wards] |
                (*adj_matrix_)[other_wards][right_ward_id];

            (*adj_matrix_)[left_ward_id][other_wards] |= connected;
            (*adj_matrix_)[other_wards][left_ward_id] |= connected;
            (*adj_matrix_)[right_ward_id][other_wards] |= connected;
            (*adj_matrix_)[other_wards][right_ward_id] |= connected;

            adj_tree_->connect(left_ward_id, right_ward_id);
        }
    }

    size_t width() const {
        return shape_[1];
    }

    size_t height() const {
        return shape_[0];
    }

    bool adjacent(Num left_ward_id, Num right_ward_id) const {
        return (*adj_matrix_)[left_ward_id][right_ward_id];
    }

    bool common(Num left_ward_id, Num right_ward_id) const {
        return adj_tree_->common(left_ward_id, right_ward_id);
    }

    WardIds adjacent_area_as_ward_ids(Num ward_id) const {
        WardIds ids;
        for(decltype(size_) other{1}; other <= size_; ++other) {
            if (adjacent(ward_id, other)) {
                ids.push_back(other);
            }
        }

        return ids;
    }
};

struct Score {
    using ConnectedSet = std::vector<std::optional<bool>>;
    std::vector<Ward> *wards_;
    ConnectedSet connected_set_;
    bool connected_{true};
    Num max_population_{0};
    Num min_population_{INF};
    Num max_n_employees_{0};
    Num min_n_employees_{INF};

    Score(std::vector<Ward>* wards, Num n_wards) : wards_(wards) {
        ConnectedSet connected_set(n_wards);
        std::swap(connected_set_, connected_set);
    }

    void invalidate(Num wards_id) {
        connected_set_.at(wards_id - 1).reset();
    }

    Num calculate(bool already_connected) {
        connected_ = true;
        max_population_ = 0;
        min_population_ = INF;
        max_n_employees_ = 0;
        min_n_employees_ = INF;
        Num index{0};

        for(const auto& ward : *wards_) {
            if (!already_connected) {
                if (!connected_set_.at(index).has_value()) {
                    connected_set_.at(index) = ward.connected();
                }

                connected_ &= connected_set_.at(index).value();
                if (!connected_) {
                    return 0;
                }
            }

            max_population_ = std::max(max_population_, ward.population_);
            min_population_ = std::min(min_population_, ward.population_);
            max_n_employees_ = std::max(max_n_employees_, ward.n_employees_);
            min_n_employees_ = std::min(min_n_employees_, ward.n_employees_);
            ++index;
        }

        const Numeric score_population = static_cast<Numeric>(min_population_) /
            static_cast<Numeric>(max_population_);
        const Numeric score_employees = static_cast<Numeric>(min_n_employees_) /
            static_cast<Numeric>(max_n_employees_);
        const Numeric base = (connected_) ? 1000000 : 1000;
        const Numeric score = base * std::min(score_population, score_employees);
        return static_cast<Num>(std::round(score));
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
    std::vector<Num> area_to_ward_;
    AdjacentWards adjacent_areas_;
    std::clock_t clock_start_;
    bool long_run_ {false};

    City(std::istream& is, bool long_run) :
        adjacent_areas_(1), clock_start_(std::clock()), long_run_(long_run) {
        setup(is);
        gather_areas();
        swap_areas();
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
            Num population {input_area.population_};
            Num n_employees {input_area.n_employees_};
            population *= scale_population;
            n_employees *= scale_n_employees;

            const Num normalized = normalized_score(population, n_employees);
            Area area(id, population, n_employees, normalized, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            areas_.at(id) = std::move(area);
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
    }

    void gather_areas() {
        AdjacentWards adjacent_wards(n_areas_ + 1);
        std::vector<Ward> sub_wards;
        sub_wards.reserve(n_areas_);

        for(decltype(n_areas_) area_id{1}; area_id <= n_areas_; ++area_id) {
            const auto& area = areas_.at(area_id);
            Ward ward(area_id, CITY_MAX_WIDTH, CITY_MAX_HEIGHT);
            ward.add(&area);
            sub_wards.push_back(ward);
        }

        adjacent_wards.set_cross_areas(sub_wards);
        adjacent_areas_ = adjacent_wards;
        Num n_sub_wards = static_cast<decltype(n_wards_)>(sub_wards.size());

        while(n_sub_wards > n_wards_) {
            for(decltype(n_wards_) source_index {0};
                (source_index < n_areas_) && (n_sub_wards > n_wards_);
                ++source_index) {
                auto& source = sub_wards.at(source_index);
                if (source.id_ == OUT_OF_CITY) {
                    continue;
                }

                Num min_normalized {INF};
                std::optional<Num> ward_index;
                for(const auto& other_ward_id : adjacent_wards.adjacent_area_as_ward_ids(source.id_)) {
                    const auto other_index = other_ward_id - 1;
                    const auto& other = sub_wards.at(other_index);
                    if ((other.id_ == OUT_OF_CITY) ||
                        (source.id_ == other_ward_id) ||
                        (other.id_ != other_ward_id)) {
                        continue;
                    }

                    if (adjacent_wards.common(source.id_, other.id_)) {
                        continue;
                    }

                    if (!adjacent_wards.adjacent(source.id_, other.id_)) {
                        continue;
                    }

                    const Num normalized = normalized_score(other.population_, other.n_employees_);
                    if (min_normalized > normalized) {
                        ward_index = other_index;
                        min_normalized = normalized;
                    }
                }

                if (!ward_index.has_value()) {
                    continue;
                }

                auto& target = sub_wards.at(ward_index.value());
                adjacent_wards.connect(sub_wards, source.id_, target.id_);
                source.merge(target);
                target.id_ = OUT_OF_CITY;
                --n_sub_wards;
            }
        }

        std::vector<Num> area_to_ward(n_areas_ + 1, OUT_OF_CITY);
        std::vector<Ward> wards;
        Num ward_serial{1};
        for(decltype(n_areas_) i{0}; i<n_areas_; ++i) {
            auto ward = sub_wards.at(i);
            if (ward.id_ == OUT_OF_CITY) {
                continue;
            }

            ward.id_ = ward_serial;
            for(const auto& area : ward.get_areas()) {
                area_to_ward.at(area.second->id_) = ward_serial;
            }
            wards.push_back(ward);
            ++ward_serial;
        }

        std::swap(wards_, wards);
        std::swap(area_to_ward_, area_to_ward);
    }

    void print(std::ostream& os) const {
        std::vector<Num> area_to_ward(n_areas_ + 1, 0);

        for(const auto& ward : wards_) {
            for(const auto& area : ward.get_areas()) {
                area_to_ward.at(area.second->id_) = ward.id_;
            }
        }

        for(decltype(n_areas_) i{1}; i<=n_areas_; ++i) {
            os << area_to_ward.at(i) << "\n";
        }

        return;
    }

    Num score_detail(const std::unordered_multimap<Num, Num>& actual_wards) const {
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
                population += input_area.population_;
                n_employees += input_area.n_employees_;
                ward.add(&areas_.at(area_id));
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
        return static_cast<Num>(std::round(score));
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

        return score_detail(actual_wards);
    }

    Num score_inner() const {
        std::unordered_multimap<Num, Num> actual_wards;

        for(decltype(n_areas_) area_id{1}; area_id <= n_areas_; ++area_id) {
            const auto ward_id = area_to_ward_.at(area_id);
            if ((ward_id <= 0) || (ward_id > n_wards_)) {
                return 0;
            }
            actual_wards.insert(std::make_pair(ward_id, area_id));
        }

        return score_detail(actual_wards);
    }

    void sort_wards(std::vector<Num>& ward_ids) const {
        std::sort(ward_ids.begin(), ward_ids.end(),
                  [&](const auto& lhs, const auto& rhs) {
                      return wards_.at(lhs - 1).normalized_ < wards_.at(rhs - 1).normalized_;
                  });
    }

    void swap_areas() {
        ArticulationPoints articulation_points(n_areas_ + 1, false);
        for(decltype(n_areas_) i{0}; i<n_wards_; ++i) {
            wards_.at(i).set_articulation_points(areas_, articulation_points);
        }

        std::vector<Num> sorted_ward_ids;
        sorted_ward_ids.reserve(n_wards_);
        for(Num ward_id{1}; ward_id<=n_wards_; ++ward_id) {
            sorted_ward_ids.push_back(ward_id);
        }
        sort_wards(sorted_ward_ids);

        std::random_device seed_gen;
        RandomEngine engine(seed_gen());
        RandomDist dist_area(1, n_areas_);
        RandomDist dist_ward(1, n_wards_);

        constexpr double time_limit_msec = 900.0;
        if (long_run_) {
            clock_start_ = std::clock();
        }

        Score score(&wards_, n_wards_);
        Num total_count {0};
        Num loop_count {0};
        Num loop_unit {1};
        loop_unit <<= 12;
        Num score_before = score.calculate(false);

        for(;;) {
            ++total_count;
            ++loop_count;
            if (loop_count > loop_unit) {
                std::clock_t clock_now = std::clock();
                const auto elapsed_msec = 1000.0 * ((clock_now - clock_start_)) / CLOCKS_PER_SEC;
                if (elapsed_msec > time_limit_msec) {
                    break;
                }
                loop_count = 0;
            }

            Num area_left_id {0};
            Num ward_left_id {0};
            Num ward_right_id {0};
            area_left_id = dist_area(engine);
            if (articulation_points.at(area_left_id)) {
                continue;
            }
            ward_left_id = area_to_ward_.at(area_left_id);

            for(Num i=0; i<1; ++i) {
                const auto& candidate_areas = adjacent_areas_.adjacent_area_as_ward_ids(area_left_id);
                auto candidate_size = candidate_areas.size();
                if (candidate_size == 0) {
                    break;
                }

                RandomDist dist(0, candidate_size - 1);
                ward_right_id = area_to_ward_.at(candidate_areas.at(dist(engine)));
                if (ward_left_id != ward_right_id) {
                    break;
                }
            }

            if ((ward_left_id == ward_right_id) || (ward_right_id == 0)) {
                continue;
            }

            const auto& area_left = areas_.at(area_left_id);
            auto& ward_left = wards_.at(ward_left_id - 1);
            auto& ward_right = wards_.at(ward_right_id - 1);
            if (ward_left.get_areas().size() <= 1) {
                continue;
            }

            area_to_ward_.at(area_left_id) = ward_right_id;
            ward_left.remove(&area_left);
            if (!ward_left.connected()) {
                ward_left.add(&area_left);
                area_to_ward_.at(area_left_id) = ward_left_id;
                continue;
            }

            ward_right.add(&area_left);
            const Num score_after = score.calculate(true);

            if (score_before < score_after) {
                sort_wards(sorted_ward_ids);
                ward_left.set_articulation_points(areas_, articulation_points);
                ward_right.set_articulation_points(areas_, articulation_points);
                score_before = score_after;
            } else {
                ward_right.remove(&area_left);
                ward_left.add(&area_left);
                area_to_ward_.at(area_left_id) = ward_left_id;
            }
        }
    }
};

void solve(std::istream& is, std::ostream& os, bool show_score) {
    City city(is, show_score);
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
