
private_inc = include_directories('include')

SRCS = []
DEPS = []

nlohmann_proj = cmake.subproject('nlohmann_json')
DEPS += nlohmann_proj.get_variable('nlohmann_json_dep')

# TODO: Flag to select this
cpr_opts = cmake.subproject_options()
cpr_opts.add_cmake_defines({
    'CPR_USE_SYSTEM_CURL': true
})
cpr_proj = cmake.subproject('cpr', options: cpr_opts)
DEPS += cpr_proj.get_variable('cpr_dep')


libarchive_proj = cmake.subproject('libarchive')
DEPS += libarchive_proj.get_variable('archive_dep')

subdir('src')

LIBCENT = library('cent', SRCS, include_directories: [private_inc, PUBLIC_INCLUDE], dependencies: DEPS)

subdir('test')
