project('cent-core', 'cpp', default_options: {'cpp_std': 'c++20', 'cpp_eh': 'none', 'cpp_rtti': 'false'})

SRCS = []
TESTS = {}

pub_include = include_directories('include')
deps = [dependency('boost'), dependency('fmt')]

subdir('src')

libcent_core = library('cent-core', SRCS, dependencies: deps, include_directories: pub_include)
libcent_core_dep = declare_dependency(link_with: libcent_core, dependencies: deps, include_directories: pub_include)

TESTS = {}
subdir('test')

test_deps = [dependency('gtest'), dependency('gtest_main'), libcent_core_dep]
foreach k, v : TESTS

  test_name = '@0@_test'.format(k)
  e = executable(test_name, v, dependencies: test_deps)
  test(test_name, e)
endforeach
