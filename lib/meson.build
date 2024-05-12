
libcent_incdir = include_directories('include')

libcent_str_dep = declare_dependency(include_directories: libcent_incdir)
libcent_cli_dep = declare_dependency(include_directories: libcent_incdir)
subdir('src')


libcent_dep = declare_dependency(dependencies: [libcent_sd_dep, libcent_str_dep])

