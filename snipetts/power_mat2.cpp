#include <array>
#include <bitset>
#include <vector>
#include <gtest/gtest.h>

namespace {
    template <typename T>
    struct Matrix {
        using Elements = std::vector<std::vector<T>>;
        Elements elements_;
        T mod_ {1};
        size_t size_ {0};

        Matrix(T diag, T mod, size_t size) :
            elements_(size, std::vector<T>(size)), mod_(mod), size_(size) {
            for(size_t row{0}; row<size_; ++row) {
                for(size_t column{0}; column<size_; ++column) {
                    elements_.at(row).at(column) = (row == column) ? diag : 0;
                }
            }
        }

        Matrix mul(const Matrix& other) const {
            Matrix prod(0, mod_, size_);

            for(size_t row{0}; row<size_; ++row) {
                for(size_t column{0}; column<size_; ++column) {
                    for(size_t i{0}; i<size_; ++i) {
                        auto& cell = prod.elements_.at(row).at(column);
                        cell += elements_.at(row).at(i) * other.elements_.at(i).at(column);
                        cell %= mod_;
                        cell = (cell + mod_) % mod_;
                    }
                }
            }

            return prod;
        }

        T get(size_t row, size_t column) const {
            return elements_.at(row).at(column);
        }
    };

    template <typename T>
    struct MatrixPowered {
        static inline constexpr size_t Width {64};
        std::vector<T> matrix_powered_by_2_;

        MatrixPowered(const T& mat) {
            matrix_powered_by_2_.push_back(mat);

            for(size_t i{0}; (i+1)<Width; ++i) {
                const auto& base = matrix_powered_by_2_.back();
                const auto powered = base.mul(base);
                matrix_powered_by_2_.push_back(powered);
            }
        }

        T power(size_t index) {
            const auto mod = matrix_powered_by_2_.at(0).mod_;
            const auto size = matrix_powered_by_2_.at(0).size_;
            T prod(1, mod, size);
            std::bitset<Width> bits(index);

            for(size_t i{0}; i<64; ++i) {
                if (bits[i]) {
                    prod = prod.mul(matrix_powered_by_2_.at(i));
                }
            }
            return prod;
        }
    };
}

// ABC 293-E
// https://atcoder.jp/contests/abc293/tasks/abc293_e
class TestMatrixPowered : public ::testing::Test {};

TEST_F(TestMatrixPowered, Elements) {
    constexpr size_t Size = 2;
    using Num = int32_t;
    using Mat = Matrix<Num>;
    const std::vector<Num> mods {2, 3, 256};
    for(const auto& mod : mods) {
        Mat base = Mat(0, mod, Size);
        base.elements_.at(0).at(0) = 1;
        base.elements_.at(0).at(1) = 2;
        base.elements_.at(1).at(0) = 3;
        base.elements_.at(1).at(1) = 4;

        MatrixPowered<Mat> mat_powered(base);
        const auto powered = mat_powered.power(2);
        const Num expected00 = 7 % mod;
        const Num expected01 = 10 % mod;
        const Num expected10 = 15 % mod;
        const Num expected11 = 22 % mod;
        EXPECT_EQ(expected00, powered.get(0, 0));
        EXPECT_EQ(expected01, powered.get(0, 1));
        EXPECT_EQ(expected10, powered.get(1, 0));
        EXPECT_EQ(expected11, powered.get(1, 1));
    }
}

TEST_F(TestMatrixPowered, Case1) {
    constexpr size_t Size = 2;
    using Num = int16_t;
    using Mat = Matrix<Num>;
    constexpr Num a = 3;
    constexpr Num x = 4;
    constexpr Num m = 7;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(5, powered.get(0, 1));
}

TEST_F(TestMatrixPowered, Case2) {
    constexpr size_t Size = 2;
    using Num = int32_t;
    using Mat = Matrix<Num>;
    constexpr Num a = 8;
    constexpr Num x = 10;
    constexpr Num m = 9;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(0, powered.get(0, 1));
}

TEST_F(TestMatrixPowered, Case3) {
    constexpr size_t Size = 2;
    using Num = uint64_t;
    using Mat = Matrix<Num>;
    constexpr Num a = 1000000000ULL;
    constexpr Num x = 1000000000000ULL;
    constexpr Num m = 998244353;

    Mat base = Mat(0, m, Size);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(919667211ULL, powered.get(0, 1));
}

TEST_F(TestMatrixPowered, Size) {
    // m <- matrix(1:9, nrow=3, ncol=3)
    // m %*% m %*% m
    constexpr size_t Size = 3;
    using Num = uint64_t;
    using Mat = Matrix<Num>;
    Mat base = Mat(0, 1000000000, Size);
    base.elements_.at(0).at(0) = 1;
    base.elements_.at(1).at(0) = 2;
    base.elements_.at(2).at(0) = 3;
    base.elements_.at(0).at(1) = 4;
    base.elements_.at(1).at(1) = 5;
    base.elements_.at(2).at(1) = 6;
    base.elements_.at(0).at(2) = 7;
    base.elements_.at(1).at(2) = 8;
    base.elements_.at(2).at(2) = 9;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(2);
    EXPECT_EQ(30ULL, powered.get(0, 0));
    EXPECT_EQ(36ULL, powered.get(1, 0));
    EXPECT_EQ(42ULL, powered.get(2, 0));
    EXPECT_EQ(66ULL, powered.get(0, 1));
    EXPECT_EQ(81ULL, powered.get(1, 1));
    EXPECT_EQ(96ULL, powered.get(2, 1));
    EXPECT_EQ(102ULL, powered.get(0, 2));
    EXPECT_EQ(126ULL, powered.get(1, 2));
    EXPECT_EQ(150ULL, powered.get(2, 2));

    const auto powered3 = mat_powered.power(3);
    EXPECT_EQ(1548ULL, powered3.get(2, 1));
    EXPECT_EQ(2034ULL, powered3.get(1, 2));
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
