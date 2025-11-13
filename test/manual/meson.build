
tests = files(
  'http_client.cpp',
  'registry_client.cpp',
)

fs = import('fs')

foreach t : tests
  n = fs.relative_to(t, meson.current_source_dir())
  n = fs.replace_suffix(n, '')
  n = n.replace('/', '.')
  n = '@0@.test'.format(n)
  e = executable(n, t, dependencies: libcent_dep)
endforeach
