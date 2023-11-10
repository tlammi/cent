project('libcent', 'cpp', default_options: {'cpp_std': 'c++20', 'cpp_eh': 'none', 'cpp_rtti': false})


pub_incdir = include_directories('include')
libcent_dep = declare_dependency(include_directories: pub_incdir)

TESTS = {}

subdir('test')

foreach k, v : TESTS
  test_name = '@0@_test'.format(k)
  deps = [v.get('deps'), dependency('gtest'), dependency('gtest_main')]
  e = executable(test_name, v['srcs'], dependencies: deps)
  test(test_name, e)
endforeach

