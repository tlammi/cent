
SRCS = []
incdir = include_directories('include')

subdir('src')

libcent_reg = library('libcent_reg', SRCS, include_directories: incdir, dependencies: [libcent_core_dep, libcent_dm_dep])
libcent_reg_dep = declare_dependency(link_with: libcent_reg, include_directories: incdir, dependencies: [libcent_core_dep, libcent_dm_dep,dependency('fmt')])

