## Symbolibre apps

Calculator applications, mainly math-related.

## Building

First build SLL.

   % cd SLL
   % make

Then build GraphViewer.

   % cd GraphViewer
   % qmake
   % make

Then you can build the applications proper.

   % cd calculator
   % qmake -recursive
   % make
