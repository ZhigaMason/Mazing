# Labyrinth generator

Generate labyrinthes blazingly fast.

## USAGE

### Build

To build this application use make.
```bash
make
```

### CLI

CLI tool is made for presentation purposes, so it does not provide complete interface. Nevertheless you can generate and print maze with following template.
```bash
./genlab [seed] [height] [width] [x-start] [y-start] [x-exit] [y-exit]
```

### Locally built Python module

To test your python build you will need to include compiled module into your PYTHONPATH environment variable.

```bash
export PYTHONPATH="${PYTHONPATH}:${PWD}/build/lib.linux-x86_64-cpython-312/labyrinth_generator.cpython-312-x86_64-linux-gnu.so"
```

## Dependencies

Build:

* clang >= 9.0
* Makefile
* python >= 3.10
    - build
    - setuptools
