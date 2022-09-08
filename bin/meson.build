
cli11_proj = subproject('cli11')
cli11_dep = cli11_proj.get_variable('CLI11_dep')

cent = executable('cent', 'main.cpp', dependencies: [libcent_dep, cli11_dep])
