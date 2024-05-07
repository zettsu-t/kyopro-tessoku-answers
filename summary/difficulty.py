#!/usr/bin/python3
# python3 difficulty.py

import math
import sys
import re
import argparse

MAX_N_TASKS = 8
MAX_DIFFICULTY = 9
colors = ["?", "灰", "茶", "緑", "水", "青", "黄", "橙", "赤", "赤<"]

# 1行1コンテストに対応する。
# コンテスト名(もしあれば)三桁のID, ':', A..H問題の結果または難易度
re_contest_pattern = "^([\\D]*\\d{3}):(.{0," + str(MAX_N_TASKS) + "})"
re_id_pattern = "^[\\D]*(\\d{3})"

def collect_results(lower_id, upper_id, difficulty_filename, result_filename):
    difficulty_map = {}

    with open(difficulty_filename) as f:
        for line in f:
            s = line.strip()
            m = re.match(re_contest_pattern, s)
            if not m:
                continue
            key = m.groups()[0]
            value = m.groups()[1]
            difficulties = [0] * MAX_N_TASKS
            for index, d in enumerate(value):
                if d.isdigit():
                    difficulties[index] = int(d)
                difficulty_map[key] = difficulties

    result_table = []
    with open(result_filename) as f:
        for line in f:
            s = line.strip()
            m = re.match(re_contest_pattern, s)
            if not m:
                continue
            key = m.groups()[0]
            value = m.groups()[1]
            result = [" "] * MAX_N_TASKS
            for index, mark in enumerate(value):
                result[index] = mark
            result_table += [(key, result)]

    result_table.sort(key=lambda x: x[0])
    wins = [0] * MAX_DIFFICULTY
    timeouts = [0] * MAX_DIFFICULTY
    losses = [0] * MAX_DIFFICULTY

    result_lines = ""
    for [key, result] in result_table:
        m = re.match(re_id_pattern, key)
        id = int(m.groups()[0])
        if id < lower_id or id > upper_id:
            continue

        if not key in difficulty_map:
#           print("Warning: missing {}".format(key))
            continue

        difficulties = difficulty_map[key]
        line = ""
        for index, difficulty in enumerate(difficulties):
            difficulty_str = str(difficulty)
            mark = result[index]
            result_str = "  "
            if difficulty == 0:
                pass
            elif mark == "+":
                wins[difficulty] = wins[difficulty] + 1
                result_str = str(difficulty) + mark
            elif mark == ".":
                timeouts[difficulty] = timeouts[difficulty] + 1
                result_str = str(difficulty) + mark
            elif mark != " ":
                losses[difficulty] = losses[difficulty] + 1
                result_str = str(difficulty) + mark
            line += " " + result_str
        result_lines += key + ":" + line + "\n"

    n_wins = 0
    n_timeouts = 0
    n_losses = 0
    def num_width(x):
        if x <= 0:
            return 1
        return int(math.log10(x)) + 1

    for [g, [w, t, l]] in enumerate(zip(wins, timeouts, losses)):
        n_wins = n_wins + w
        n_timeouts = n_timeouts + t
        n_losses = n_losses + l

    width = max([num_width(x) for x in [n_wins, n_timeouts, n_losses]])
    summary_lines = "difficulty: solved + timeout + loss = total\n"

    for [g, [w, t, l]] in enumerate(zip(wins, timeouts, losses)):
        n = w + t + l
        if g < 1 or n <= 0:
            continue
        color = colors[g]
        line = f"{g}{color}: {w:{width}} + {t:{width}} + {l:{width}} = {n}"
        summary_lines = summary_lines + line + "\n"

    n_total = n_wins + n_timeouts + n_losses
    line = f"total: {n_wins} + {n_timeouts} + {n_losses} = {n_total}\n"
    summary_lines = summary_lines + line
    return (result_lines, summary_lines)

def parse_command_line():
    parser = argparse.ArgumentParser()
    # コンテスト番号の下限
    parser.add_argument("--lower_id", dest="lower_id", type=int,
                        default=0, help="Lowest contest ID")
    # コンテスト番号の上限
    parser.add_argument("--upper_id", dest="upper_id", type=int,
                        default=1000000, help="Hight contest ID")
    # 問題の難易度
    # "ABC297:112 5" はコンテスト ABC297の1..5問目の難易度が
    # 1(灰),1(灰),2(茶),不明,5(青) という意味。6問目以降は問題が無いか難易度が不明。
    # :の直後にA問題の結果を置く。:と結果の間には何も書かない。
    parser.add_argument("--difficulty_filename", dest="difficulty_filename", type=str,
                        default=None, help="Textfile for difficulty of tasks")

    # コンテスト名(,区切りで複数可)
    args_list = lambda x:list(map(str, x.split(',')))
    parser.add_argument("--contests", dest="contests", type=args_list,
                        default=["abc", "arc", "agc"], help="Names of contests")

    # 問題を解いた結果
    # "ABC297: ++ -" はコンテスト ABC297の1..5問目を
    # 未回答、解けた、解けた、未回答、解けなかったという意味。6問目以降は未回答。
    parser.add_argument("--result_filename", dest="result_filename", type=str,
                        default="results/results.txt", help="Textfiles for result of tasks")
    args = parser.parse_args()
    return args

def execute_all():
    args = parse_command_line()
    lower_id = args.lower_id
    upper_id = args.upper_id
    difficulty_filename = args.difficulty_filename
    result_filename = args.result_filename
    contests = args.contests

    difficulty_filenames = []
    if difficulty_filename:
        difficulty_filenames = [difficulty_filename]
    else:
        difficulty_filenames = ["incoming_data/difficulty_auto_{}.csv".format(name.lower()) for name in contests]

    for difficulty_filename in difficulty_filenames:
        result_lines, summary_lines = collect_results(
            lower_id = lower_id, upper_id = upper_id,
            difficulty_filename = difficulty_filename, result_filename = result_filename)
        print(result_lines)
        print(summary_lines)

if __name__ == "__main__":
    execute_all()
