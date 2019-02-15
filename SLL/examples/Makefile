#! /usr/bin/make -f

src := $(wildcard *.cpp)
exe := $(src:.cpp=)

cflags := -I..
lflags := -L.. -lsll -lgiac -lgmp

all: $(exe)

%: %.cpp
	g++ $< -o $@ $(cflags) $(lflags)

clean:
	rm -f $(exe)
distclean: clean

.PHONY: all clean distclean
