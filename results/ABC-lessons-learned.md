# AtCoder Beginner Contest lessons learned

ABC 6,7,8問体制(126..最新)のD問題をだいたい解いて、コンテストに参加したので、そこから得た教訓をまとめていきます。

- ABC (126..最新)の、D問題、緑水diff全問
- ARC, AGC (058..最新), AGC(002..最新) の灰茶緑水diff
- ABC, ARC のコンテスト中に解いた問題
- その他記事からリンクされた問題

## AtCoder Beginner Contest

- [ABC 最新](ABC-lessons-learned12.md)
- [ABC 参加記5](ABC-lessons-learned11.md)
- [ABC 参加記4](ABC-lessons-learned10.md)
- [ABC 400まで](ABC-lessons-learned9.md)
- [ABC 参加記3](ABC-lessons-learned8.md)
- [ABC 参加記2](ABC-lessons-learned7.md)
- [ABC 参加記1](ABC-lessons-learned6.md)
- [ABC 324まで](ABC-lessons-learned5.md)
- [ABC 300まで](ABC-lessons-learned4.md)
- [ABC 250まで](ABC-lessons-learned3.md)
- [ABC 200まで](ABC-lessons-learned2.md)
- [ABC 150まで](ABC-lessons-learned1.md)

## AtCoder Regular Contest / AtCoder Grand Contest

- [ARC 最新](ARC-lessons-learned5.md)
- [ARC 参加記1](ARC-lessons-learned4.md)
- [ARC 184まで](ARC-lessons-learned3.md)
- [ARC 150まで](ARC-lessons-learned2.md)
- [ARC 103まで](ARC-lessons-learned1.md)
- [AGC](AGC-lessons-learned1.md)

## 標準出力

コンソールに入力と出力が混ざってどれが出力か分からなくなる。 `grep` で色付けすると、どれが出力か分かる。空行を `_` に置き換えて空行を目立たせる。

```bash
make && ./343b | sed 's/^$/_/' | grep --color -e "^.*"
```

あらかじめ用意した期待値(出力例)と、コマンドの出力が一致するかどうかは、 `bash` なら以下のように調べる。

`diff` の `-C` オプションは、copied contextで出力を十分長く出して、出力が何か確認しつつ期待値 `expected.txt` との差を表示する。 `--strip-trailing-cr` オプションは行末のCRを除くことで、WindowsとCygwinで改行コードがCRLFかLFの違いを無視する。

```bash
diff -C 100000 --strip-trailing-cr <(make && ./335c 2>&1) expected.txt
```

## データ構造とアルゴリズム別

### 二次元配列

添え字の実行時
三次元以上も同様である。

C++17標準ライブラリ

```cpp
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

constexpr size_t height = 3;
constexpr size_t width = 2;
std::array<std::array<int, width>, height> arr;
fill_2darray(arr, v);
```

Boost C++ Libraries

`boost::multi_array` は配列長が決まっているのでSTLのように `std::swap()` できないが、スマートポインタに持たせれば `std::swap()` できる。グローバル変数に置いてDFSするときに、 `main()` 実行前に初期化したグローバル変数を後で交換する際に注意する。Assertion failedならまだよくて、セグメンテーションフォールトになったり他の変数を上書きしたりするとデバッグが厄介になる。

```cpp
using Matrix = boost::multi_array<int, 2>;
using MatrixShape = boost::array<Matrix::index, 2>;
constexpr size_t height = 3;
constexpr size_t width = 2;
MatrixShape shape {{height, width}};
Matrix arr(shape);
std::fill_n(arr.data(), arr.num_elements(), v);
```

### N目並べ

$N \times N$ 行列の縦横斜め $N$ 列を列挙する。 `make_lines` は直線を列挙し、 `positions_and_lines` は各頂点が属する直線を列挙する(その頂点自身を含む)。

```cpp
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
```

### 行列を時計回りに回転する

回転する。

```cpp
using Num = long long int;
using Matrix = std::vector<std::vector<Num>>;

Matrix rotate_board(const Matrix& input) {
    auto input_height = input.size();
    size_t input_width = 0;
    for(const auto& row : input) {
        input_width = std::max(input_width, row.size());
    }
    auto target_height = input_width;
    auto target_width = input_height;

    std::vector<std::vector<Num>> board(target_height);
    for(decltype(target_height) y{0}; y<target_height; ++y) {
        board.at(y).resize(target_width, 0);
        for(decltype(target_width) x{0}; x<target_width; ++x) {
            const auto& row = input.at(input_height-1-x);
            board.at(y).at(x) = (y < row.size()) ? row.at(y) : 0;
        }
    }

    return board;
}
```

回転して一致するかどうか確認するために、周辺の余白を取り除く。

```cpp
using Num = long long int;
using Matrix = std::vector<std::vector<Num>>;

Matrix shrink_board(const Matrix& input) {
    const Matrix zero;
    auto height = static_cast<Num>(input.size());
    if (height == 0) {
        return zero;
    }

    Num width = 0;
    for(const auto& row : input) {
        width = std::max(width, static_cast<Num>(row.size()));
    }

    Num top {height + 1LL};
    Num bottom {-1};
    Num left {width + 1LL};
    Num right {-1};

    Num count {0};
    for(Num y{0}; y<height; ++y) {
        const auto& row = input.at(y);
        auto it_left = std::find_if(row.begin(), row.end(), [](const auto& e) { return (e > 0); });
        if (it_left == row.end()) {
            continue;
        }

        ++count;
        top = std::min(top, y);
        bottom = y;
        const auto pos = static_cast<Num>(it_left - row.begin());
        left = std::min(left, pos);

        auto max_x = std::min(width, static_cast<Num>(row.size()));
        for(Num x{max_x-1}; x>=left; --x) {
            if (row.at(x) > 0) {
                right = std::max(right, x);
                break;
            }
        }
    }

    if (count == 0) {
        return zero;
    }

    Matrix board(bottom - top + 1);
    Num y {0};
    for(Num i=top; i<=bottom; ++i, ++y) {
        board.at(y).resize(right - left + 1, 0);
        Num x {0};
        for(Num j=left; j<=right; ++j, ++x) {
            board.at(y).at(x) = input.at(i).at(j);
        }
    }

    return board;
}
```

