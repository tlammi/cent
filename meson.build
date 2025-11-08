project('cent', 'cpp', default_options: ['cpp_std=c++23'], version: '>=1.3.0')

add_project_arguments('-fno-rtti', '-Wfatal-errors', language: 'cpp')

cmake = import('cmake')


libcent_dep = declare_dependency(include_directories: include_directories('include'))

subdir('test')

