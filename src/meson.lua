SRCS({ "archive.cpp", "cli.cpp", "crypto.cpp", "module.cpp" })
EXECUTABLE({ "main.cpp", name = "cent" })

TEST({ "test_archive.cpp", suite = "unit" })

INCLUDE("dist")
INCLUDE("io")
INCLUDE("os")
INCLUDE("process")
INCLUDE("strg")
INCLUDE("util")
INCLUDE("mods")