二つの行列が等しいかどうか確認する。

```cpp
bool are_equal(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    auto size = lhs.size();
    for(decltype(size) i{0}; i<size; ++i) {
        if (lhs.at(i).size() != rhs.at(i).size()) {
            return false;
        }

        const auto eq = std::equal(lhs.at(i).begin(), lhs.at(i).end(), rhs.at(i).begin());
        if (!eq) {
            return false;
        }
    }

    return true;
}
```

### 矩形の頂点から距離が等しい点を列挙する

矩形の頂点から距離が等しい矩形内の場所、言い換えればマンハッタン距離(1ノルム)が等しい場所の集合は、矩形に対して45度斜め線になる。斜線がどの辺と交わるか厄介なのであらかじめ用意しておく。

`std::vector<std::pair<Num, Num>>` の出し入れをそのまま実装するとTLEすることがある。その場合はforループを直に回せばいい。

```cpp
namespace {
    using Num = long long int;
    using Positions = std::vector<std::pair<Num, Num>>;
}

Positions norm1_positions(Num width, Num height, Num distance) {
    Positions ps;
    const auto max_distance = width + height - 2;
    if (distance > max_distance) {
        return ps;
    }

    Num start_y = std::min(height - 1, distance);
    Num start_x = std::max(0LL, distance - start_y);
    Num y {start_y};
    Num x {start_x};
    for(;;) {
        ps.push_back(std::make_pair(x, y));
        ++x;
        --y;
        if ((x >= width) || (y < 0)) {
            break;
        }
    }

    return ps;
}
```

### 配列を空白区切りで出力する

```cpp
for(Num i{1}; i<=n; ++i) {
    os << answer.at(i);
    if (i == n) {
        os << "\n";
    } else {
        os << " ";
    }
}
```

### 書式を指定して数値を文字列として出力する

整数の先頭を0埋めして出力する。

```cpp
os << std::setfill('0') << std::right << std::setw(width) << value;
```

`printf` の方が簡潔だが整数型を指定する手間が要る。それと `ios::sync_with_stdio(false);` で `std::cout` と `printf` の同期を切ると上手くいかない。

```cpp
long long int r {343};
printf("%010lld\n", r);
```

浮動小数の桁数を指定して出力する。

```cpp
os << std::setprecision(width) << value;
```

### 組み合わせ

n個からk個を選ぶ組み合わせ(n choose k)は、以下の通り計算する。なお 0 choose 0 は1である。

```cpp
#include <atcoder/all>
using Num = long long int;
using ModInt = atcoder::modint1000000007;

ModInt calc_combi(Num n, Num k) {
    if (n < k) {
        return 0;
    }

    ModInt n_combi = 1;
    for(Num i{1}; i<=k; ++i) {
        n_combi *= (n + 1 - i);
    }

    for(Num i{1}; i<=k; ++i) {
        n_combi /= i;
    }

    return n_combi;
}
```

組み合わせを求めるとき、 ${{n} \choose {k}} = n!/(n-k)!k!$ を $k=0..N$ ごとに計算するとTLEすることがあるので、逐次的に求める。

- ${{n} \choose {0}} = 1$
- ${{n} \choose {k+1}} = n!/(n-(k+1))!(k+1)! = {{n} \choose {k}} \times (n-k)/(k+1)$

${{n+i} \choose {i}}$ も逐次的に求める。

- ${{n} \choose {0}} = 1$
- ${{n+i+1} \choose {i+1}} = (n+i+1)!/n!(i+1)! = {{n+i} \choose {i}} \times (n+i+1)/(i+1)$

区別のつく $N$ 個の要素 $seq$ の前後に、 区別のつかない要素 $a$ を $M$ 個を挟み込む( $a$ が連続してもよい)方法は、 ${{N+M} \choose {M}}$ 通りである。全要素の $(N+M!)$ 通りの順列のうち、 $seq$ は固定なので $N!$ で割り、 $M!$ 通りの $a$ は区別がつかないので割ると導出できる。

### 素数表と素数判定

エラトステネスの篩で作る。0と1は素数ではない。

- `upper` 素数の上限(正の整数: 入力)
- `std::vector<Num>& primes` 素数の列(2から上限まで昇順: 出力)
- `std::vector<bool>& are_primes` ある整数が素数かどうか(0から上限までについて: 出力)
- `std::vector<Num>& n_primes` ある整数以下の素数の個数(0から上限までについて: 出力)

```cpp
using Num = long long int;

void find_primes(Num upper, std::vector<Num>& primes,
                 std::vector<bool>& are_primes, std::vector<Num>& n_primes) {
    primes.clear();
    are_primes.clear();
    n_primes.clear();
    are_primes.resize(upper + 1, true);
    n_primes.resize(upper + 1, 0);

    for(Num i{0}; i<=std::min(upper, 1LL); ++i) {
        are_primes.at(i) = false;
    }

    for(Num p{2}; (p*p)<=upper; ++p) {
        if (!are_primes.at(p)) {
            continue;
        }
        for(Num i=p*2; i<=upper; i+=p) {
            are_primes.at(i) = false;
        }
    }

    for(Num p{2}; p<=upper; ++p) {
        if (are_primes.at(p)) {
            primes.push_back(p);
        }
        n_primes.at(p) += n_primes.at(p-1) + are_primes.at(p);
    }
}
```

