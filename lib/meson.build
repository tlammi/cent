SRCS = []
DEPS = []

private_inc = include_directories('.')
subdir('cent')

LIBCENT = library('cent', SRCS, include_directories: [private_inc, PUBLIC_INCLUDE], dependencies: DEPS)
#private_inc = include_directories('include')
#
#SRCS = []
#DEPS = []
#
#DEP_ARGS = [
#  ['nlohmann_json', 'cmake', 'nlohmann_json', 'nlohmann_json_dep', {}],
#  ['libarchive', 'cmake', 'libarchive', 'archive_dep', {}],
#  ['cpr', 'cmake', 'cpr', 'cpr_dep', {'CPR_USE_SYSTEM_CURL': true}]
#]
#
#foreach arg: DEP_ARGS
#  dep = dependency(arg[0], required: false)
#  if not dep.found()
#    if arg[1] == 'cmake'
#      opts = cmake.subproject_options()
#      opts.add_cmake_defines(arg[4])
#      proj = cmake.subproject(arg[2], options: opts)
#      dep = proj.get_variable(arg[3])
#    endif
#  endif
#  DEPS += [dep]
#endforeach
#
#
#subdir('src')
#
#LIBCENT = library('cent', SRCS, include_directories: [private_inc, PUBLIC_INCLUDE], dependencies: DEPS)
#
#subdir('test')
#