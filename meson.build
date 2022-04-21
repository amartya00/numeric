project(
  'numeric.sigabrt.io',
  'cpp',
  version : '0.0.2',
  license : 'GPLV2',
  default_options:[
    'warning_level=3'
  ])
add_global_arguments('--std=c++17', '-Werror', language : 'cpp')

#Dependencies
catch = dependency('catch2')
soup = dependency('thesoup')

# Sources
inc = include_directories('headers')
subdir('headers')
subdir('tst')

# Set up pkgconfig
pkg = import('pkgconfig')
pkg.generate(
    subdirs : 'headers/numeric',
    version : '0.0.2',
    name : 'numeric',
    filebase : 'numeric',
    description : 'A numeric library'
)


