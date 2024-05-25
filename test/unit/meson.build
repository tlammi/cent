
TESTS = []


basedeps = [dependency('gtest'), dependency('gtest_main')]

subdir('cli')
subdir('sd')
subdir('str')

fs = import('fs')


foreach v : TESTS
  nm = fs.name(v['src']).split('.')[0]
  test_nm = '@0@_test'.format(nm)
  deps = basedeps + v['depends']
  incdirs = v.get('incdirs', [])
  e = executable(test_nm, v['src'], dependencies: deps, include_directories: incdirs)
  test(test_nm, e)
endforeach
