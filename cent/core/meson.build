
SRCS = []

incdir = include_directories('include')
subdir('src')

libcent_core = library('cent-core', SRCS, include_directories: incdir)

libcent_core_dep = declare_dependency(link_with: libcent_core, include_directories: incdir)

subdir('test')

