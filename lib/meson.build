
cmake = import('cmake')

magic_enum_dep = dependency(
  'magic_enum',
  fallback: ['magic_enum', 'magic_enum_dep'],
  default_options: {
    'test': false
  },
)

reflectcpp_proj = cmake.subproject('reflect-cpp')
reflectcpp_dep = reflectcpp_proj.get_variable('reflectcpp_dep')

incdir = include_directories('include')


SRCS = []

subdir('src')

libcent_str_dep = declare_dependency(include_directories: incdir)



libcent_util_dep = declare_dependency(include_directories: incdir)

libcent_containers_dep = declare_dependency(
  include_directories: incdir,
  dependencies: [
    magic_enum_dep,
    libcent_util_dep,
  ],
)

curl_dep = dependency('libcurl')

libcent = library('cent', SRCS, dependencies: [curl_dep], include_directories: incdir)
libcent_dep = declare_dependency(
  link_with: libcent,
  dependencies: [
    libcent_str_dep,
    libcent_util_dep,
    libcent_containers_dep,
  ]
)

subdir('test')
