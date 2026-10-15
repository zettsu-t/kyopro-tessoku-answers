// A subsequence not necessarily contiguous
#include <string>
#include <vector>
#include <gtest/gtest.h>

size_t find_subsequence(const std::string& target, const std::string& sub) {
    size_t n_matched {0};
    size_t target_size = target.size();
    size_t sub_size = sub.size();

    for(size_t cursor{0}; (cursor < target_size) && (n_matched < sub_size); ++cursor) {
        if (target.at(cursor) == sub.at(n_matched)) {
            ++n_matched;
        }
    }

    return n_matched;
}

class TestAll : public ::testing::Test {};

const std::vector<std::string> test_cases {
    "abc", "a", "aa", "aaa", "aab", "abb"
};

TEST_F(TestAll, Exact) {
    for(const auto& s : test_cases) {
        const auto actual = find_subsequence(s, s);
        ASSERT_EQ(s.size(), actual);
    }
}

TEST_F(TestAll, Symmetric) {
    for(const auto& s : test_cases) {
        const auto size = s.size();
        for(size_t len {1}; (len + 1) < size; ++len) {
            const auto sub = s.substr(0, len);

            const auto actual = find_subsequence(s, sub);
            ASSERT_EQ(len, actual);

            const auto rev = find_subsequence(sub, s);
            ASSERT_EQ(len, rev);
        }
    }
}

TEST_F(TestAll, SubSeq) {
    const std::string target = "abbcccddddeeeccca";
    const std::vector<std::string> full_cases {
        "aa", "abcdeca", "ace", "bda", "cec"
    };

    for(const auto& s : full_cases) {
        const auto actual = find_subsequence(target, s);
        ASSERT_EQ(s.size(), actual);
    }

    const std::vector<std::pair<std::string, size_t>> sub_cases {
        {"aaa", 2}, {"abcxeca", 3}, {"dddddd", 4}, {"xyz", 0}
    };

    for(const auto& [s, expected] : sub_cases) {
        const auto actual = find_subsequence(target, s);
        ASSERT_EQ(expected, actual);
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
