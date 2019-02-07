#! /usr/bin/make -f

src = $(wildcard *.cpp)
obj = $(src:%=build/%.o)
lib = libsll.a

cflags = -Wall -Wextra -I.

all: $(lib)

$(lib): $(obj)
	ar rcs $@ $^

build/%.o: % | build/
	g++ -c $< -o $@ $(cflags)

%/:
	mkdir -p $@

clean:
	rm -rf build
distclean: clean
	rm -f $(lib)

.PRECIOUS: %/
.PHONY: all clean distclean
