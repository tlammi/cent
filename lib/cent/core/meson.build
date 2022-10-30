SRCS += files('cent.cpp', 'storage.cpp', 'workspace.cpp', 'context.cpp')


#  ['nlohmann_json', 'cmake', 'nlohmann_json', 'nlohmann_json_dep', {}],
nlohmann_json_dep = dependency('nlohmann_json', required: false)

if not nlohmann_json_dep.found()
    proj = cmake.subproject('nlohmann_json')
    DEPS += proj.get_variable('nlohmann_json_dep')
else
    DEPS += nlohmann_json_dep
endif

subdir('storage')