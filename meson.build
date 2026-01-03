project('cent', 'cpp', default_options: ['cpp_std=c++23'], meson_version: '>=1.3.0')

cmake = import('cmake')
fs = import('fs')

meson_lua = find_program(meson.current_source_dir() / 'meson.lua')
lua_files = run_command([meson_lua, 'config-files'], check:true).stdout().split()
configure_file(command: [meson_lua, 'sources'], capture: true, input: lua_files, output: 'sources.txt')
configure_file(command: [meson_lua, 'executables'], capture: true, input: lua_files, output: 'excutables.txt')
configure_file(command: [meson_lua, 'tests'], capture: true, input: lua_files, output: 'tests.txt')

srcs = files(run_command([meson_lua, 'sources'], check: true).stdout().strip().split(' '))
lines = run_command([meson_lua, 'executables'], check: true).stdout().strip().split('\n')
execs = []
foreach l : lines
  words = l.split(' ')
  execs += [[words[0], files(words[1])]]
endforeach

lines = run_command([meson_lua, 'tests'], check: true).stdout().strip().split('\n')
tests = []
foreach l : lines
  words = l.split(' ')
  tests += {'suite': words[0], 'file': files(words[1])}
endforeach

add_project_arguments('-fno-rtti', '-Wfatal-errors', language: 'cpp')

deps = [
  dependency('libcurl'),
  dependency('boost', modules: ['url']),
  dependency('libarchive'),
  dependency('reflect-cpp', include_type: 'system', fallback: ['reflect-cpp', 'reflectcpp_dep']),
  dependency('magic-enum', include_type: 'system', fallback: ['magic_enum', 'magic_enum_dep']),
  dependency('sqlitecpp', fallback: ['sqlitecpp', 'sqlitecpp_dep']),
  dependency('sqlite3'),
  dependency('openssl'),
  dependency('simdjson'),
  dependency('grove', fallback: ['grove', 'grove_dep']),
]

srcdir = include_directories('src')

libcent = static_library('cent', srcs, include_directories: srcdir, dependencies: deps)
libcent_dep = declare_dependency(link_whole: libcent, include_directories: srcdir)

foreach e : execs
  executable(e[0], e[1], dependencies: deps + libcent_dep)
endforeach


gtest_dep = dependency('gtest')
gtest_main_dep = dependency('gtest_main')

test_kwargs = {
  'unit': {
    'dependencies': [libcent_dep, gtest_dep, gtest_main_dep],
    'cpp_args': ['-include', 'gtest/gtest.h', '-include', 'gmock/gmock.h']
  }
}

test_deps = [libcent_dep, dependency('gtest'), dependency('gtest_main')]

foreach t: tests
  p = files(fs.relative_to(t['file'], meson.current_source_dir()))
  nm = fs.name(p).split('.')[0]
  nm = '@0@.test'.format(nm)
  test(nm, executable(nm, p, kwargs: test_kwargs[t['suite']]), suite:t['suite'])
endforeach

#subdir('src')
#subdir('test')
