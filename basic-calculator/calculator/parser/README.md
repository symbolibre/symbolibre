This is the parser part of the project. Since we compile Parser.cpp and Lexer.cpp with Flex and Bison, 
we cannot use qmake to do this compilation. Thus, I created a little makefile that generate the codes 
for the parser and the lexer. All you need to do is to type make in this repertory before doing qmake.

I implemented a simple function for testing the parser / calculator without going throught the interface.
To compile the test, type in this directory:
make
g++ -c main.cpp -lgiac -lgmp
g++ main.o interface.o Lexer.o Parser.o expression.o -lgiac -lgmp

