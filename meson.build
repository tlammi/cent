project('cent', 'cpp', default_options: ['cpp_std=c++23'], version: '>=1.3.0')

add_project_arguments('-fno-rtti', '-Wfatal-errors', language: 'cpp')

cmake = import('cmake')

incdir = include_directories('include')

SRCS = []
subdir('lib')

libdeps = [
  dependency('libcurl'),
  dependency('boost', modules: ['url']),
]

libcent = library('cent', SRCS, include_directories: incdir, dependencies: libdeps)

libcent_dep = declare_dependency(include_directories: incdir, link_with: libcent, dependencies: libdeps)

subdir('test')

