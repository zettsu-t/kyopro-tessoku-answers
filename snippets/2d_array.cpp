#include <array>
#include <iostream>
#include <type_traits>
#include <vector>
#include <boost/multi_array.hpp>
#include <gtest/gtest.h>

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Array) {
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    std::array<std::array<int, width>, height> arr;

    for(int v{0}; v<10; ++v) {
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                arr.at(y).at(x) = v;
            }
        }

        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr.at(y).at(x));
            }
        }
    }
}

template <typename T, size_t H, size_t W>
void fill_2darray(std::array<std::array<T, W>, H>& arr, const T& v) {
    static_assert(std::is_same_v<size_t, typename std::array<std::array<T, W>, H>::size_type>);
    static_assert(std::is_same_v<size_t, typename std::array<T, W>::size_type>);
    for(size_t y{0}; y<H; ++y) {
        for(size_t x{0}; x<W; ++x) {
            arr.at(y).at(x) = v;
        }
    }
}

TEST_F(TestAll, Array2) {
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    std::array<std::array<int, width>, height> arr;

    for(int v{0}; v<10; ++v) {
        fill_2darray(arr, v);
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr.at(y).at(x));
            }
        }
    }
}

TEST_F(TestAll, Boost) {
    using Matrix = boost::multi_array<int, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    constexpr size_t height = 3;
    constexpr size_t width = 2;
    MatrixShape shape {{height, width}};
    Matrix arr(shape);

    for(int v{0}; v<10; ++v) {
        std::fill_n(arr.data(), arr.num_elements(), v);
        for(size_t y{0}; y<height; ++y) {
            for(size_t x{0}; x<width; ++x) {
                EXPECT_EQ(v, arr[y][x]);
            }
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
