This is a minimal example of a really minimal calculator.

The files are structured in this way:
  - Parser.y is a parser implemented with bison. It parses an expression and returns an AST
  - The AST is implemented with classes. They are defined in expression.h.
  - The methods of the classes are implemented in expression.cpp
  - Lexer.l is the lexer, implemented with flex. It is based on the tokens declared in the parser. (Next we will implement our custom lexer).
  
There is a makefile, it wil create the programm calculatrice.
After typing ./calculatrice, the program will wait an input. Type it, then press enter then C-d.
Your can also type echo "input" | ./calculatrice.
If there is a syntax error then the programm will reject the input.

Here is the tutorial I used to create this implementation and learn about classes.
http://web.eecs.utk.edu/~bvz/teaching/cs461Sp11/notes/parse_tree/
I took a lot of inspiration from the files provided here since I never implemented classes before.

Also, if you want some informations on classes, you can look at the tutorial in open classroom.
As there are many examples, it is good to check a specific syntax.

You can run test.sh to try the calculator on several inputs (this is simplist, just to have an idea).
