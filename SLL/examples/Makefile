#! /usr/bin/make -f

src = $(wildcard *.cpp)
exe = $(src:.cpp=)

all: $(exe)

%: %.cpp
	g++ $< -o $@ -I.. -L.. -lsll -lgiac -lgmp

clean:
	rm -f $(exe)
distclean: clean

.PHONY: all clean distclean
