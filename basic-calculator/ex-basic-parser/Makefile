CC = g++
Cflags = -Wall
EXEC = calculatrice

all: $(EXEC)

$(EXEC): Parser.o Lexer.o expression.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $^

Lexer.cpp: Lexer.l Parser.cpp
	flex -o $@ $<

Parser.cpp: Parser.y expression.o
	bison --warnings -y -d -f $< --defines=Parser.h -o $@

.PHONY: clean mrproper

clean:
	rm -rf *.o *.c Parser.cpp Lexer.cpp Parser.h stack.hh

mrproper: clean
	rm -rf $(EXEC)

	
