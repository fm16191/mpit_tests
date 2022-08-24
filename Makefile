CC=mpicc
CFLAGS=-Wall -Wextra -g3
INCLUDEFLAGS=-I include
CTFLAGS=-Wall -Wextra -g3 -Wno-unused-parameter # Compiler flags for tests only.
OFLAGS=-march=native -mtune=native -O2

SRC=src
EXT=ext
DEPS=build
BINS=tests
_LIBS=
LIBS=$(foreach l,$(_LIBS),-l$l)
CTLIBS=
TESTS=list_cvars list_pvars list_events

.PHONY: all test clean

all: test

test: $(TESTS)

%: $(SRC)/%.c
	$(CC) $(CTFLAGS) $(OFLAGS) $(INCLUDEFLAGS) $^ -o $@ $(CTLIBS)

clean:
	rm -Rf $(TARGET) $(EXE) $(TESTS)
