
SRCS = []
incdir = include_directories('include')

libcent_reg = library('libcent_reg', SRCS, include_directories: incdir)
libcent_reg_dep = declare_dependency(link_with: libcent_reg, include_directories: incdir, dependencies: libcent_core_dep)

