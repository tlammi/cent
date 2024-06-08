
TESTS = []


TESTS += {'src': 'concepts.cpp', 'depends': libcent_dep}
basedeps = [dependency('gtest'), dependency('gtest_main')]

subdir('cli')
subdir('sd')
subdir('str')
subdir('types')

fs = import('fs')


foreach v : TESTS
  nm = fs.relative_to(v['src'], '.')
  nm = nm.replace('/', '_')
  nm = fs.stem(nm)
  test_nm = '@0@_test'.format(nm)
  deps = basedeps + v['depends']
  incdirs = v.get('incdirs', [])
  e = executable(test_nm, v['src'], dependencies: deps, include_directories: incdirs)
  test(test_nm, e)
endforeach
