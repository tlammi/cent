project('cent', 'cpp', default_options: ['cpp_std=c++20'])

TESTS = {}

subdir('cent')


gtest_deps = [dependency('gtest'), dependency('gtest_main')]

foreach k, v : TESTS
  test_files = v['files']
  extra_deps = v['deps']
  foreach t : test_files
    tname = '@0@'.format(t)
    tname = tname.split('/')[-1]
    tname = tname.split('.')[0]
    tname = '@0@_@1@'.format(k, tname)
    test(tname, executable(tname, t, dependencies: gtest_deps + extra_deps))
  endforeach
endforeach

#add_project_arguments('-fno-rtti', '-Wfatal-errors', language: 'cpp')
#
#cmake = import('cmake')
#
#PUBLIC_INCLUDE = include_directories('include')
#
#subdir('lib')
#
#libcent_dep = declare_dependency(include_directories: PUBLIC_INCLUDE, link_with: LIBCENT)
#
#subdir('bin')
