#!/usr/bin/env python3

import sys
import os
import hashlib

import util

SPDX_HEADER_FIELD = "SPDX-License-Identifier"
SPDX_HEADER = f"/* {SPDX_HEADER_FIELD}:  GPL-3.0-or-later */".strip()

EXPECTED_LICENSE_DIGEST = "3972dc9744f6499f0f9b2dbf76696f2ae7ad8af9b23dde66d6af86c9dfb36986"

def license_file_digest():
    thisdir = os.path.dirname(sys.argv[0])
    lic_file = os.path.join(thisdir, "../LICENSE")
    with open(lic_file, "rb") as f:
        digest = hashlib.sha256(f.read(), usedforsecurity=False)
    return digest.hexdigest()


def digest_failure_msg(got: str):
    return f"""
LICENSE file digest changed. Check that SPDX identifier is up to date.
Cached digest: '{EXPECTED_LICENSE_DIGEST}'.
Got: '{got}'.
""".strip()


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
            if SPDX_HEADER_FIELD not in line:
                # No SPDX header ->  Prepend
                lines = [f"{SPDX_HEADER}\n"] + lines
            else:
                # SPDX header present -> Replace
                lines = [f"{SPDX_HEADER}\n"] + lines[1:]
            with open(s, "w") as f:
                f.writelines(lines)


def main():

    try:
        if len(sys.argv) != 2:
            raise ValueError(
                    f"Usage: {sys.argv[0]} 'commmand' where command is 'check' or 'inject'")
        srcs = util.src_files()

        digest = license_file_digest()
        if digest != EXPECTED_LICENSE_DIGEST:
            raise ValueError(digest_failure_msg(digest))

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

