#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#include <gtest/gtest.h>

using Num = long long int;
using Vec = std::vector<Num>;
//  using BigInt = __int128;
using BigInt = boost::multiprecision::cpp_int;
using Rational = boost::rational<BigInt>;

// 直線の標準形(canonical form)
struct Line {
    bool vertical_ {false};  // Y軸に平行、X軸に垂直
    Rational slope_ {0};
    Rational intercept_ {0};

    Line(Rational x0, Rational y0, Rational x1, Rational y1) {
        const auto dy = y1 - y0;
        const auto dx = x1 - x0;

        if (dx == 0) {
            vertical_ = true;
            intercept_ = x0;
        } else {
            slope_ = dy;
            slope_ /= dx;

            auto height = slope_;
            height *= x0;
            intercept_ = y0;
            intercept_ -= height;
        }
    }

    Line(bool vertical, Rational slope, Rational intercept) :
        vertical_(vertical), slope_(slope), intercept_(intercept) {
    }

    // 線の上にある点を中心に90度回転する
    // 線の上に無い点については未定義
    Line rotate90on(Rational x, Rational y) const {
        bool vertical {false};
        Rational slope {0};
        Rational intercept {0};

        if (vertical_) {
            // 水平線にする
            intercept = y;
        } else if (slope_ == 0) {
            // 垂直線にする
            vertical = true;
            intercept = x;
        } else {
            slope = -1;
            slope /= slope_;
            Rational height = slope;
            height *= x;
            intercept = y;
            intercept -= height;
        }

        return Line(vertical, slope, intercept);
    }

    bool is_equal(const Line& other) const {
        return ((vertical_ == other.vertical_) && (slope_ == other.slope_) && (intercept_ == other.intercept_));
    }

    bool operator==(const Line& other) const {
        return is_equal(other);
    }

    bool operator!=(const Line& other) const {
        return !is_equal(other);
    }

    // 交点があればtrue
    bool cross(const Line& other) const {
        if (vertical_ != other.vertical_) {
            return true;
        }

        if (slope_ != other.slope_) {
            return true;
        }

        return intercept_ == other.intercept_;
    };

    void print(std::ostream& os) const {
        os << vertical_ << " " << slope_ << ":" << intercept_ << "\n";
    }
};

class TestLine : public ::testing::Test {};

TEST_F(TestLine, Initialize) {
    const Num xunit = 3;
    const Num yunit = 2;

    for(Num dx{-9}; dx<=9; dx+=3) {
        const Line vertival(true, 0, dx);
        for(Num dy{-5}; dy<=5; ++dy) {
            const Line horizontal(false, 0, dy);
            Rational expected_slope = yunit;
            expected_slope /= xunit;
            Rational expected_intercept = 1;
            expected_intercept /= xunit;
            expected_intercept += dy;
            expected_intercept -= (dx * yunit) / xunit;

            const Line expected(false, expected_slope, expected_intercept);
            const Line actual1(4 + dx, 3 + dy, 7 + dx, 5 + dy);
            const Line actual2(1 + dx, 1 + dy, 7 + dx, 5 + dy);
            EXPECT_TRUE(expected == actual1);
            EXPECT_FALSE(expected != actual1);

            EXPECT_TRUE(expected == actual2);
            EXPECT_FALSE(expected != actual2);

            EXPECT_FALSE(expected == vertival);
            EXPECT_TRUE(expected != vertival);

            EXPECT_FALSE(expected == horizontal);
            EXPECT_TRUE(expected != horizontal);

            if ((dx != 0) && (dy != 0)) {
                const Line cross(1 + dx, 1 + dy, 7 + dx, 5 - dy);
                EXPECT_FALSE(expected == cross);
                EXPECT_TRUE(expected != cross);
            }
        }
    }
}

TEST_F(TestLine, Vertical) {
    for(Num dx{-1}; dx<=1; ++dx) {
        const Line horizontal(false, 0, dx);
        const Line expected(true, 0, dx);
        const Line actual(dx, -2, dx, 3);
        EXPECT_TRUE(expected == actual);
        EXPECT_FALSE(expected != actual);
        EXPECT_FALSE(expected == horizontal);
        EXPECT_TRUE(expected != horizontal);
    }
}

TEST_F(TestLine, Horizontal) {
    for(Num dy{-1}; dy<=1; ++dy) {
        const Line vertical(true, 0, dy);
        const Line expected(false, 0, dy);
        const Line actual(-2, dy, 3, dy);
        EXPECT_TRUE(expected == actual);
        EXPECT_FALSE(expected != actual);
        EXPECT_FALSE(expected == vertical);
        EXPECT_TRUE(expected != vertical);
    }
}

TEST_F(TestLine, Rotate) {
    Rational slope1 {1};
    slope1 /= 4;
    Rational intercept1 {5};
    intercept1 /= 2;
    const Line line1(false, slope1, intercept1);

    Rational slope2 {-4};
    Rational intercept2 {11};
    const Line line2(false, slope2, intercept2);

    EXPECT_TRUE(line2 == line1.rotate90on(2, 3));
    EXPECT_TRUE(line1 == line2.rotate90on(2, 3));
}

TEST_F(TestLine, RotateVertical) {
    const auto sx = 5;
    const auto sy = 7;
    const Line vertical(true, 0, sx);
    const Line horizontal(false, 0, sy);
    EXPECT_TRUE(vertical == horizontal.rotate90on(sx, sy));
    EXPECT_TRUE(horizontal == vertical.rotate90on(sx, sy));
}

TEST_F(TestLine, Cross) {
    const auto sx = 5;
    const auto sy = 7;
    const auto dx = 5;
    const auto dy = 7;
    const auto ofs = 4;

    const Line vertical(true, 0, sx);
    const Line vertical_alt(true, 0, sx + 1);
    const Line horizontal(false, 0, sy);
    const Line horizontal_alt(false, 0, sy - 1);
    const Line line(sx, dx+dx, sy, sy+dy);
    const Line cross(sx, dx+dx, sy, sy-dy);
    const Line parallel(sx+ofs, dx+dx+ofs, sy+ofs, sy+dy+ofs);

    EXPECT_TRUE(vertical.cross(vertical));
    EXPECT_FALSE(vertical.cross(vertical_alt));
    EXPECT_TRUE(vertical.cross(horizontal));
    EXPECT_TRUE(vertical.cross(line));
    EXPECT_TRUE(vertical.cross(cross));
    EXPECT_TRUE(vertical.cross(parallel));

    EXPECT_TRUE(horizontal.cross(vertical));
    EXPECT_TRUE(horizontal.cross(horizontal));
    EXPECT_FALSE(horizontal.cross(horizontal_alt));
    EXPECT_TRUE(horizontal.cross(line));
    EXPECT_TRUE(horizontal.cross(cross));
    EXPECT_TRUE(horizontal.cross(parallel));

    EXPECT_TRUE(line.cross(vertical));
    EXPECT_TRUE(line.cross(vertical_alt));
    EXPECT_TRUE(line.cross(horizontal));
    EXPECT_TRUE(line.cross(horizontal_alt));
    EXPECT_TRUE(line.cross(line));
    EXPECT_TRUE(line.cross(cross));
    EXPECT_FALSE(line.cross(parallel));

    EXPECT_TRUE(cross.cross(cross));
    EXPECT_TRUE(cross.cross(line));
    EXPECT_FALSE(line.cross(parallel));

    EXPECT_TRUE(parallel.cross(parallel));
    EXPECT_FALSE(parallel.cross(line));
    EXPECT_TRUE(parallel.cross(cross));
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
