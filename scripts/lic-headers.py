#!/usr/bin/env python3

import sys

import util

SPDX_HEADER = "/* SPDX-License-Identifier:  GPL-3.0-or-later */".strip()

def first_line(path: str):
    with open(path, "r") as f:
        return f.readline()


def check(srcs: list):
    failed=[]
    for s in srcs:
        line = first_line(s)
        if line.strip() != SPDX_HEADER:
            failed.append(s)

    if len(failed) > 0:
        print("# Missing license header for")
        print("\n".join(failed))
        return 1
    return 0


def inject(srcs: list):
    for s in srcs:
        line = first_line(s)
        if line.strip() != SPDX_HEADER:
            with open(s, "r") as f:
                lines = f.readlines()
            lines = [f"{SPDX_HEADER}\n"] + lines
            with open(s, "w") as f:
                f.writelines(lines)


def main():

    try:
        if len(sys.argv) != 2:
            raise ValueError(
                    f"Usage: {sys.argv[0]} 'commmand' where command is 'check' or 'inject'")
        srcs = util.src_files()

        if sys.argv[1] == "check":
            sys.exit(check(srcs))
        if sys.argv[1] == "inject":
            sys.exit(inject(srcs))
        raise ValueError(f"Invalid command '{sys.argv[1]}'")
    except ValueError as e:
        print(e)
        sys.exit(1)
    sys.exit(0)


if __name__ == "__main__":
    main()

