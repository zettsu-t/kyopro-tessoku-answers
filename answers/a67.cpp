// https://atcoder.jp/contests/tessoku-book/submissions/39898015
#include <algorithm>
#include <iostream>
#include <variant>
#include <vector>

struct UnionFindTree {
    using Num = long long int;
    struct None {};
    struct Root {
        Num id_ {0};
        size_t size_ {0};
    };
    using Id = Num;
    using Node = std::variant<None, Root, Id>;
    using Tree = std::vector<Node>;
    Tree tree_;

    UnionFindTree(Num size) {
        Tree tree(size, None{});
        std::swap(tree_, tree);
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
            tree_.at(x) = Root{x, 2};
            tree_.at(y) = x;
            return;
        }

        if (std::holds_alternative<None>(x_top) && std::holds_alternative<Root>(y_top)) {
            auto& y_root = std::get<Root>(y_top);
            tree_.at(x) = y;
            y_root.size_ += 1;
            compress(x);
            return;
        }

        if (std::holds_alternative<Root>(x_top) && std::holds_alternative<None>(y_top)) {
            auto& x_root = std::get<Root>(x_top);
            tree_.at(y) = x;
            x_root.size_ += 1;
            compress(y);
            return;
        }

        auto& x_root = std::get<Root>(x_top);
        auto& y_root = std::get<Root>(y_top);
        const auto x_root_id = x_root.id_;
        const auto y_root_id = y_root.id_;
        const auto x_size = x_root.size_;
        const auto y_size = y_root.size_;

        if (x_size < y_size) {
            tree_.at(x_root_id) = y_root_id;
            y_root.size_ += x_size;
            compress(x_root_id);
        } else {
            tree_.at(y_root_id) = x_root_id;
            x_root.size_ += y_size;
            compress(y_root_id);
        }
    }
};

struct Edge {
    using Num = UnionFindTree::Num;
    Num weight {0};
    Num a {0};
    Num b {0};
    bool operator<(const Edge& rhs) const {
        return std::tie(weight, a, b) < std::tie(rhs.weight, rhs.a, rhs.b);
    }
};

void solve(std::istream& is, std::ostream& os) {
    using Num = UnionFindTree::Num;
    Num n {0};
    size_t m {0};
    is >> n >> m;

    std::vector<Edge> all_edges;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        Num c {0};
        is >> a >> b >> c;
        all_edges.push_back(Edge{c, a, b});
    }
    std::sort(all_edges.begin(), all_edges.end());

    UnionFindTree tree(n+1);
    Num total {0};
    Num count {0};
    for(const auto& edge : all_edges) {
        Num from {edge.a};
        Num to {edge.b};

        if (!tree.common(from, to)) {
            tree.connect(from, to);
            total += edge.weight;
            ++count;
        }

        if (count == (n-1)) {
            break;
        }
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
