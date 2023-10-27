SOURCES=$(wildcard ./*.cpp)
TARGETS=$(notdir $(basename $(SOURCES)))

CXX=g++
LD=g++
CPPFLAGS=-std=gnu++20 -Wall -Wextra -O0 -g -I . -DNDEBUG
# CPPFLAGS=-std=gnu++17 -Wall -Wextra -O2 -I . -DNDEBUG
LIBPATH=
LDFLAGS=
LIBS=

.PHONY: all run clean
.SUFFIXES: .cpp

all: $(TARGETS)

$(TARGETS) : % : %.cpp
	$(LD) $(LIBPATH) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)
	@echo "Ready"

run : $(TARGETS)
	./$(TARGETS)

clean:
	rm -f $(TARGETS)
