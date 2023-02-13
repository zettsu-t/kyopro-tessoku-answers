# kyopro-tessoku-answers

「競技プログラミングの鉄則 ～アルゴリズムと思考力を高める 77 の技術～」(米田優峻 著, 2022/9, マイナビ出版)の問題に順に解答していきます。著者のサイトは[こちら](https://github.com/E869120/kyopro-tessoku)です。

## 概要

- [自動採点システム](https://atcoder.jp/contests/tessoku-book)にACした解答から載せます。
- コーディングのスタイルは、一般的なC++のコードとして好ましいものにします。たとえば配列を固定長のグローバル変数で確保することはしない、といったことです。
- 関数が長くてユニットテストできないのは割り切ります。
- コンパイル環境はこのレポジトリのDockerfileで作成し、[ルール](https://atcoder.jp/contests/APG4b/rules?lang=ja)に従ってC++17を使います。C++20は使いません。
- このレポジトリは私の練習帳なので、もっとよいコードが多数あると思います。探してみてください。
- まず全問ACすることが私の目標です。複雑な分岐をエレガントに書き換えるのはそのあとの課題です。

## ビルド環境を構築して起動する

```bash
docker-compose build
docker-compose up -d
docker-compose exec kyopro-tessoku-answers /bin/bash
docker-compose down
```

## 解答を実行する

コンテナ内で作業ディレクトリに移動して `make` すると、各問について実行可能な解答を作成します。実行可能ファイル名は、問題番号と同じです(問題A01の解答はa01)。

```bash
cd /home/work/
make -j
./a01
./b01
```
