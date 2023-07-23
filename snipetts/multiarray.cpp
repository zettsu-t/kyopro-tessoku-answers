#include <memory>
#include <boost/multi_array.hpp>
#include <gtest/gtest.h>

namespace {
    using Num = int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    Matrix global_matrix;
    std::unique_ptr<Matrix> global_pmatrix;
}

void fill_pmatrix(Num width, Num height) {
    MatrixShape shape{{height, width}};
    auto local_pmatrix = std::make_unique<Matrix>(shape);
    std::swap(global_pmatrix, local_pmatrix);

    for(Num y{0}; y<height; ++y) {
        for(Num x{0}; x<width; ++x) {
            (*global_pmatrix)[y][x] = x * y;
        }
    }
}

// #define RUNTIME_ERROR 1

void fill_matrix(Num width, Num height) {
    MatrixShape shape{{height, width}};
    Matrix local_matrix(shape);
//  実行時エラー
#ifdef RUNTIME_ERROR
    std::swap(global_matrix, local_matrix);
#endif

    for(Num y{0}; y<height; ++y) {
        for(Num x{0}; x<width; ++x) {
#ifdef RUNTIME_ERROR
            global_matrix[y][x] = x * y;
#endif
            local_matrix[y][x] = x * y;
        }
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, Ptr) {
    Num width {1000};
    Num height {2000};
    fill_pmatrix(width, height);

    for(Num y{0}; y<height; ++y) {
        for(Num x{0}; x<width; ++x) {
            EXPECT_EQ((*global_pmatrix)[y][x], x * y);
        }
    }
}

TEST_F(TestAll, Obj) {
    Num width {1000};
    Num height {2000};
    fill_matrix(width, height);
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
