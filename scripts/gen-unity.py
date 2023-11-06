#!/usr/bin/env python3

import argparse
import sys


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--out")
    p.add_argument("file", nargs="+")

    ns = p.parse_args(sys.argv[1:])
    with open(ns.out, "w") as f:
        for fn in ns.file:
            f.write(f'#include "{fn}"')

if __name__ == "__main__":
    main()

