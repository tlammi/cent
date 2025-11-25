
gtest_dep = [dependency('gtest'), dependency('gtest_main')]
test_deps = gtest_dep + libcent_dep

TESTS = files(
  'name.cpp',
)

subdir('util')
subdir('data')
subdir('dist')
subdir('strg')
subdir('core')

fs = import('fs')

foreach t: TESTS
  n = fs.relative_to(t, meson.current_source_dir())
  n = fs.replace_suffix(n, '')
  n = n.replace('/', '.')
  n = '@0@.test'.format(n)
  e = executable(n, t, dependencies: test_deps, include_directories: lib_incdir)
  test(n, e)
endforeach