素数かどうかを素因数分解無しで判定する。

```cpp
bool is_prime(Num n) {
    auto r = n;
    for(decltype(r) p{2}; (p*p)<=n; ++p) {
        if ((r % p) == 0) {
            r /= p;
        }

        if (r <= 1) {
            break;
        }
    }

    return (r == n);
}
```

### 素因数分解

pで割り切った後はp*2で割り切れる数は残っていない。

```cpp
using Num = long long int;
using FactorMap = std::map<Num, Num>;

FactorMap prime_factorize(Num x) {
    FactorMap factors;
    auto r = x;

    for(Num p{2}; (p*p)<=x; ++p) {
        while((r % p) == 0) {
            factors[p] += 1;
            r /= p;
        }

        if (r == 1) {
            break;
        }
    }

    if (r > 1) {
        factors[r] = 1;
    }

    return factors;
}
```

### 約数

昇順で、同じ数は二度挙げない。

```cpp
using Factors = std::vector<Num>;

Factors factorize(Num x) {
    Factors factors;
    for(Num p{1}; (p*p)<=x; ++p) {
        if ((x % p) != 0) {
            continue;
        }

        factors.push_back(p);
        const auto q = x / p;
        if (q != p) {
            factors.push_back(q);
        }
    }

    std::sort(factors.begin(), factors.end());
    auto it = std::unique(factors.begin(), factors.end());
    factors.erase(it, factors.end());
    return factors;
}
```

### aのb乗 mod M

64-bit整数の入力に対応するために、内部の整数を128-bit以上にする。 `boost::multiprecision::cpp_int` を使って任意精度整数にしてもよい。

```cpp
using Num = int64_t;

template <typename T=__int128>
Num power(Num base, Num index, Num mod) {
    const Num x = base % mod;
    if (x == 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    T acc {1};
    T doubled = x;
    auto i = index;

    while(i > 0) {
        if ((i & 1) != 0) {
            acc *= doubled;
            acc %= mod;
        }

        doubled *= doubled;
        doubled %= mod;
        i /= 2;
    }

    return static_cast<Num>(acc);
}
```

### aのb乗のc乗 mod M

