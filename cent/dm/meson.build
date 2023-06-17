
SRCS = []
incdir = include_directories('include')

subdir('src')

libcent_dm = library('cent_dm', SRCS, dependencies: libcent_core_dep, include_directories: incdir)

libcent_dm_dep = declare_dependency(link_with: libcent_dm, include_directories: incdir, dependencies: libcent_core_dep)

subdir('test')
