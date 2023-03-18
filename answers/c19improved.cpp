// https://atcoder.jp/contests/tessoku-book/submissions/39811801
#include <algorithm>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

template <typename Element=long long int>
struct SegmentTree {
    SegmentTree(size_t size) {
        static_assert(sizeof(size) == 8);
        n_levels_ = 64 - __builtin_clzll(size - 1);
        decltype(n_leaves_) n_leaves {1};
        n_leaves <<= n_levels_;
        n_leaves_ = n_leaves;

        elements_ = std::vector<Element>(n_leaves * 2, Element{});
        top_ = n_leaves * 2 - 2;

        size_t start {0};
        size_t width {n_leaves_};
        for(size_t i{0}; i<=n_levels_; ++i) {
            starts_.push_back(start);
            start += width;
            width >>= 1;
        }
    }

    void update(size_t index, Element value) {
        elements_.at(index) = value;
        size_t start {0};
        size_t width {n_leaves_};
        size_t offset {index};
        for(;;) {
            const auto next_width = (width >> 1);
            if (next_width == 0) {
                break;
            }
            const auto neighbor = offset ^ 1;
            const auto left = elements_.at(start + offset);
            const auto right = elements_.at(start + neighbor);
            start += width;
            width = next_width;
            offset >>= 1;
            elements_.at(start + offset) = std::min(left, right);
        }
    }

    Element find_detail(size_t left, size_t right, size_t level) {
        if (level == 0) {
            return elements_.at(left);
        }

        size_t span {1};
        span <<= level;

        const auto offset = left >> level;
        const auto range_begin = offset * span;
        const auto range_end = range_begin + span - 1;
        if ((left == range_begin) && (right == range_end)) {
            return elements_.at(starts_.at(level) + offset);
        }

        size_t half_span {1};
        half_span <<= (level - 1);
        auto range_center = range_begin + half_span;
        auto range_center_prev = range_center - 1;

        if ((right < range_center) || (left >= range_center)) {
            return find_detail(left, right, level - 1);
        }
        return std::min(find_detail(left, range_center_prev, level - 1),
                        find_detail(range_center, right, level - 1));
    }

    Element find(size_t left, size_t right) {
        auto param_left = left;
        auto param_right = right;
        if (param_left > param_right) {
            std::swap(param_left, param_right);
        }
        return find_detail(param_left, param_right, n_levels_);
    }

    void print(std::ostream& os) const {
        size_t width {n_leaves_};
        size_t start {0};
        while(width > 0) {
            for(size_t i{0}; i<width; ++i) {
                os << elements_.at(start + i) << ",";
            }
            os << "\n";
            start += width;
            width = (width >> 1);
        }
    }

    size_t n_levels_ {0};
    size_t n_leaves_ {0};
    size_t top_ {0};
    std::vector<Element> elements_;
    std::vector<size_t> starts_;
};

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    constexpr Num inf = 1000000000000000LL;

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

    bool out {false};
    Num prev_position {0};
    SegmentTree<Num> segtree(l);

    for(decltype(l) position{1}; position<=l; ++position) {
        const bool can_stop = prices.at(position).has_value();
        const Num value = (can_stop) ? prices.at(position).value() : inf;
        segtree.update(position, value);
        if (can_stop) {
            out |= ((position - prev_position) > k);
            prev_position = position;
        }
    }

    out |= ((l - prev_position) > k);
    if (out) {
        os << "-1\n";
        return;
    }

    Num max_fuel = l - k;
    for(decltype(max_fuel) position{1}; position<=max_fuel; ++position) {
        const auto value = segtree.find(position, position + k - 1);
        segtree.update(position, value);
    }

    Num total{0};
    for(decltype(max_fuel) position{1}; position<=max_fuel; ++position) {
        total += segtree.find(position, position);
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
