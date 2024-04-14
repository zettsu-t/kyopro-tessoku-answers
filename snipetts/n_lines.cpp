// NxNマスのN列
#include <algorithm>
#include <numeric>
#include <gtest/gtest.h>

using Num = long long int;

template<typename T>
void print_oneline(const std::vector<T>& vec, std::ostream& os) {
    const auto size = vec.size();
    for(size_t i{0}; i<size; ++i) {
        os << vec.at(i) << (((i+1) == size) ? '\n' : ' ');
    }
}

Num yx_to_index(Num n, Num y, Num x) {
    return y * n + x;
}

std::vector<std::vector<Num>> make_lines(Num n) {
    std::vector<std::vector<Num>> lines;

    std::vector<Num> d_line;
    std::vector<Num> u_line;
    for(Num i{0}; i<n; ++i) {
        std::vector<Num> h_line;
        std::vector<Num> v_line;
        for(Num j{0}; j<n; ++j) {
            h_line.push_back(yx_to_index(n, i, j));
            v_line.push_back(yx_to_index(n, j, i));
        }
        lines.push_back(h_line);
        lines.push_back(v_line);
        d_line.push_back(yx_to_index(n, i, i));
        u_line.push_back(yx_to_index(n, i, n-1-i));
    }

    lines.push_back(d_line);
    lines.push_back(u_line);
    return lines;
}

void print_lines(const std::vector<std::vector<Num>>& lines, std::ostream& os) {
    for(const auto& line : lines) {
        print_oneline(line, os);
    }
}

std::vector<std::vector<std::vector<Num>>> positions_and_lines(Num n, const std::vector<std::vector<Num>>& lines) {
    const auto grid_size = n * n;
    std::vector<std::vector<std::vector<Num>>> line_set(grid_size);
    for(const auto& line : lines) {
        for(const auto& v : line) {
            line_set.at(v).push_back(line);
        }
    }

    return line_set;
}

void print_line_set(const std::vector<std::vector<std::vector<Num>>>& line_set, std::ostream& os) {
    Num index {0};
    for(const auto& lines : line_set) {
        os << index << "\n";
        for(const auto& line : lines) {
            print_oneline(line, os);
        }
        ++index;
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Two) {
    const Num n = 2;
    auto lines = make_lines(n);
    std::ranges::sort(lines);

    std::ostringstream oss_lines;
    print_lines(lines, oss_lines);
    const std::string expected_lines = "0 1\n0 2\n0 3\n1 2\n1 3\n2 3\n";
    ASSERT_EQ(expected_lines, oss_lines.str());

    std::ostringstream oss_line_set;
    const auto line_set = positions_and_lines(n, lines);
    print_line_set(line_set, oss_line_set);
    const std::string expected_line_set = "0\n0 1\n0 2\n0 3\n1\n0 1\n1 2\n1 3\n2\n0 2\n1 2\n2 3\n3\n0 3\n1 3\n2 3\n";
    ASSERT_EQ(expected_line_set, oss_line_set.str());
}

TEST_F(TestAll, Three) {
    const Num n = 3;
    auto lines = make_lines(n);
    std::ranges::sort(lines);

    std::ostringstream oss_lines;
    print_lines(lines, oss_lines);
    const std::string expected_lines = "0 1 2\n0 3 6\n0 4 8\n1 4 7\n2 4 6\n2 5 8\n3 4 5\n6 7 8\n";
    ASSERT_EQ(expected_lines, oss_lines.str());

    std::ostringstream oss_line_set;
    const auto line_set = positions_and_lines(n, lines);
    print_line_set(line_set, oss_line_set);
    const std::string expected_line_set = "0\n0 1 2\n0 3 6\n0 4 8\n1\n0 1 2\n1 4 7\n2\n0 1 2\n2 4 6\n2 5 8\n3\n0 3 6\n3 4 5\n4\n0 4 8\n1 4 7\n2 4 6\n3 4 5\n5\n2 5 8\n3 4 5\n6\n0 3 6\n2 4 6\n6 7 8\n7\n1 4 7\n6 7 8\n8\n0 4 8\n2 5 8\n6 7 8\n";
    ASSERT_EQ(expected_line_set, oss_line_set.str());
}

TEST_F(TestAll, Four) {
    const Num n = 4;
    auto lines = make_lines(n);
    std::ranges::sort(lines);

    std::ostringstream oss_lines;
    print_lines(lines, oss_lines);
    const std::string expected_lines = "0 1 2 3\n0 4 8 12\n0 5 10 15\n1 5 9 13\n2 6 10 14\n3 6 9 12\n3 7 11 15\n4 5 6 7\n8 9 10 11\n12 13 14 15\n";

    std::ostringstream oss_line_set;
    const auto line_set = positions_and_lines(n, lines);
    const std::vector<size_t> expected_size = {3,2,2,3,2,3,3,2,2,3,3,2,3,2,2,3};

    const size_t size = static_cast<size_t>(n*n);
    ASSERT_EQ(line_set.size(), size);
    ASSERT_EQ(line_set.size(), expected_size.size());
    for(size_t i{0}; i<size; ++i) {
        ASSERT_EQ(line_set.at(i).size(), expected_size.at(i));
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
Local Variables:
mode: c++
coding: utf-8-dos
tab-width: nil
c-file-style: "stroustrup"
End:
*/
