compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'clang'
  executable('fuzz-json-parser', 'parser.cpp', dependencies: libcent_dep, cpp_args: ['-fsanitize=fuzzer'], link_args: ['-fsanitize=fuzzer'])
  executable('fuzz-json-lex', 'lex.cpp', dependencies: libcent_dep, cpp_args: ['-fsanitize=fuzzer'], link_args: ['-fsanitize=fuzzer'])
endif
