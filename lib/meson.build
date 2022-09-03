
private_inc = include_directories('include')

SRCS = []
DEPS = []

subdir('src')

LIBCENT = library('cent', SRCS, include_directories: [private_inc, PUBLIC_INCLUDE], dependencies: DEPS)

subdir('test')
