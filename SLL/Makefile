#! /usr/bin/make -f

src    := $(wildcard *.cpp)
lexer  := $(wildcard *.l)
parser := $(wildcard *.y)
lib    := libsll.a

cflags := -Wall -Wextra -std=c++17 -I. -D_GNU_SOURCE -O2
dflags  = -MMD -MT $@ -MF $(@:.o=.d) -MP
bflags  = -W -f --defines=$(@:.tab.cpp=.h) --verbose

ifdef NDEBUG
  cflags += -DNDEBUG
endif

src-l  := $(lexer:%.l=build/%.yy.cpp)
src-p  := $(parser:%.y=build/%.tab.cpp)
obj    := $(src:%=build/%.o) $(src-l).o $(src-p).o

# Building

all: $(lib)

$(lib): $(obj)
	ar rcs $@ $^

build/%.cpp.o: %.cpp | build/
	g++ -c $< -o $@ $(cflags) $(dflags)

# Lexer
$(src-l).o: $(src-l) | build/
	g++ -c $< -o $@ $(cflags) $(dflags)
$(src-l): $(lexer) $(src-p)
	flex -s -o $@ $<

# Parser
$(src-p).o: $(src-p) | build/
	g++ -c $< -o $@ $(cflags) $(dflags)
$(src-p): $(parser) | build/
	bison $< -o $@ $(bflags)

%/:
	mkdir -p $@

# Dependency management

.PRECIOUS: build/%.tab.cpp
.PRECIOUS: build/%.yy.cpp

.PRECIOUS: build/%.d

build/%.d: ;

include $(wildcard build/*.d)

# Cleaning

clean:
	rm -rf build
distclean: clean
	rm -f $(lib)

.PRECIOUS: %/
.PHONY: all clean distclean
.SUFFIXES:

