manual_tests = files(
  'net_client.cpp'
)

fs = import('fs')

foreach t : manual_tests
  nm = fs.stem(t)
  e = executable(nm, t, dependencies: libcent_dep)
endforeach
