project('cent', 'cpp', default_options: ['cpp_std=c++23'], meson_version: '>=1.3.0')

add_project_arguments('-fno-rtti', '-Wfatal-errors', language: 'cpp')

cmake = import('cmake')

incdir = include_directories('include')

subdir('src')
#SRCS = []
#subdir('lib')
#
#libdeps = [
#  dependency('libcurl'),
#  dependency('boost', modules: ['url']),
#  dependency('reflect-cpp', fallback: ['reflect-cpp','reflectcpp_dep'], include_type: 'system'),
#  dependency('magic_enum', fallback: ['magic_enum', 'magic_enum_dep']),
#  dependency('sqlitecpp', fallback: ['sqlitecpp', 'sqlitecpp_dep']),
#  dependency('sqlite3'),
#  dependency('simdjson'),
#  dependency('openssl'),
#]
#
## TODO: Change to library(). Need patching reflect-cpp first
#libcent = static_library('cent', SRCS, include_directories: incdir, dependencies: libdeps)
#
#libcent_dep = declare_dependency(include_directories: incdir, link_with: libcent, dependencies: libdeps)
#
#lib_incdir = include_directories('lib')
#
#subdir('bin')
#
#subdir('test')

