SOURCES=$(wildcard ./*.cpp)
TARGETS=$(notdir $(basename $(SOURCES)))
TARGETS_DEBUG=$(patsubst %,%-debug,$(TARGETS))
TARGETS_RELEASE=$(patsubst %,%-release,$(TARGETS))
TARGETS_ALL=$(TARGETS) $(TARGETS_DEBUG) $(TARGETS_RELEASE)

CXX=g++-14
LD=$(CXX)
CPPFLAGS_COMMON=-std=gnu++20 -Wall -Wextra -Wconversion -Wformat=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Wno-unused-parameter -Wbool-operation -Wshadow -fno-omit-frame-pointer -DNDEBUG -D_GLIBCXX_ASSERTIONS
CPPFLAGS_DEBUG=$(CPPFLAGS_COMMON) -O0 -g -isystem .
CPPFLAGS_RELEASE=$(CPPFLAGS_COMMON) -O2 -isystem .
LIBPATH=
LDFLAGS=
LIBS=
CXX_VERSION:=$(shell $(CXX) -dumpversion | cut -d. -f1)

.PHONY: all debug release precompile clean show
.SUFFIXES: .cpp

all: debug release
debug: $(TARGETS_DEBUG)
release: $(TARGETS_RELEASE)

# Compile foo.cpp to foo-debug for all .cpps
$(TARGETS_DEBUG) : %-debug : %.cpp
	$(LD) $(LIBPATH) $(CPPFLAGS_DEBUG) -o $@ $< $(LDFLAGS) $(LIBS)
	@echo "Ready"

# Compile foo.cpp to foo-release for all .cpps
$(TARGETS_RELEASE) : %-release : %.cpp
	$(LD) $(LIBPATH) $(CPPFLAGS_RELEASE) -o $@ $< $(LDFLAGS) $(LIBS)
	@echo "Ready"

## Make stdc++.h.gch
precompile: Makefile
	$(CXX) $(CPPFLAGS_DEBUG) /usr/include/x86_64-linux-gnu/c++/$(CXX_VERSION)/bits/stdc++.h

## Leave stdc++.h.gch
clean:
	rm -f $(TARGETS_ALL)

# Show all variables
show:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
