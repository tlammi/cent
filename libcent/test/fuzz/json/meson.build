compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'clang'
  kwargs = {
    'cpp_args': '-fsanitize=fuzzer',
    'link_args': '-fsanitize=fuzzer',
    'dependencies': libcent_dep,
    'override_options': ['optimization=3', 'debug=false', 'lto=true'],
  }
  executable('fuzz-json-parser', 'parser.cpp', kwargs: kwargs)
  executable('fuzz-json-lex', 'lex.cpp', kwargs: kwargs)
  #executable('fuzz-json-parser', 'parser.cpp', dependencies: libcent_dep, cpp_args: ['-fsanitize=fuzzer'], link_args: ['-fsanitize=fuzzer'])
  #executable('fuzz-json-lex', 'lex.cpp', dependencies: libcent_dep, cpp_args: ['-fsanitize=fuzzer'], link_args: ['-fsanitize=fuzzer'])
endif
