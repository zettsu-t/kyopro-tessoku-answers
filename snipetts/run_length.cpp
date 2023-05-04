#include <optional>
#include <tuple>
#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = int;
}

template <typename T>
struct RunLength {
    struct Run {
        Num pos {0};
        Num len {0};
        T value {0};
        bool operator==(const Run& rhs) const {
            return std::tie(pos, len, value) == std::tie(rhs.pos, rhs.len, rhs.value);
        }
    };

    using Runs = std::vector<Run>;
    Runs runs;

    RunLength(const std::vector<T>& seq) {
        std::optional<Run> current;

        Num pos {0};
        for(const auto& value : seq) {
            const Run run {pos, 1, value};

            if (!current.has_value()) {
                current = run;
            } else {
                if (current.value().value != value) {
                    runs.push_back(current.value());
                    current = run;
                } else {
                    current.value().len += 1;
                }
            }
            ++pos;
        }

        if (current.has_value()) {
            runs.push_back(current.value());
        }
    }
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Empty) {
    const std::vector<Num> input;
    const auto actual = RunLength<Num>(input);
    EXPECT_TRUE(actual.runs.empty());
}

TEST_F(TestAll, Same) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {3,3,3,3,3};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,5,3}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Each) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {2,4,8};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,1,2}, {1,1,4}, {2,1,8}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Groups1) {
    using Rle = RunLength<Num>;
    const std::vector<Num> input {40,40,20,20,20,30};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,2,40}, {2,3,20}, {5,1,30}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
}

TEST_F(TestAll, Groups2) {
    using Rle = RunLength<char>;
    const std::vector<char> input {'a','b','b','b','c'};
    const auto rle = Rle(input);
    const auto& actual = rle.runs;
    const Rle::Runs expected {{0,1,'a'}, {1,3,'b'}, {4,1,'c'}};
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_TRUE(std::equal(actual.begin(), actual.end(), expected.begin()));
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
