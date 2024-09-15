project(
  'cent',
  'cpp',
  default_options: {
    'cpp_std': 'c++23',
    'cpp_rtti': 'false'
})

magic_enum_dep = dependency(
  'magic_enum',
  fallback: ['magic_enum', 'magic_enum_dep'],
  default_options: {
    'test': false
  },
)

subdir('lib')

