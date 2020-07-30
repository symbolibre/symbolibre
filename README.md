# Symbolibre math apps

Calculator applications, mainly math-related.

## Building the applications

symbolibre-math depends on Qt >= 5.12, QCustomPlot and Giac.
Qt 5.12 is available on Ubuntu 19.10 and Debian bullseye.

To install the project, run:
```
git clone --recurse-submodules git@gitlab.aliens-lyon.fr:symbolibre/symbolibre-math.git
cd symbolibre-math
mkdir build
cd build
cmake ..
make install
```

This installs:
- the Symbolibre Qt Quick Controls 2 theme
- the Symbolibre library
- the symbolibre QML plugins
- the symbolibre-menu, symbolibre-calcsheet, and symbolibre-funapp applications

You may want to specify a custom installation directory in the `cmake ..`
invocation using the `-D CMAKE_INSTALL_PREFIX=` option.
