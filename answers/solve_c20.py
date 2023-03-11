#!/usr/bin/python3
# coding: utf-8

'''
競技プログラミングの鉄則 問題C20
について、公開されているテストケース100個を解いて得点を求めます。

実行方法(C20の実行ファイルはmakeするなりしてあらかじめ用意してください)
$ python3 solve_c20.py

グローバルな定数は以下の用途に使います。適宜ご変更ください。
- DEFAULT_TEST_CASE_TOPDIR は、公開されているテストケースの.zipを展開したディレクトリ名です。
- COMMAND_LINE は、問題を解く実行ファイルを起動するためのコマンドと引数です。
  第一引数をscoreにしているのは得点を表示させるためです。得点を表示させると
  WAになってしまうので、第一引数をscoreにしない得点を表示させません。
- TIMEOUT_SECは、実行を打ち切るためタイムアウト(単位は秒)です。
'''

import glob
import os
import re
import subprocess
import natsort

DEFAULT_TEST_CASE = "testcase/c20/sample-in"
COMMAND_LINE = ["./c20", "score"]
TIMEOUT_SEC = 1


def solve_one(command_line, input_filename):
    """
    テストケースを解く
    """
    score = 0
    with open(input_filename, mode="r", encoding="utf-8") as infile:
        proc = subprocess.run(command_line, stdin=infile,
                              stdout=subprocess.PIPE, timeout=TIMEOUT_SEC, check=False, text=True)
        answer = proc.stdout
        replaced = re.sub(r"^\d+", "", answer, flags=re.MULTILINE)
        replaced = re.sub(r"\r", "", replaced)
        replaced = re.sub(r"\n+", "\n", replaced)
        score = int(re.sub(r"\D", "", replaced))
    return score


def solve_all(command_line, test_case_dir):
    """
    すべてのテストケースを解く
    """
    input_files = natsort.natsorted(glob.glob(os.path.join(test_case_dir, "*.txt")))
    total = 0
    for input_filename in input_files:
        score = solve_one(command_line=command_line, input_filename=input_filename)
        print(f"{score:>7} : {input_filename}")
        total = total + score
    print(f"Total : {total}")


def main():
    """
    一通り実行する
    """
    solve_all(command_line=COMMAND_LINE, test_case_dir=DEFAULT_TEST_CASE)


if __name__ == "__main__":
    main()
