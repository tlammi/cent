grove_dep = dependency('grove', fallback: ['grove', 'grove_dep'])


srcs = files('main.cpp', 'cli.cpp')

cent = executable('cent', srcs, dependencies: [libcent_dep, grove_dep])
