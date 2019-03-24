# Calculation Sheet

This folder contains the implementation of edition trees, the interactive
computation sheet, and the function plot application.

### Dependencies

  To compile the code, you will need

* `g++` 
* `qmake` v3.0 or above,
* a fresh version of `QtQuick`, 
at least 2.6 (*to verify*) and if possible above 2.8. It is recommended
to install `QtCreator` if you aim at working on the code.
* `bison` v3.0 or above.

Also QCustomPlot either in the system (uncomment the -qcustomplot in the .pro
files) or copied into the GraphViewer folder in the parent directory.

### How do I compile ?

    % qmake -recursive
    % make

