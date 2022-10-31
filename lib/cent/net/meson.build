
SRCS += files('curl_driver.cpp', 'net.cpp', 'http_client.cpp', 'fs_driver.cpp')

cpr_dep = dependency('cpr', required: false)
if not cpr_dep.found()
    opts = cmake.subproject_options()
    opts.add_cmake_defines({'CPR_USE_SYSTEM_CURL': true})
    proj = cmake.subproject('cpr', options: opts)
    dep = proj.get_variable('cpr_dep')
    DEPS += [dep]
else
    DEPS += [cpr_dep]
endif
