// https://atcoder.jp/contests/tessoku-book/submissions/38905301
#include <algorithm>
#include <iostream>
#include <set>
#include <variant>
#include <vector>

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

void solve(std::istream& is, std::ostream& os) {
    using Num = UnionFindTree::Num;
    Num n {0};
    Num m {0};
    is >> n >> m;

    struct Path {
        Num a {0};
        Num b {0};
    };

    std::vector<Path> lines;
    for(decltype(m) i{0}; i<m; ++i) {
        Num a {0};
        Num b {0};
        is >> a >> b;
        lines.push_back(Path{a, b});
    }

    Num q {0};
    is >> q;
    std::vector<Num> queries(q, 0);
    std::set<Num> all_canceled;
    std::vector<Num> canceled(q, 0);
    std::vector<Path> stations(q, Path{0, 0});

    for(decltype(q) i{0}; i<q; ++i) {
        Num c {0};
        is >> c;
        queries.at(i) = c;
        if (c == 1) {
            Num x {0};
            is >> x;
            --x;
            canceled.at(i) = x;
            all_canceled.insert(x);
        } else if (c == 2) {
            Num a {0};
            Num b {0};
            is >> a >> b;
            stations.at(i) = Path{a, b};
        }
    }

    UnionFindTree tree(n+1);
    for(decltype(m) i{0}; i<m; ++i) {
        if (all_canceled.find(i) == all_canceled.end()) {
            const auto e = lines.at(i);
            tree.connect(e.a, e.b);
        }
    }

    std::vector<bool> answers;
    for(decltype(q) i{q-1}; i>=0; --i) {
        const auto c = queries.at(i);
        if (c == 1) {
            const auto e = lines.at(canceled.at(i));
            tree.connect(e.a, e.b);
        } else if (c == 2) {
            const auto e = stations.at(i);
            answers.push_back(tree.common(e.a, e.b));
        }
    }

    std::reverse(answers.begin(), answers.end());
    for(const auto& e : answers) {
        if (e) {
            os << "Yes\n";
        } else {
            os << "No\n";
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
