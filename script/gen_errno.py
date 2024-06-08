#!/usr/bin/env python3

import sys

from subprocess import run, PIPE
from typing import Iterable
from dataclasses import dataclass

@dataclass
class Errno:
    name: str
    number: int
    desc: str

    def enum(self):
        nm = self.name[1:]
        if nm[0] == '2':
            nm = "too" + nm[1:]
        return nm.capitalize()


def parse_errnos() -> Iterable[Errno]:
    res = run(["errno", "--list"], stdout=PIPE, check=True)
    lines = res.stdout.decode("utf-8").splitlines()
    for line in lines:
        name, number, *rest = line.split()
        desc = " ".join(rest)
        yield Errno(name, int(number), desc)

def gen_errno(errnos: Iterable[Errno]):
    yield "enum class Errno {"
    for err in errnos:
        nm = err.enum()
        yield f"    {nm} = {err.number},"
    yield "};"

def gen_desc(errnos: Iterable[Errno]):
    yield "constexpr str::StaticStr errno_to_desc(Errno e) noexcept {"
    yield "    switch(e){"
    processed = set()
    for e in errnos:
        if e.number in processed:
            continue
        yield f"    case Errno::{e.enum()}: return \"{e.desc}\";"
        processed.add(e.number)
    yield "    }"
    yield "    return \"Unknown error\";"
    yield "}"

def gen_from_enum(errnos: Iterable[Errno]):
    yield "constexpr Errno errno_from_int(int e){ return Errno(e); }"
    yield "Errno errno_from"

_STDOUT = sys.stdout
def _print(*args, **kwargs):
    print(*args, file=_STDOUT, **kwargs)

def main():
    if len(sys.argv) > 1:
        global _STDOUT
        _STDOUT = open(sys.argv[1], "w")
    errnos = list(parse_errnos())
    for line in gen_errno(errnos):
        _print(line)
    for line in gen_desc(errnos):
        _print(line)

if __name__ == "__main__":
    sys.exit(main() or 0)

