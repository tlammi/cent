#!/usr/bin/env python3

import re

import util


RE_SINGLE_LINE_COMMENT = re.compile(r"^\s*//.*?$", re.MULTILINE)
RE_COMMENT = re.compile(r"^\s*/\*.*?\*/", re.MULTILINE | re.DOTALL)


def read(srcs: list):
    data = ""
    for s in srcs:
        with open(s, "r") as f:
            data += f.read()
    return data

def remove_one_line_comments(data: str):
    data = RE_SINGLE_LINE_COMMENT.sub("", data)
    return data

def remove_comments(data: str):
    data = RE_COMMENT.sub("", data)
    return data


def main():
    data = read(util.src_files())
    data = remove_one_line_comments(data)
    data = remove_comments(data)
    lines = [l for l in data.split("\n") if len(l.strip()) > 0]
    print(len(lines))


if __name__ == "__main__":
    main()

