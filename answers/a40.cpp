// https://atcoder.jp/contests/tessoku-book/submissions/38827504
#include <iostream>
#include <unordered_map>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    is >> n;

    std::unordered_map<Num, Num> edges;
    for(decltype(n) i{0}; i<n; ++i) {
        Num a {0};
        is >> a;
        if (edges.find(a) != edges.end()) {
            edges[a] += 1;
        } else {
            edges[a] = 1;
        }
    }

    Num count {0};
    for(const auto& p : edges) {
        Num n_edges = p.second;
        Num local_count {0};
        if (n_edges >= 3) {
            local_count = n_edges * (n_edges-1) * (n_edges-2);
            local_count /= 6;
        }
        count += local_count;
    }

    os << count << "\n";
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
