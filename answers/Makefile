SOURCES=$(wildcard ./*.cpp)
TARGETS=$(notdir $(basename $(SOURCES)))

CXX=g++
LD=g++
CPPFLAGS=-std=gnu++17 -Wall -Wextra -O0 -g
LIBPATH=
LDFLAGS=
LIBS=

.PHONY: all clean
.SUFFIXES: .cpp

all: $(TARGETS)

$(TARGETS) : % : %.cpp
	$(LD) $(LIBPATH) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGETS)
