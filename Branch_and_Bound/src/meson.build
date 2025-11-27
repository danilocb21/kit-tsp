sources = files(
  'main.cpp',
  'data.cpp',
  'hungarian.cpp',
  'BnB.cpp',
  'solve.cpp',
  'construction.cpp',
  'node.cpp',
)

incdir = include_directories('../include')

executable(
  meson.project_name(), 
  sources,
  include_directories: [incdir, incdir_deps],
  dependencies: [dependencies, external_dependencies],
  install: true
)

