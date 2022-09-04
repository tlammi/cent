gtest_dep = [dependency('gtest'), dependency('gtest_main')]

dep_test = gtest_dep

test_kwargs = {
    'dependencies': dep_test,
    'include_directories': [PUBLIC_INCLUDE, private_inc],
    'link_with': LIBCENT
}

test_image = executable('test_image', 'image.cpp', kwargs: test_kwargs)
test_http_header = executable('test_http_header', 'http_header.cpp', kwargs: test_kwargs)
test_enum_arr = executable('test_enum_arr', 'enum_arr.cpp', kwargs: test_kwargs)
test_manifests = executable('test_manifests', 'manifests.cpp', kwargs: test_kwargs)

test('Image', test_image)
test('HTTP Header', test_http_header)
test('Enum Array', test_enum_arr)
test('Manifests', test_manifests)
