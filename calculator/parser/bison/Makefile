CC = g++
Cflags = -Wall

all: Lexer.cpp Parser.cpp
	mv Lexer.cpp ..
	mv Parser.cpp .. 
	mv Parser.h ..

Lexer.cpp: Lexer.l Parser.cpp
	flex -o $@ $<

Parser.cpp: Parser.y
	bison --warnings -y -d -f $< --defines=Parser.h -o $@

.PHONY: clear mrproper

clean:
	rm -f *.o *.c stack.hh ../Parser.cpp ../Parser.h ../Lexer.cpp

mrproper: clean