# SymboLibre Language (SLL)

SLL is the interface of all the math of the system. Applications that wish to
perform math and symbolic calculation should ask SLL, not the underlying CAS.

SLL currently interfaces with Giac.

## Building the library and examples

To build the library, type `make`. Specifying `NDEBUG=1` will turn on parser
tracing which outputs data to stderr.

`make` in the `examples` folder to build the examples.
