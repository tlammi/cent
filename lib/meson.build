
private_inc = include_directories('include')

SRCS = []
DEPS = []

nlohmann_proj = cmake.subproject('nlohmann_json')
DEPS += nlohmann_proj.get_variable('nlohmann_json_dep')

subdir('src')

LIBCENT = library('cent', SRCS, include_directories: [private_inc, PUBLIC_INCLUDE], dependencies: DEPS)

subdir('test')
