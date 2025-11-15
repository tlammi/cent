
gtest_dep = [dependency('gtest'), dependency('gtest_main')]
test_deps = gtest_dep + libcent_dep

TESTS = files(
  #'exceptions.cpp',
)

subdir('util')
subdir('data')
subdir('dist')

fs = import('fs')

foreach t: TESTS
  n = fs.relative_to(t, meson.current_source_dir())
  n = fs.replace_suffix(n, '')
  n = n.replace('/', '.')
  n = '@0@.test'.format(n)
  e = executable(n, t, dependencies: test_deps)
  test(n, e)
endforeach

