SRCS = files('archive.cpp', 'test_archive.cpp', 'crypto.cpp')

DEPS = [
  dependency('libcurl'),
  dependency('boost', modules: ['url']),
  dependency('libarchive'),
  dependency('reflect-cpp', include_type: 'system', fallback: ['reflect-cpp', 'reflectcpp_dep']),
  dependency('magic-enum', include_type: 'system', fallback: ['magic_enum', 'magic_enum_dep']),
  dependency('sqlitecpp', fallback: ['sqlitecpp', 'sqlitecpp_dep']),
  dependency('sqlite3'),
  dependency('openssl'),
  dependency('simdjson'),
]

subdir('io')
subdir('dist')
subdir('strg')
subdir('process')
subdir('os')
subdir('util')

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


add_project_arguments('-include', 'macros.hpp', language: 'cpp')


libcent = static_library('cent', FILTERED, dependencies: DEPS)
libcent_dep = declare_dependency(link_with: libcent, include_directories: include_directories('.'))

bin_srcs = files('main.cpp', 'cli.cpp')
cent = executable('cent', bin_srcs, dependencies: [libcent_dep, dependency('grove', fallback: ['grove', 'grove_dep'])])

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
