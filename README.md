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

This installs the symbolibre Qt Quick Controls 2 theme, the symbolibre library,
the symbolibre QML plugins and the symbolibre-calcsheet and symbolibre-funapp
apps.

You may want to specify a custom installation directory to cmake using its
`-D CMAKE_INSTALL_PREFIX=` option.
