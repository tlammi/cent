
incdir = include_directories('include')


libcent_str_dep = declare_dependency(include_directories: incdir)

magic_enum_dep = dependency(
  'magic_enum',
  fallback: ['magic_enum', 'magic_enum_dep'],
  default_options: {
    'test': false
  },
)

libcent_util_dep = declare_dependency(include_directories: incdir)

libcent_containers_dep = declare_dependency(
  include_directories: incdir,
  dependencies: [
    magic_enum_dep,
    libcent_util_dep,
  ],
)

subdir('test')
