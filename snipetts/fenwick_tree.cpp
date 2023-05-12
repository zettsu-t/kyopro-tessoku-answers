#include <vector>
#include <gtest/gtest.h>

namespace {
    using Num = long long int;
}

struct Bit {
    std::vector<Num> bits_;
    Num max_ {0};
    Bit(Num size) : bits_(size+1, 0), max_(size) {}

    void increment(Num pos) {
        for(auto i=pos; i<=max_; i+=(i&-i)) {
            bits_.at(i) += 1;
        }
    }

    void decrement(Num pos) {
        for(auto i=pos; i<=max_; i+=(i&-i)) {
            bits_.at(i) -= 1;
        }
    }

    Num count(Num right) const {
        Num total {0};
        for (auto i=right; i>0; i-=(i&-i)) {
            total += bits_.at(i);
        }
        return total;
    }
};

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Sequential) {
    constexpr Num n = 11;
    Bit bit(n);

    for(Num i{1}; i<=n; ++i) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }

    for(Num i{n}; i>=1; --i) {
        bit.decrement(i);
        ASSERT_EQ(i-1, bit.count(i));
        for(Num j{1}; j<=(i-1); ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }
}

TEST_F(TestAll, OddEven) {
    constexpr Num n = 19;
    Bit bit(n);

    for(Num i{1}; i<=n; i+=2) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = (j + 1) / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
    }

    for(Num i{2}; i<=n; i+=2) {
        bit.increment(i);
        for(Num j{1}; j<=i; ++j) {
            ASSERT_EQ(j, bit.count(j));
        }
    }

    for(Num i{1}; i<=n; i+=2) {
        bit.decrement(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = j / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
    }

    for(Num i{2}; i<=n; i+=2) {
        bit.decrement(i);
        for(Num j{1}; j<=i; ++j) {
            Num expected = (j <= i) ? 0 : (j - i) / 2;
            ASSERT_EQ(expected, bit.count(j));
        }
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
