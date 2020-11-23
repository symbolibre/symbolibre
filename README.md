# Symbolibre software

This repository contains calculator applications and related libraries.

## Building the applications

Symbolibre depends on Qt >= 5.12, QCustomPlot and Giac.
Qt 5.12 is available on Ubuntu 19.10 and Debian bullseye.

To build the applications, QTermWidget and KSyntaxHighlighting are also needed.

To install the dependencies on Debian or Ubuntu, run `apt install
build-essential cmake libutf8proc-dev qtbase5-dev qtdeclarative5-dev qtquickcontrols2-5-dev
qttools5-dev-tools qml-module-qtquick2 qml-module-qtquick-window2 qml-module-qtquick-layouts
qml-module-qtquick-controls2 qml-module-qt-labs-folderlistmodel libkf5syntaxhighlighting-dev
libqcustomplot-dev libqtermwidget5-0-dev libfltk1.3-dev libgmp-dev libgsl-dev libmpfi-dev
libntl-dev libgiac-dev xcas python3`.

To install the project, run:
```sh
git clone --recurse-submodules https://github.com/symbolibre/symbolibre.git
cd symbolibre
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
