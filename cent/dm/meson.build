
SRCS = []
incdir = include_directories('include')

subdir('src')


deps = [libcent_core_dep, dependency('nlohmann_json')]

libcent_dm = library('cent_dm', SRCS, dependencies: deps, include_directories: incdir)

libcent_dm_dep = declare_dependency(link_with: libcent_dm, include_directories: incdir, dependencies: deps)

subdir('test')
