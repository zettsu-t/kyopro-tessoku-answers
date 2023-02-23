#!/usr/bin/python3
# coding: utf-8

'''
競技プログラミングの鉄則 演習問題集の自動採点システム
https://atcoder.jp/contests/tessoku-book
でテストケースが公開されているので、テストケースを手元にダウンロードして
通すためのスクリプトです。

テストケース一式は、ディレクトリ名=問題番号です。
つまり問題A01のテストケース一式は、A01/ にあるとします。

起動方法
$ python3 solve.py A01 [testcase-dir]
を実行すると、問題A01のテストケースを通して結果の正誤を表示します。
問題番号の大文字と小文字は区別しません。

省略可能な第二引数 testcase-dir を指定すると、テストケースがそのディレクトリ直下に
あると想定してテストケースを探します。省略時はカレントディレクトリ相対で、
このスクリプトの DEFAULT_TEST_CASE_DIR に書いた通りです。

DEFAULT_TEST_CASE_TOPDIR = "testcase" なら、問題A01のテストケースは、
- ./testcase/A01/in*.txt は、実行ファイルが標準入力から読むテキスト
- ./testcase/A01/out*.txt は、実行ファイルが標準出力に書くべき正解
です。正解が一意でない場合は扱いません。

TLE(制限時間超過)で打ち切ることはないので、いつまで経っても答えが返ってこない場合は、
ctrl-cを入力して手動で打ち切ってください。
'''

import glob
import os
import re
import subprocess
import sys
import natsort

DEFAULT_TEST_CASE_TOPDIR = "testcase"
HELP_MESSAGE = "make && python3 solve.py problem-id"


def trim_lines(answer):
    """
    正解と解答を単純な文字列一致比較にするため、曖昧な表記を統一する。
    各行の先頭と末尾にある空白文字を取り除く
    CRLF改行ならCRを取り除いてLF改行にする
    空行(LFしかない行)を取り除き、最終行はLFで終わる
    """
    replaced = re.sub(r"^\s*", "", answer, flags=re.MULTILINE)
    replaced = re.sub(r"\s*$", "", replaced, flags=re.MULTILINE)
    replaced = replaced + "\n"
    replaced = re.sub(r"\r", "", replaced)
    return re.sub(r"\n+", "\n", replaced)


def check(title, expected, actual):
    """
    正解(expected)と解答(actual)が一致するかどうか調べる。
    一致すればTrue、不一致ならFalseを返す
    titleにメッセージをつけて返す
    """
    result = trim_lines(expected) == trim_lines(actual)
    message = ""

    if trim_lines(expected) == trim_lines(actual):
        message = "AC"
    else:
        message = "WA"
    return (result, f"{title}: {message}")


class Problem:
    """
    問題
    """

    def __init__(self, problem_id, test_case_topdir):
        """
        指定した問題(problem_id)のテストケースを test_case_topdir/ から読んで準備する
        準備に失敗したら例外を投げる
        """
        # 問題の大文字小文字を区別しないが、内部的には統一する
        canonical_id = problem_id.strip().upper()
        test_case_dir = None
        exefile_name = None

        # テストケースのディレクトリ名は、大文字のみか小文字のみかどちらかとする
        for subdir_name in [canonical_id, canonical_id.lower()]:
            dir_name = os.path.join(test_case_topdir, subdir_name)
            if os.path.isdir(dir_name):
                test_case_dir = dir_name
                break

        # 実行ファイル名は、大文字のみか小文字のみかどちらかで、拡張子.exeの有無は問わない
        # 最後に見つかった実行ファイルを使う
        for base_name in [canonical_id, canonical_id.lower()]:
            for exe_name in [base_name, base_name + ".exe"]:
                filename = os.path.join("./", exe_name)
                if os.path.isfile(filename):
                    exefile_name = filename

        if test_case_dir is None:
            raise FileNotFoundError(
                f"Test cases of {canonical_id} not found\n")

        if exefile_name is None:
            raise FileNotFoundError(
                f"No executable for {canonical_id} found\n")

        self.exefile_name = exefile_name
        self.set_files(test_case_dir=test_case_dir)

    def set_files(self, test_case_dir):
        """
        テストケースのファイル名を設定する
        """
        input_dir = os.path.join(test_case_dir, "in")
        output_dir = os.path.join(test_case_dir, "out")
        input_files = natsort.natsorted(
            glob.glob(os.path.join(input_dir, "*.txt")))
        filenames = [os.path.basename(x) for x in input_files]
        expected_files = [os.path.join(output_dir, x) for x in filenames]

        self.input_files = input_files
        self.expected_files = expected_files

    def run(self):
        """
        テストケースを実行して、全件正解かそうでないかを返す
        """
        total = True

        for (input_filename, expected_filename) in zip(self.input_files, self.expected_files):
            title = os.path.basename(input_filename)
            with open(expected_filename, mode="r", encoding="utf-8") as infile:
                expected = infile.read()

            with open(input_filename, mode="r", encoding="utf-8") as infile:
                proc = subprocess.run(
                    [self.exefile_name], stdin=infile, stdout=subprocess.PIPE,
                    check=False, text=True)
                if proc.returncode == 0:
                    result, message = check(
                        title=title, expected=expected, actual=proc.stdout)
                    total = total and result
                    print(message)
                else:
                    print("Runtime error occured")
                    total = False

        if total:
            print("Passed")
        else:
            print("Failed")

        return 0 if total else 1


def main():
    """
    引数で指定したテストケースを実行する
    """
    if len(sys.argv) == 1:
        print(HELP_MESSAGE)
        return

    problem_id = sys.argv[1]
    test_case_topdir = DEFAULT_TEST_CASE_TOPDIR
    if len(sys.argv) > 2:
        test_case_topdir = sys.argv[2]

    exit_status_code = Problem(
        problem_id=problem_id, test_case_topdir=test_case_topdir).run()
    sys.exit(exit_status_code)


if __name__ == "__main__":
    main()
