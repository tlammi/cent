project('cent', 'cpp', default_options: ['cpp_std=c++23', 'cpp_eh=none', 'cpp_rtti=false'])
add_project_arguments('-Wfatal-errors', language: 'cpp')

incdir = include_directories('src')

TESTS = {}
SRCS = []
subdir('src')


if get_option('build_unity')
  gen_unity = custom_target('gen-unity',
    output: 'unity.cpp',
    input: SRCS,
    command: ['scripts/gen-unity.py', '--out=@OUTPUT@', '@INPUT@'],
    depend_files: SRCS,
  )
  gen_unity_dep = declare_dependency(sources: gen_unity)
  cent = executable('cent', dependencies: gen_unity_dep)
else
  test_deps = [dependency('gtest'), dependency('gtest_main')]
  foreach k, v : TESTS
    test_name = '@0@_test'.format(k)
    test_exec = executable(test_name, v, dependencies: test_deps, include_directories: incdir)
    test(test_name, test_exec)
  endforeach
endif
