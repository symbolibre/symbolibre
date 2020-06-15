## Symbolibre apps

Calculator applications, mainly math-related.

## Building the applications

```
qmake -recursive
make
```

Out-of-tree builds are not supported for now.

## Running the applications

```
cd apps/calcsheet
LD_LIBRARY_PATH=../../lib ./calcsheet
```
