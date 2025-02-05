from setuptools import setup, Extension

SRCS = [
    "src/python_module/mazing.c",
    "src/python_module/tile_class/tile_class.c",
    "src/python_module/maze_class/maze_class.c",
    "src/tile/tile.c",
    "src/grid/grid.c",
    "src/grid/data_structures/stack.c",
    "src/grid/data_structures/random_container.c"
]

DIRS = [
    "src/tile",
    "src/grid",
    "src/grid/data_structures",
    "src/python_module/tile_class",
    "src/python_module/maze_class",
]

extension = Extension(
    name='mazing',
    sources=SRCS,
    include_dirs=DIRS,
)


setup(
    ext_modules=[extension],
)
