gen_errno_script = meson.project_source_root() + '/script/gen_errno.py'
GEN_ERRNO = custom_target('gen-errno', output: ['errno.gen.inl'], command: [gen_errno_script, '@OUTPUT@'])
