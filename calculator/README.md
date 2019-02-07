# Calculation Sheet

  Here is the code of edition tree, a first try for communications with SageMaths,
and for the calculation sheet.

### Dependencies

  To compile the code, you will need

* `g++` 
* `qmake` v3.0 or above,
* a fresh version of `QtQuick`, 
at least 2.6 (*to verify*) and if possible above 2.8. It is recommended
to install `QtCreator` if you aim at working on the code.
* `bison` v3.0 or above.

### How do I compile ?

* Go in directory `parser/bison` and run `make`.
* Go back in the root directory, and run `qmake -recursive` to built `Makefile`s
from `.pro` files. Then, run `make`.

