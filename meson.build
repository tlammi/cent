project('cent', 'cpp', default_options: ['cpp_std=c++20'])


PUBLIC_INCLUDE = include_directories('include')

subdir('lib')

libcent_dep = declare_dependency(include_directories: PUBLIC_INCLUDE, link_with: LIBCENT)

subdir('bin')