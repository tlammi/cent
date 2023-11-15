project('libcent', 'cpp', default_options: {'cpp_std': 'c++20', 'cpp_eh': 'none', 'cpp_rtti': false})


libcent_inc = include_directories('include')
SRCS = []
subdir('lib')
libcent = library('libcent', SRCS, include_directories: libcent_inc)
libcent_dep = declare_dependency(include_directories: libcent_inc, link_with: libcent)
libcent_srcs = SRCS


TESTS = {}
subdir('test')

foreach k, v : TESTS
  test_name = '@0@_test'.format(k)
  deps = [v.get('deps'), dependency('gtest'), dependency('gtest_main')]
  e = executable(test_name, v['srcs'], dependencies: deps)
  test(test_name, e)
endforeach

