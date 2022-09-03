gtest_dep = [dependency('gtest'), dependency('gtest_main')]

dep_test = gtest_dep

test_kwargs = {
    'dependencies': dep_test,
    'include_directories': [PUBLIC_INCLUDE, private_inc],
    'link_with': LIBCENT
}

test_image = executable('test_image', 'image.cpp', kwargs: test_kwargs)

test('Image', test_image)
