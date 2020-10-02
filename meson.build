project(
  'numeric.sigabrt.io',
  'cpp',
  version : '0.0.1',
  license : 'GPLV2',
  default_options:[
    'warning_level=3'
  ])

add_global_arguments('--std=c++17', '-Werror', language : 'cpp')

inc = include_directories('headers')

subdir('headers')
subdir('src')
subdir('tst')


