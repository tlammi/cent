#!/usr/bin/env python3

import errno
import os
import sys


def create_entry_list():
    entries = []
    for val, nm in errno.errorcode.items():
        entries.append((nm.title(), int(val), os.strerror(val)))
    entries.append(("Ok", 0, "OK"))
    entries.sort(key=lambda x: x[1])
    return entries

def main():
    if len(sys.argv) == 1:
        f = open("/dev/stdout", "w")
    elif len(sys.argv) == 2:
        f = open(sys.argv[1], "a")
    entries = create_entry_list()
    enum_class = ["enum class Errno {"]
    for e in entries:
        enum_class.append(f"    {e[0]} = {e[1]},")
    enum_class.append("};")

    msg_func = ["constexpr StaticStr errno_message(Errno e) noexcept {", "    switch(e){"]
    for e in entries:
        msg_func.append(f'        case Errno::{e[0]}: return "{e[2]}";')
    msg_func.append('        default: return "Unknown error";')
    msg_func.append("    }")
    msg_func.append("}")

    from_errno_func = "constexpr Errno from_errno(int e) noexcept { return static_cast<Errno>(e); }"
    from_errno_func_2 = "inline Errno from_errno() noexcept {return from_errno(errno);}"

    print("#pragma once", file=f)
    print("// GENERATED FILE. DO NOT EDIT!", file=f)
    print("#include <cent/str/static_str.hpp>", file=f)
    print("namespace cent {", file=f)
    print("\n".join(enum_class), file=f)
    print("", file=f)
    print("\n".join(msg_func), file=f)
    print("", file=f)
    print(from_errno_func, file=f)
    print(from_errno_func_2, file=f)
    print("}", file=f)

if __name__ == '__main__':
    main()
