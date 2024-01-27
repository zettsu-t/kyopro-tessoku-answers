#include <array>
#include <bitset>
#include <vector>
#include <gtest/gtest.h>

namespace {
    template <typename T, size_t Size>
    struct Matrix {
        using Elements = std::array<std::array<T, Size>, Size>;
        Elements elements_;
        T mod_ {1};

        Matrix(T diag, T mod) : mod_(mod) {
            for(size_t row{0}; row<Size; ++row) {
                for(size_t column{0}; column<Size; ++column) {
                    elements_.at(row).at(column) = (row == column) ? diag : 0;
                }
            }
        }

        Matrix mul(const Matrix& other) const {
            Matrix prod(0, mod_);

            for(size_t row{0}; row<Size; ++row) {
                for(size_t column{0}; column<Size; ++column) {
                    for(size_t i{0}; i<Size; ++i) {
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
            T prod(1, mod);
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

TEST_F(TestMatrixPowered, Case1) {
    using Num = int16_t;
    using Mat = Matrix<Num, 2>;
    constexpr Num a = 3;
    constexpr Num x = 4;
    constexpr Num m = 7;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(5, powered.get(0, 1));
}

TEST_F(TestMatrixPowered, Case2) {
    using Num = int32_t;
    using Mat = Matrix<Num, 2>;
    constexpr Num a = 8;
    constexpr Num x = 10;
    constexpr Num m = 9;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(0, powered.get(0, 1));
}

TEST_F(TestMatrixPowered, Case3) {
    using Num = uint64_t;
    using Mat = Matrix<Num, 2>;
    constexpr Num a = 1000000000ULL;
    constexpr Num x = 1000000000000ULL;
    constexpr Num m = 998244353;

    Mat base = Mat(0, m);
    base.elements_.at(0).at(0) = a;
    base.elements_.at(0).at(1) = 1;
    base.elements_.at(1).at(1) = 1;

    MatrixPowered<Mat> mat_powered(base);
    const auto powered = mat_powered.power(x);
    EXPECT_EQ(919667211ULL, powered.get(0, 1));
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
