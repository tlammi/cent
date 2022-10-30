SRCS += files('archive.cpp', 'libarchive_driver.cpp')

dep = dependency('libarchive', required: false)

if not dep.found()
    proj = cmake.subproject('libarchive')
    DEPS += proj.get_variable('libarchive_dep')
else
    DEPS += dep
endif