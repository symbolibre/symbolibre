## Symbolibre apps

Calculator applications, mainly math-related.

## Building

First build SLL in the `SLL` directory.

   % make -C SLL

Then build GraphViewer.

   % cd GraphViewer
   % qmake
   % make

Then you can build the applications proper.

   % cd calculator
   % qmake -recursive
   % make
