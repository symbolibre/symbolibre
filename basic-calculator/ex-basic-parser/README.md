This is a minimal example of a really minimal calculator.

The files are structured in this way:
  - Parser.y is a parser implemented with bison. It parses an expression and returns an AST
  - The AST is implemented with classes. They are defined in expression.h.
  - The methods of the classes are implemented in expression.cpp
  - Lexer.l is the lexer, implemented with flex. It is based on the tokens declared in the parser
  
There is a functioning makefile atrocious, but functioning), it wil create the fabulous programm calculatrice (the project is over).
After typing ./calculatrice, the program will wait an input. Type it, then press enter then C-d.
Please don't write mistakes if you don't want memory leackage (I told you!). 

Here is the tutorial I used to create this implementation and learn about classes.
http://web.eecs.utk.edu/~bvz/teaching/cs461Sp11/notes/parse_tree/
I took a lot of inspiration from the files provided here since I never implemented classes before.

Also, if you want some informations on classes, you can look at the tutorial in open classroom.
As there are many examples, it is good to check a specific syntax.

ADD : you can run test.sh to try the calculator on several inputs (this is simplist, just to have an idea).
