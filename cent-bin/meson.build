project('cent', 'cpp', default_options: {'cpp_std': 'c++20', 'cpp_eh': 'none', 'cpp_rtti': false})

SRCS = []
DEPS = []
subdir('src')

cent_core_proj = subproject('cent-core')
if get_option('build_unity')
  SRCS += cent_core_dep.get_variable('SRCS')
else
  DEPS += cent_core_proj.get_variable('libcent_core_dep')
endif
executable('cent', SRCS, dependencies: DEPS)
