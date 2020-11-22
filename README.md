# Symbolibre software

This repository contains calculator applications and related libraries.

## Building the applications

Symbolibre depends on Qt >= 5.12, QCustomPlot and Giac.
Qt 5.12 is available on Ubuntu 19.10 and Debian bullseye.

To build the applications, QTermWidget and KSyntaxHighlighting are also needed.

To install the project, run:
```sh
git clone --recurse-submodules git@gitlab.aliens-lyon.fr:symbolibre/symbolibre-math.git
cd symbolibre-math
mkdir build
cd build
cmake ..
make install
```

This installs:
- the Symbolibre Qt Quick Controls 2 theme
- the Symbolibre C++ library and QML plugins
- the `symbolibre` binary (the applet launcher)
- the calcsheet, funapp and ide applets

You may want to specify a custom installation directory in the `cmake ..`
invocation using the `-D CMAKE_INSTALL_PREFIX=` option.

### Additional CMake options

- If you choose a non-standard installation directory, you may want to add
the `-D SET_RPATH=ON` option so that the installed binaries can locate the
Symbolibre shared library.
- `-D USE_CUSTOM_QML_PATH=OFF` (default ON) installs the QML modules system-wide
instead of in a custom location.
- `-D DEV_BUILD=ON` (default OFF) enables compiler warnings and enables the `SET_RPATH` option.
- `-D ENABLE_DOC=ON` (default OFF) builds and installs HTML documentation (needs Sphinx).
- `-D ENABLE_APPS=OFF` (default ON) disables building the applications.

## Licenses

See LICENSE.md.
