
import os
import sys
import glob

SRC_DIRS = [
    "../bin",
    "../lib",
    "../include",
]

def src_files():
    thisdir = os.path.dirname(sys.argv[0])

    src_dirs = [os.path.join(thisdir, d) for d in SRC_DIRS]

    srcs = []
    for d in src_dirs:
        raw_srcs = glob.glob("./**/*.cpp", root_dir=d, recursive=True) \
                + glob.glob("./**/*.hpp", root_dir=d, recursive=True)
        srcs.extend([os.path.join(d, s) for s in raw_srcs])

    return [os.path.normpath(s) for s in srcs]
