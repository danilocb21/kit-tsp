sources = files(
  'main.cpp',
  'data.cpp',
  'solution.cpp',
  'MLP.cpp',
  'GILS.cpp',
)

incdir = include_directories('../include')

executable(
  meson.project_name(), 
  sources,
  include_directories: [incdir, incdir_deps],
  dependencies: [dependencies, external_dependencies],
  install: true
)

