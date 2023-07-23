#include <boost/multi_array.hpp>
#include <gtest/gtest.h>

namespace {
    using Num = int;
    using Matrix = boost::multi_array<Num, 2>;
    using MatrixShape = boost::array<Matrix::index, 2>;
    Matrix global_matrix;
}

void fill_matrix(Num width, Num height) {
    MatrixShape shape{{height, width}};
    Matrix local_matrix(shape);
//  実行時エラー
//  std::swap(global_matrix, local_matrix);

    for(Num y{0}; y<height; ++y) {
        for(Num x{0}; x<width; ++x) {
//          global_matrix[y][x] = x * y;
            local_matrix[y][x] = x * y;
        }
    }
}

class TestAll : public ::testing::Test {};

TEST_F(TestAll, All) {
    fill_matrix(1000, 2000);
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
