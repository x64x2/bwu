project('ray', 'c',
  default_options : [
    'c_std=c99',
    'optimization=3',
    'debug=false'])
add_project_arguments(
  '-Wno-unused-parameter',
  '-Wno-unused-function',
  language : 'c')

cc = meson.get_compiler('c')
# cc = 'clang'
math_dep = cc.find_library('m', required : false)


incdir = include_directories('include')
src = [
  'src/ray.c',
  'src/graph.c',
  'src/play.c',
  'src/front.c',
  'src/util.c',
  'src/bit.c',
  'src/map.c'
  # 'src/main.c'
]

shared_library('ray', src,
  include_directories : incdir,
  # win_subsystem: 'windows',)
