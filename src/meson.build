SRCS = []
subdir('io')

TESTS = []
FILTERED = []

fs = import('fs')

foreach f : SRCS
  if fs.name(f).startswith('test_')
    TESTS += f
  else
    FILTERED += f
  endif
endforeach


libcent = static_library('cent', FILTERED)
libcent_dep = declare_dependency(link_with: libcent)

test_deps = [dependency('gtest'), dependency('gtest_main'), libcent_dep]

foreach t: TESTS
  n = fs.relative_to(t, meson.current_source_dir())
  parent = fs.parent(n)
  basename = fs.name(n).substring(5)
  n = '@0@/@1@'.format(parent, basename)
  n = fs.replace_suffix(n, '')
  n = n.replace('/', '.')
  n = '@0@.test'.format(n)
  e = executable(n, t, dependencies: test_deps)
  test(n, e)
endforeach
