

srcs = files('json.cpp')

libcent_sd_inc_priv = include_directories('.')
libcent_sd = library('cent-sd', srcs, include_directories: libcent_incdir)
libcent_sd_dep = declare_dependency(include_directories: libcent_incdir, link_with: libcent_sd)

