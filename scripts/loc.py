#!/usr/bin/env python3

import glob
import os
import sys
import re

SRC_DIRS = [
    "../bin",
    "../lib",
    "../include",
]

RE_SINGLE_LINE_COMMENT = re.compile(r"^\s*//.*?$", re.MULTILINE)
RE_COMMENT = re.compile(r"^\s*/\*.*?\*/", re.MULTILINE | re.DOTALL)

def src_files():
    thisdir = os.path.dirname(sys.argv[0])   

    src_dirs = [os.path.join(thisdir, d) for d in SRC_DIRS]

    srcs = []
    for d in src_dirs:
        raw_srcs = glob.glob("./**/*.cpp", root_dir=d, recursive=True) \
                + glob.glob("./**/*.hpp", root_dir=d, recursive=True)
        srcs.extend([os.path.join(d, s) for s in raw_srcs])

    return [os.path.normpath(s) for s in srcs]

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
    data = read(src_files())
    data = remove_one_line_comments(data)
    data = remove_comments(data)
    lines = [l for l in data.split("\n") if len(l.strip()) > 0]
    print(len(lines))


if __name__ == "__main__":
    main()

