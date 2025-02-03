# Mazing

Python module in C to generate labyrinthes blazingly fast.

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

Makefile will install python module into python venv, if none was found it will try to install it system-wide. In this case, to test your python build you will need to include compiled module into your PYTHONPATH environment variable.

```bash
export PYTHONPATH="${PWD}/build/lib.<os>-<arc>-cpython-<pyversion>:${PYTHONPATH}"
```

## Dependencies

Build:

* clang
* Makefile
* python >= 3.10
    - build
    - setuptools
