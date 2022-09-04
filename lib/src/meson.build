
SRCS = SRCS + files('cent.cpp', 'http_client.cpp', 'logs.cpp', 'image.cpp', 'registry_client.cpp')



# TODO: Flag to select this
curlpp_proj = cmake.subproject('curlpp')
DEPS = DEPS + [curlpp_proj.get_variable('curlpp_dep')]
SRCS = SRCS + files('curl_http_session.cpp')