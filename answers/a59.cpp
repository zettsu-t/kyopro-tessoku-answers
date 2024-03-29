// https://atcoder.jp/contests/tessoku-book/submissions/39897978
#include <algorithm>
#include <iostream>
#include <vector>

template <typename Element>
struct SegmentTree {
    using Apply = Element(*)(Element, Element);
    Apply apply_;
    SegmentTree(size_t size, Apply apply) : apply_(apply) {
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
            elements_.at(start + offset) = apply_(left, right);
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
        return apply_(find_detail(left, range_center_prev, level - 1),
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

namespace {
    using Num = long long int;
    Num apply(Num a, Num b) {
        return a + b;
    }
}

void solve(std::istream& is, std::ostream& os) {
    size_t n {0};
    Num q {0};
    is >> n >> q;

    SegmentTree<Num> segtree(n+1, apply);
    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        if (c == 1) {
            size_t position {0};
            Num value {0};
            is >> position >> value;
            segtree.update(position, value);
        } else if (c == 2) {
            size_t left {0};
            size_t right {0};
            is >> left >> right;
            --right;
            const auto answer = segtree.find(left, right);
            os << answer << "\n";
        }
    }

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
