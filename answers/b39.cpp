// https://atcoder.jp/contests/tessoku-book/submissions/38827431
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

void solve(std::istream& is, std::ostream& os) {
    using Num = long long int;
    size_t n {0};
    Num d {0};
    is >> n >> d;

    struct Job {
        Num start {0};
        Num income {0};
    };

    struct {
        bool operator()(const Job& lhs, const Job& rhs) const {
            return std::tie(lhs.start, lhs.income) < std::tie(rhs.start, rhs.income);
        }
    } lessJob;

    struct {
        bool operator() (const Job& job, Num day) const { return job.start < day; }
        bool operator() (Num day, const Job& job) const { return day < job.start; }
    } compJob;

    std::vector<Job> jobs(n+1);
    for(decltype(n) i{1}; i<=n; ++i) {
        Num x {0};
        Num y {0};
        is >> x >> y;
        jobs.at(i) = Job{x, y};
    }
    std::sort(jobs.begin(), jobs.end(), lessJob);

    Num total {0};
    std::priority_queue<int> pending_jobs;
    for(decltype(d) i{1}; i<=d; ++i) {
        const auto today_jobs = std::equal_range(jobs.begin(), jobs.end(), i, compJob);
        for(auto it = today_jobs.first; it != today_jobs.second; ++it) {
            pending_jobs.push(it->income);
        }
        if (!pending_jobs.empty()) {
            total += pending_jobs.top();
            pending_jobs.pop();
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
