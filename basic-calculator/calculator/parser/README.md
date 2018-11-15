This is the parser part of the project. Since we compile Parser.cpp and Lexer.cpp with Flex and Bison, we cannot use qmake to do this compilation.
Thus, I created a little makefile that generate the codes for the parser and the lexer. All you need to do is to type make in this repertory before doing qmake.
