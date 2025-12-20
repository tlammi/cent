#!/usr/bin/env python3

import re
import sys
import argparse

from pathlib import Path
from typing import Iterable
from dataclasses import dataclass

import yaml

ROOT = Path(__file__).parent.parent.resolve()
SRC = ROOT / "src"

META_RE = re.compile(r"""
^CENT_META\((.*?)\)
""", re.VERBOSE | re.MULTILINE | re.DOTALL)

@dataclass
class Regular:
    path: Path

@dataclass
class Test:
    path: Path
    suite: str

def parse_meta(path: Path):
    with open(path) as f:
        match = META_RE.search(f.read())
        if match is None:
            return None
        return yaml.safe_load(match.groups()[0])

def make_meta(p: Path) -> Regular | Test:
    res = parse_meta(p)
    if res is None:
        return Regular(p)
    return Test(path=p, suite=res["suite"])

def sources() -> Iterable[Regular | Test]:
    for s in SRC.rglob("*.cpp"):
        if not s.is_file():
            continue
        yield make_meta(s)

def generate():
    srcs = list(sources())

    index = {
        "srcs": [],
        "tests": {}
    }

    for s in srcs:
        if isinstance(s, Regular):
            index["srcs"].append(str(s.path.relative_to(ROOT)))
        elif isinstance(s, Test):
            index["tests"].setdefault(s.suite, [])
            index["tests"][s.suite].append(str(s.path.relative_to(ROOT)))
    with open(ROOT / ".index.yaml", "w") as f:
        yaml.dump(index, f)

def main():
    p = argparse.ArgumentParser()
    sp = p.add_subparsers(dest="cmd")
    sp.add_parser("generate")
    sp.add_parser("sources")
    sp.add_parser("suites")
    tests = sp.add_parser("tests")
    tests.add_argument("--suite", required=True, type=str)
    ns = p.parse_args()
    if ns.cmd == "generate":
        generate()
        return
    if ns.cmd == "sources":
        with open(ROOT / ".index.yaml") as f:
            obj = yaml.safe_load(f)
            print(" ".join(obj["srcs"]))
        return
    if ns.cmd == "suites":
        with open(ROOT / ".index.yaml") as f:
            obj = yaml.safe_load(f)
            print(" ".join(obj["tests"].keys()))
        return
    if ns.cmd == "tests":
        with open(ROOT/ ".index.yaml") as f:
            obj = yaml.safe_load(f)
            print(" ".join(obj["tests"][ns.suite]))
        return
    raise ValueError(f"Unknown subcommand: {ns.cmd}")

if __name__ == "__main__":
    sys.exit(main() or 0)
