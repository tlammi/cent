
libcent_incdir = include_directories('include')
subdir('include')
libcent_str_dep = declare_dependency(include_directories: libcent_incdir)
libcent_cli_dep = declare_dependency(include_directories: libcent_incdir)
libcent_types = library('cent-types', [GEN_ERRNO])
libcent_types_dep = declare_dependency(include_directories: libcent_incdir, link_with: libcent_types)
subdir('src')


deps = [libcent_sd_dep, libcent_str_dep, libcent_cli_dep]
libcent_dep = declare_dependency(dependencies: deps)