[こちら](https://stackoverflow.com/questions/4223313/finding-abc-mod-m) を参照。

ある条件の下で、 $a^{b^c} mod M = a^{b^c mod (M-1)} mod M$ になる。

### floor(A/B) mod M

$A,B,M$ が正の整数の時、 $\lfloor A/B \rfloor mod M$ を求める。$A$ は巨大な数、 $B$ は素数とは限らない、 $M$ は素数とする。こちらの [説明](https://x.com/tau0529/status/2030287155697004671) を参考にした。

$D = BM$ とする。 $A = SD + R , 0 \leq R < D$ と表現できる。

$A/B = SD/B + R/B = SBM/B + R/B = SM + R/B$ と表現できる。 $A/B = SM + R/B$ を $M$ で割った余りは、 $A/B \equiv R/B mod M$ である。よって $A$ を何らかの方法で構築するときに、 $A mod BM$ を取りながら計算して、 $B$ で割って、 $M$ で割った余りを求めればよい。

### 2のべき乗のべき乗

2のべき乗と、2のべき乗のべき乗 2, 4, 16, 256,... を求める。まずRとgmpパッケージで求める。

```r
nums <- purrr::map_dbl(1:128, ~ gmp::asNumeric(gmp::pow.bigz(2, .x) %% 1000000007))

purrr::reduce(.x=2:32, .init=list(gmp::as.bigz(2)), .f=function(acc, i) {
   v <- acc[[NROW(acc)]]
   v <- v * v
   v <- v %% 1000000007
   c(acc, list(v))
}) %>%
  purrr::map_dbl(gmp::asNumeric)
```

C++ではこう実装する。Rの結果でユニットテストする。

```cpp
#include <vector>
#include <atcoder/modint.hpp>

namespace {
    using Num = int;
    using ModInt = atcoder::modint1000000007;
}

std::vector<ModInt> two_to_the_power_of_two(void) {
    std::vector<ModInt> powered(32);
    powered.at(0) = 2;
    for(Num i{1}; i<32; ++i) {
        powered.at(i) = powered.at(i-1) * powered.at(i-1);
    }

    return powered;
}

ModInt two_to_the_n_power(Num n) {
    const auto powered = two_to_the_power_of_two();

    ModInt answer = 1;
    Num index = 0;
    Num r = n;
    while(r > 0) {
        if ((r & 1) > 0) {
            answer *= powered.at(index);
        }
        r >>= 1;
        ++index;
    }

    return answer;
}
```

### long long int 除算の丸め

floor(負の無限大方向に丸め)、ceil(正の無限大方向に丸め)、round(0方向に丸め)を作る。倍精度浮動小数では精度が足りない。

分子(dividend)/分母(divisor)について商(quotient)と余り(remainder)の組を返し、 $dividend = quotient \times divisor + remainder$ を満たす。分子は正でも負でも0でもよいが、分母は正であることを前提とする。

- 商の符号は、分母と同じか0である
- 余りの符号は、floorは正または0、ceilは負または0、roundは分母と同じか0である

```cpp
namespace {
    using Num = long long int;
}

std::pair<Num, Num> ifloor(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = p % q;
        quotient = p / q;
    } else {
        remainder = (q - (std::abs(p) % q)) % q;
        quotient = (p - remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iceil(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    Num remainder {0};
    Num quotient {0};
    if (p >= 0) {
        remainder = (q - p % q) % q;
        quotient = (p + remainder) / q;
    } else {
        remainder = std::abs(p) % q;
        quotient = (p + remainder) / q;
    }

    const std::pair<Num, Num> answer {quotient, -rem_sign * remainder};
    return answer;
}

std::pair<Num, Num> iround(Num dividend, Num divisor) {
    Num p = dividend;
    Num q = divisor;

    Num rem_sign = 1;
    if (divisor < 0) {
        p = -p;
        q = -q;
        rem_sign = -1;
    }

    if ((p == 0) || (q == 0)) {
        const std::pair<Num, Num> zero {0LL, 0LL};
        return zero;
    }

    const Num sign = (p > 0) ? 1 : -1;
    const Num abs_p = std::abs(p);
    const Num quotient = sign * (abs_p / q);
    const Num remainder = sign * (abs_p % q);
    const std::pair<Num, Num> answer {quotient, rem_sign * remainder};
    return answer;
}
```

ABC 239-B の解説に従うと、もっと簡潔に書ける。ここでは10で割っているので、分母は適宜変更する。

- 正の数を正の数で割ったとき、C++では絶対値が小さい方向の商と、ゼロまたは正のあまりを得る
- 負の数を正の数で割ったとき、C++では絶対値が小さい方向の商と、ゼロまたは負のあまりを得る

#### 切り上げ

```cpp
void solve(std::istream& is, std::ostream& os) {
    Num x {0};
    is >> x;
    const Num div = (x / 10) + ((x % 10) > 0);
    os << div << "\n";
}
```

#### 切り捨て

```cpp
void solve(std::istream& is, std::ostream& os) {
    Num x {0};
    is >> x;
    const Num div = (x / 10) - ((x % 10) < 0);
    os << div << "\n";
}
```

### 平方根

`double` で精度が足りないときは二分探索する。二分探索のオーバーフローを避けるため、 $\sqrt{intmax}$ ぎりぎりは扱えない。

```cpp
template<typename T>
T isqrt(const T& x) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    auto pred = [&](const auto& thr) {
        return (thr <= x) && ((thr * thr) <= x);
    };

    T upper {1};
    upper <<= (sizeof(T) * 4);
    upper -= 1;
    return *std::ranges::partition_point(std::views::iota(T{0}, upper), pred) - 1;
}
```

N乗根は `std::pow(x, inv_n)` から補正する。

```cpp
template<typename T, typename U>
T inverse_pow(const T& x, const U& n) {
    if (x <= 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    if (n == 0) {
        return (x == 0) ? 0 : 1;
    }

    if (n == 1) {
        return x;
    }

    if (n == 2) {
        return isqrt(x);
    }

    auto pred = [&](const auto& thr) {
        T prod {1};
        for(U i{0}; i<n; ++i) {
            prod *= thr;
            if (prod > x) {
                return false;
            }
        }

        return true;
    };

    double inv_n {1};
    inv_n /= n;
    const T center = std::floor(std::pow(x, inv_n));
    const T lower = std::max(T{0}, center - 2);
    const T upper = std::max(lower, center + 3);
    return *std::ranges::partition_point(std::views::iota(lower, upper + 1), pred) - 1;
}
```

### 固定小数

`std::round` でも大丈夫かもしれないが、念のため。 `std::atoll` は数値と解釈できない文字列に対して0を返すことを利用している。

```cpp
std::pair<long long int, long long int>
str_to_fixed_point(const std::string& s, size_t n_frac_digits) {
    using Int = long long int;
    const auto pos = s.find('.');
    if (pos == std::string::npos) {
        std::pair<Int, Int> num {std::atoll(s.c_str()), 0};
        return num;
    }

    const auto int_part_str = s.substr(0, pos);
    const Int int_part = std::atoll(int_part_str.c_str());

    const auto len = std::min(s.size() - pos - 1, n_frac_digits);
    const auto frac_part_str = s.substr(pos + 1, len);
    Int frac_part = std::atoll(frac_part_str.c_str());

    for(size_t i = frac_part_str.size(); i < n_frac_digits; ++i) {
        frac_part *= 10;
    }

    std::pair<Int, Int> num {int_part, frac_part};
    return num;
}

const auto [int_part, frac_part] = str_to_fixed_point("65.4321", 4);
```

### 回文

文字列に限らず使える。

```cpp
// あるシーケンスが回文かどうか判定する
template <typename T>
bool is_palindrome(const T& s) {
    return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
};

// あるシーケンスが他方を反転したものかどうか判定する
template <typename T, typename U>
bool are_flipped(const T& s, const U& t) {
    return std::equal(begin(s), end(s), rbegin(t), rend(t));
};
```

### 正規表現

A,B問題で悩んだら正規表現で解く方が速い。記法で悩まないようにABC 345-A の解答を載せる。

```cpp
void solve(std::istream& is, std::ostream& os) {
    std::string s;
    is >> s;

    std::regex re(R"(^<=+>$)");
    if (std::regex_match(s, re)) {
        os << "Yes\n";
    } else {
        os << "No\n";
    }
}
```

### 連続しないかもしれない部分文字列

$target$ から連続しないかもしれない文字を順番を保って取り出し、 $sub$ の先頭何文字に一致するか調べる。

```cpp
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
```

### ローリングハッシュ

文字列を整数のダイジェストに圧縮する。二つの文字列がおそらく同じということを判定するために用いる。ダイジェストが32-bit整数だとハッシュ衝突が頻発するので、複数の素数でダイジェストを作る。もっと衝突が少なくて速い実装は[こちら](https://qiita.com/keymoon/items/11fac5627672a6d6a9f6)。

なお left, right は0-based indexingで、先頭が0文字目でleft文字目を含みとright文字目を含まない。こうしておかないと二つの文字列のハッシュから、結合した文字列のハッシュを計算できない。

```cpp
namespace {
    using Num = long long int;
}

struct StringHash {
    std::string s;
    Num b {0};
    Num mod {0};
    std::vector<Num> digests;
    std::vector<Num> multipliers;

    StringHash(const std::string& arg_s, Num arg_b, Num arg_mod) :
        s(arg_s), b(arg_b), mod(arg_mod) {
        auto size = s.size();
        Num digest = 0;
        Num multiplier = 1;
        digests.push_back(digest);
        multipliers.push_back(multiplier);

        for(decltype(size) i{0}; i<size; ++i) {
            const Num c = (mod + static_cast<Num>(s.at(i))) % mod;
            digest = ((digest * b) + c) % mod;
            digests.push_back(digest);
            multiplier = (multiplier * b) % mod;
            multipliers.push_back(multiplier);
        }
    }

    Num digest(Num left, Num right) const {
        return (mod + digests.at(right) -
                (multipliers.at(right - left) * digests.at(left)) % mod)
            % mod;
    }

    Num merge(Num left_digest, Num right_digest, Num right_len) const {
        return (right_digest + (left_digest * multipliers.at(right_len)) % mod) % mod;
    }
};
```

### ランレングス符号化

先頭の位置を0とする。

```cpp
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

using Rle = RunLength<Num>;
const std::vector<Num> input {3,3,3,3,3};
const auto rle = Rle(input);
```

センチネルをある数と指定できるなら、このように簡潔に実装できる。

```cpp
namespace {
    using Num = long long int;
}

std::vector<std::pair<Num,Num>> rle_func(const std::vector<Num>& vs) {
    Num prev = std::numeric_limits<Num>::min();
    Num len {0};
    std::vector<std::pair<Num,Num>> runs;

    for(const auto& v : vs) {
        if (prev != v){
            if (len > 0) {
                runs.emplace_back(prev, len);
            }
            prev = v;
            len = 0;
        }
        ++len;
    }

    runs.emplace_back(prev, len);
    return runs;
}
```

### 二分探索

整数の二分探索は、 $left$ が条件を満たす最大値、 $right$ が条件を満たさない最小値、が最終状態になるよう実装する。以下の方法なら $(right + left) / 2 = left$ でも正しく動く。これを誤ると区間が1または2から小さくならず無限ループに陥る。詳しくは[解説](https://atcoder.jp/contests/abc192/editorial/782)を参照すること。

```cpp
Num left = 0;
Num right = m;
while((right - left) > 1) {
    Num base = (left + right) / 2;
    if (eval(base)) {
        left = base;
    } else {
        right = base;
    }
}
```

### 三分探索

下に凸または単調な関数を三分探索する。

```cpp
using Numeric = double;
using Func = Numeric(&)(Numeric);

Numeric ternary_search(Numeric lower, Numeric upper, Func func) {
    constexpr Numeric tolerance = 1e-15;
    Numeric left {lower};
    Numeric right {upper};

    while(std::abs(left - right) > tolerance) {
        Numeric relative = std::max(std::abs(left), std::abs(right));
        relative = std::abs(left - right) / std::max(relative, tolerance);
        if (relative < tolerance) {
            break;
        }

        Numeric midleft = (left * 2.0 + right) / 3.0;
        Numeric midright = (left + right * 2.0) / 3.0;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    return left;
}
```

```cpp
namespace {
    using Num = long long int;
    using FuncResult = long long int;
    using Func = FuncResult(&)(Num);
    using SearchResult = std::pair<FuncResult,Num>;
}

SearchResult ternary_search(Num lower, Num upper, Func func) {
    Num left {lower};
    Num right {upper};

    while(std::abs(left - right) > 3) {
        const Num midleft = (left * 2 + right) / 3;
        const Num midright = (left + right * 2) / 3;
        const auto value_midleft = func(midleft);
        const auto value_midright = func(midright);

        if (value_midleft > value_midright) {
            left = midleft;
        } else {
            right = midright;
        }
    }

    std::vector<SearchResult> ps;
    for(Num i{left}; i<=right; ++i) {
        ps.emplace_back(func(i), i);
    }

    return *std::ranges::min_element(ps);
}
```

### 尺取り法

229-Dの一部である。ほぼ公式解説の実装そのままである。Rightをできるだけ伸ばし、rightが伸びきったらleftを伸ばしていく。

```cpp
Num right {1};
Num answer {0};
for(Num left{1}; left<=n; ++left) {
    while((right <= n) && ((cumsum.at(right) - cumsum.at(left-1)) <= k)) {
        ++right;
    }
    answer = std::max(answer, right - left);
}
```

### グラフ

隣接行列ではなく辺の集合で、疎なグラフを表現する。

- 有向グラフは from頂点番号 - to頂点番号 で管理する。無向グラフは辺を双方向に張った有向グラフにすればよい。
- 辺はfrom頂点の番号をキー、to頂点と属性(辺の重みなど)を値とする、配列か連想配列にする。頂点数を `n` として以下のどれかを選ぶ。
  - 辺を読み込んだら変えないなら `std::vector<std::vector<ToNode>>` 。頂点番号は通常 1-based indexing なので、要素数は `n+1` 個にする。
  - 辺を後から減らしたいなら `std::map<Num, std::vector<ToNode>>` にする。あらかじめ頂点数を指定しない。
  - To頂点集合が `std::vector<ToNode>` なら後から `ToNode` をソートできる。 `vector` の代わりに `std::set<ToNode>` にすれば、辺を追加するごとに自動的にソートできる。要素にランダムアクセスするコストが $log(n)$ 倍になることに注意。

### オイラーツアー

頂点をいつ追加するか悩むので一度実装してテストしておく。

```cpp
using Num = long long int;
using Edges = std::vector<std::vector<Num>>;
using Path = std::vector<Num>;

void eular_tour(Num current, Num prev, const Edges& edges, Path& path) {
    path.push_back(current);

    for(const auto& next : edges[current]) {
        if (next != prev) {
            eular_tour(next, current, edges, path);
            path.push_back(current);
        }
    }
}
```

### Lowest Common Ancestor (LCA)

オイラーツアーを使ってLCAを求めることができる。LCAを載せるセグメント木にはオイラーツアーでつけた頂点の通し番号が付く。同様に根から葉に向かう枝の重みと、葉から根に向かう枝の重み(これは枝の重みの符号を反転させたもの)もセグメント木に載せる。そうすると二つの頂点間の最短距離が求まる(二つの頂点からみてLCAより先の枝の重みは、正負打ち消し合って0になるので)。

コードは非常に長いので[こちら](https://github.com/zettsu-t/kyopro-tessoku-answers/blob/main/snipetts/lca.cpp) に載せた。

### 木の頂点をBFSで網羅する

頂点の数=辺の数+1かつ全部の頂点が連結なら、問題に明示されていなくても木である。

- 訪問する頂点とその順番をキュー(優先度キューではない)を使って管理する。
- キューに載せるのは辺の両端の頂点である。辺の訪問元(頂点番号: 根を訪問するときは-1など頂点番号になりえない値)、辺の訪問先(有効な頂点番号)、追加情報である。この追加情報に何を載せるかでさまざまな問題が解ける。
- 訪問元は無限ループ(今きた辺をすぐ逆にたどる)を避けるために使う
- キューの先頭は根の頂点にする。木の場合は、根はなんでもよくてとりあえず1番頂点に固定して済むことが多い。
- 訪問先に隣接する頂点をキューの末尾に追加する。そうすることで根から近い頂点から順番に訪問する。
- 入力が木なら、その定義上ループはありえないのでループ検出は要らない。

追加情報に何を載せるかで、色々な問題が解ける。例えば根から各頂点までの距離を得たいなら、根は0、それ以外は訪問元の距離+1とする。以下は ABC 126-Dのコード片である。

```cpp
struct Candidate {
     Num from {0};
     Num to {0};
     Num color {0};
};

std::vector<Num> colors(n+1, 0);
std::queue<Candidate> candidates;
candidates.push(Candidate{-1, 1, 0});

while(!candidates.empty()) {
    // popするので参照ではなく値で受け取る
    const auto e = candidates.front();
    candidates.pop();
    const auto current = e.to;

    // 次の辺がなければ空集合
    for(const auto& node : edges[current]) {
        const auto& next = node.id;
        // 来た辺をすぐ戻らない
        if (next == e.from) {
            continue;
        }

        // この味付け次第で様々な問題が解ける
        const auto color = (node.weight + e.color) & 1;
        colors.at(next) = color;
        candidates.push(Candidate{current, next, color});
    }
}
```

### 頂点間の距離が固定のとき最短距離を求める

頂点間の距離が固定なら、ダイクストラ法ではなくBFSで解ける。ダイクストラ法よりBFSの方が計算量が小さい。既知の最短距離より長い経路を早めに刈る。

```cpp
struct Candidate {
    Num from {0};
    Num to {0};
    Num distance {0};
};

constexpr Num inf = 1000000000000LL;
Num max_distance {0};
std::vector<Num> distances(size, inf);
std::queue<Candidate> candidates;
candidates.push(Candidate{-1, i, 0});

while(!candidates.empty()) {
    const auto e = candidates.front();
    candidates.pop();
    const auto current = e.to;
    // 最短距離ではない
    if (distances.at(current) <= e.distance) {
        continue;
    }

    const auto min_dist = std::min(distances.at(current), e.distance);
    distances.at(current) = min_dist;
    max_distance = std::max(max_distance, min_dist);

    const auto next_dist = e.distance + 1;
    for(const auto& next : edges.at(current)) {
        if ((next == e.from) || (distances.at(next) <= next_dist)) {
            continue;
        }
        candidates.push(Candidate{current, next, next_dist});
    }
}
```

### ダイクストラ法

頂点間の距離が固定なら、ダイクストラ法を使う。優先度キューを使って計算量を減らすことと、経路を余計にたどらないように気を付ける。BFSと同様、辺には好きな情報を追加する。

```cpp
using Num = long long int;
constexpr Num inf = std::numeric_limits<Num>::max() / 2;

struct Edge {
    Num to {0};
    Num distance {0};
};
using Edges = std::vector<std::vector<Edge>>;
using Distances = std::vector<Num>;

void dijkstra_algo(Num n_nodes, Num start, const Edges& edges, Distances& distances) {
    Distances zeros(n_nodes+1, inf);
    std::swap(distances, zeros);

    using Node = std::pair<Num, Num>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
    distances.at(start) = 0;
    q.push(std::make_pair(0, start));

    while(!q.empty()) {
        const auto [distance, from] = q.top();
        q.pop();

        if (distances.at(from) < distance) {
            continue;
        }

        for(const auto& edge : edges.at(from)) {
            const auto& to = edge.to;
            auto to_dist = distance + edge.distance;
            if (to_dist < distances.at(to)) {
                distances.at(to) = to_dist;
                q.push(std::make_pair(to_dist, to));
            }
        }
    }

    return;
}
```

最大距離問題もダイクストラ法で解ける。ただし優先度キューに積むものととして、最大距離を最初のキーにすると探索回数が多すぎてTLEすることがある。どのような頂点順に確定すると最大距離が順に求まるか、問題ごとに設定する必要がある。

### ワーシャルフロイド法

外ループから順に、経由地、出発地、到着地にする。この順番を間違えると上手くいかない。

```cpp
using Num = long long int;
using Distance = long long int;
constexpr Num MaxWidth = 1000;
using Matrix = std::array<std::array<Num, MaxWidth>, MaxWidth>;

void floyd_warshall(Num n, Matrix& distances) {
    for(decltype(n) i{0}; i<n; ++i) {
        for(decltype(n) x{0}; x<n; ++x) {
            for(decltype(n) y{0}; y<n; ++y) {
                distances[x][y] = std::min(distances[x][y], distances[x][i] + distances[i][y]);
            }
        }
    }
}

void init_array(Matrix& distances, Num value) {
    for(Num i{0}; i<MaxWidth; ++i) {
        for(Num j{0}; j<MaxWidth; ++j) {
            distances[i][j] = value;
        }
    }
}
```

### クラスカル法

最小全域木を作る。コストが安い辺から順に、連結成分が増える(ループにならない)辺を追加する。Union-find木を使うと簡潔に書ける。以下は 218-E のコードの要点である。辺の重みで並び替えるので、辺の重みをtupleの第一要素にする。

```cpp
using Num = long long int;
using Edge = std::tuple<Num, Num, Num>;
std::vector<Edge> edges;
atcoder::dsu tree(n + 1);

for(decltype(m) i{1}; i<=m; ++i) {
    edges.push_back(std::make_tuple(c, a, b));
}
std::sort(edges.begin(), edges.end());

for(const auto& [c, a, b] : edges) {
    if (!tree.same(a, b)) {
        tree.merge(a, b);
    }
}
```

### トポロジカルソート

トポロジカルソートに一意性はないが、辞書順で最小のものを得ることはできる。

トポロジカルソートに一意性があるかどうかは、ソート済の隣同士の頂点に枝があるかどうかで分かる。ある隣同士の頂点に枝がなければ、それらの頂点は入れ替え可能なので、そのトポロジカルソートに一意性はない。あるいはソート中に次に選ぶ頂点が常に一意ならトポロジカルソートも一意である。

```cpp
using Num = long long int;
using Nodes = std::vector<Num>;
using Degrees = std::vector<Num>;
using Graph = std::vector<Nodes>;

bool topological_sort(Num n, const Graph& graph, const Degrees& in_degrees, Nodes& answer) {
    auto updated_in_degrees = in_degrees;
    std::priority_queue<Num, std::vector<Num>, std::greater<Num>> candidates;

    for(decltype(n) i{0}; i<n; ++i) {
        if (updated_in_degrees.at(i) == 0) {
            candidates.push(i);
        }
    }

    Nodes sorted_nodes;
    bool unique {true};

    while(!candidates.empty()) {
        unique &= (candidates.size() == 1);
        const auto current = candidates.top();
        candidates.pop();
        sorted_nodes.push_back(current);

        for(const auto& next : graph[current]) {
            updated_in_degrees.at(next) -= 1;
            if (updated_in_degrees.at(next) <= 0) {
                candidates.push(next);
            }
        }
    }

    std::swap(answer, sorted_nodes);
    return unique && (static_cast<Num>(answer.size()) == n);
}
```

### フェニック木

Binary Indexed Tree (BIT) ともいう。ACLの `atcoder::fenwick_tree<T>` として用意されている。

```cpp
using Num = long long int;

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
```

### 座標圧縮

`std::set` , `std::map` をiterateして連番を振る、というのが一つの方法である。もう一つはABC 354-Fの公式解説に載っていた方法が以下である。

```cpp
void compress(Vec& v) {
    Vec sorted = v;
    std::ranges::sort(sorted);
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    for(auto&& x : v) {
        x = std::ranges::lower_bound(sorted, x) - sorted.begin();
    }
}
```

あるいはランレングス圧縮と同様に、一度 `std::vector` にする。

```cpp
namespace {
    using Num = long long int;
}

std::map<Num,Num> dense_rank(std::vector<Num>& vs) {
    std::sort(vs.begin(), vs.end());
    std::map<Num,Num> tbl;
    Num prev = std::numeric_limits<Num>::min();
    Num serial {0};

    for(const auto& v : vs) {
        if (v != prev) {
            tbl[v] = serial;
            ++serial;
        }
        prev = v;
    }

    return tbl;
}
```

### Longest Increasing Subsequence (LIS)

蟻本65ページ目の説明が分かりやすい。

```cpp
size_t lis(const std::vector<Num>& xs) {
    std::vector<Num> lis;
    for (const auto& a : xs) {
        auto it = std::lower_bound(lis.begin(), lis.end(), a);
        if (it == lis.end()) {
            lis.push_back(a);
        } else {
            *it = a;
        }
    }

    return lis.size();
}
```

### Mo's Algorithm

ある区間に $N$ 個の要素があって、区間 $[L,R]$ について何か値を求める(例えば何種類の要素があるか)ことを考える。ここで $L$ または $R$ を1個前後して値を更新するするコストを $O(1)$ とする。

クエリが $Q$ 回あるときに評価回数が $O(QN)$ だと辛いので回数を減らしたい。クエリによって答えが変わらず(既に載っているデータを評価するだけ)クエリを先読みできるなら、Mo's Algorithm を用いて $O(N \sqrt{N})$ に軽減できる。

わかりやすい実装を他の方が[記事](https://ei1333.hateblo.jp/entry/2017/09/11/211011) にまとめている。

### 遅延セグメント木

#### 区間加算、区間和

AWC 0014-E の解説を見た。

```cpp
struct Node {
    Num value {0};
    Num width {0};
};

using Op = Num;

Node func_operate(Node a, Node b) {
    const Node c {a.value + b.value, a.width + b.width};
    return c;
}

Node func_unit() {
    const Node node {0, 0};
    return node;
}

Node func_map(Op a, Node b) {
    const Node node {b.value + a * b.width, b.width};
    return node;
}

Op func_compose(Op a, Op b) {
    return a + b;
}

Op func_id() {
    const Op op {0};
    return op;
}

atcoder::lazy_segtree<
    Node, func_operate, func_unit, Op, func_map, func_compose, func_id
> tree(n);
```

#### 区間加算、区間和(二乗和)

ABC 455-Fの他の方の実装を見た。

```cpp
struct Node {
    ModInt size {0};
    ModInt sum {0};
    ModInt sqsum {0};
};

Node func_operate(Node a, Node b) {
    const Node node {
        a.size + b.size,
        a.sum + b.sum,
        a.sqsum + b.sqsum,
    };
    return node;
}

Node func_unit() {
    const Node node;
    return node;
}

Node func_map(ModInt f, Node a) {
    const Node node {
        a.size,
        a.sum + a.size * f,
        a.sqsum + 2 * f * a.sum + f * f * a.size,
    };
    return node;
}

ModInt func_compose(ModInt f, ModInt g) {
    return f + g;
}

ModInt func_id() {
    return 0;
}

atcoder::lazy_segtree<
    Node, func_operate, func_unit, ModInt, func_map, func_compose, func_id
> tree(n);
```

## コーディングスタイル

### インクルード文

あれこれ悩むくらいなら全部入りにする。一般的なC++のコーディングスタイルとしては歓迎されないが、一秒でも速く解くことが目的ならいいだろう。使わない定義があると警告が出たら余計なインクルード文を除く。

```cpp
#include <bits/stdc++.h>
#include <boost/multi_array.hpp>
#include <atcoder/all>
```

### 型

最初に強調しておくと、整数オーバーフローでWAすると非常に分かりにくいので、整数は 64-bit で扱う。 64-bit では収まらないならアルゴリズムを工夫する。

- 改めて、整数は `long long int` にして、別名をつける。32-bitでは表現できない整数があるので、ライブラリなどが `int` を要求するとき以外は  `long long int` (最低64-bit)にすると間違いが起きない。よく `ll` と略されるが、個人的には `Num` のようにユーザ定義型は大文字始まりの CamelCase にしたい(これは個人の好み)。
- 素数で割った値 (mod M) を格納する整数は `atcoder::modint1000000007` を使う。元となる素数は出題に従い、元を指定して型を定義するには `static_modint<1000000007>` と書く。非負の `int` と演算できる。
- `std::` を省略するとコンパイルエラーで悩むので、個人的には `using namespace std;` で`std::` を省略するのは好まない。タイピング量を減らすために `std::` を省略する人が多いのは確か(これも個人の好み)。

```cpp
namespace {
    using Num = long long int;
    using ModInt = atcoder::modint1000000007;
}
```

### 入出力

入力を `std::istream& is` から受け取り、 `std::ostream& os` に出力する。 `std::cin` から受け取り `std::cout` に出力しても問題ないが、ユニットテストを書くときに `std::istringstream` , `std::ostringstream` に差し替えられて便利である。タイピング量も減る。

```cpp
void solve(std::istream& is, std::ostream& os);
```

改行は `"\n"` にする。 `std::endl` は遅いので、出力ストリームのフラッシュが明示的に必要な時(ABC 299-Dなど)以外は使わない。

### STL and Ranges

#### 要素があるかどうか

現時点でAtCoderで用いているg++のバージョンは12.2なので、C++20 Rangesが使える。よって、C++17までの

```cpp
std::sort(vec.begin(), vec.end());
```

を

```cpp
std::ranges::sort(vec);
```

と書ける。つまり `vec.begin(), vec.end()` と書く代わりに `vec` で全域を操作する。STLにあるものは、たいていRangesにもあるし、イテレータが必要ならイテレータを得られる。

コンテナに有効な要素があるかどうか調べるのに、イテレータが `iter != arr.end()` の検査は煩雑で忘れやすいので書かないように工夫する。

`std::map<T> table` の `table[i]` は、要素がなければデフォルトコンストラクタが呼ばれる。よって to頂点を追加するときにfrom頂点が既にあるかどうかを `!= table.end()` で確認する必要はない。To頂点を読み出したとき、要素がなければ空集合のコンテナなり整数0なりが返る。逆に要素0を追加したくなければ、 `!= table.end()` で確認する。

#### 構造体をソートする

構造体をソートするためには、比較演算子が必要である。きちんとしたコードなら構造体とメンバと比較演算子を定義すべきである。しかし比較演算子を定義する時間がもったいない早解きで、比較方法が一通りしかないなら、 `std::tuple` を使うと辞書順の比較演算子がついてくる。辞書順なのでメンバの順番に気を付ける。

```cpp
struct Group {
    Num num {0};
    Num size {0};
    bool operator<(const Group& rhs) const {
        return std::tie(num, size) > std::tie(rhs.num, rhs.size);
    }
};
```

```cpp
using Group = std::tuple<Num, Num>;
std::vector<Group> groups;
std::sort(groups.begin(), groups.end(), std::greater<Group>{});
```
