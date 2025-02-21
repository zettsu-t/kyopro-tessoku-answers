SOURCES=$(wildcard ./*.cpp)
TARGETS=$(notdir $(basename $(SOURCES)))
STDCPP_GCH=stdc++.h.gch

CXX=g++
LD=g++
CPPFLAGS=-std=gnu++20 -Wall -Wextra -Wconversion -Wformat=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Wno-unused-parameter -Wbool-operation -Wshadow -fno-omit-frame-pointer -DNDEBUG -D_GLIBCXX_ASSERTIONS -O0 -g -I .
# CPPFLAGS=-std=gnu++17 -Wall -Wextra -O2 -I . -DNDEBUG
LIBPATH=
LDFLAGS=
LIBS=

.PHONY: all precompile run clean
.SUFFIXES: .cpp

all: $(TARGETS)

$(TARGETS) : % : %.cpp
	$(LD) $(LIBPATH) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)
	@echo "Ready"

precompile: Makefile
	$(CXX) $(CPPFLAGS) /usr/include/x86_64-linux-gnu/c++/11/bits/stdc++.h

run : $(TARGETS)
	./$(TARGETS)

## leave stdc++.h.gch
clean:
	rm -f $(TARGETS)
