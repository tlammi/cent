
deps = [dependency('gtest'), dependency('gtest_main'), libcent_dep]
test_fs = executable('test_fs', 'fs.cpp', dependencies: deps)

test('File system driver', test_fs, workdir: meson.current_build_dir())
