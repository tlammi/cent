TESTS = []
TEST_DEPS = {}
subdir('core')
subdir('str')
subdir('containers')
subdir('net')

fs = import('fs')

deps = [
  dependency('gtest'), dependency('gtest_main'), dependency('gmock')
]

foreach t : TESTS
  srcs = t['srcs']
  rel = fs.relative_to(t['srcs'][0], '.')
  rel = rel.split('.')[0]
  nm = '@0@_test'.format(rel.replace('/', '_'))
  e = executable(nm, srcs, dependencies: deps + t['deps'])
  test(nm, e)
endforeach
