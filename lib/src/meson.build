
SRCS = SRCS + files('cent.cpp', 'http_client.cpp', 'logs.cpp')



curlpp_proj = cmake.subproject('curlpp')
DEPS = DEPS + [curlpp_proj.get_variable('curlpp_dep')]
# TODO: Flag to select this
SRCS = SRCS + files('curl_http_session.cpp')