compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'clang'
  executable('json-parser-fuzz', 'main.cpp', dependencies: libcent_dep, cpp_args: ['-fsanitize=fuzzer'], link_args: ['-fsanitize=fuzzer'])
endif
