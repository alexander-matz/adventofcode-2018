EXES=day8-code day9-code day10-code day11-code
CFLAGS=-std=c99
CXXFLAGS=-std=c++11

all: $(EXES)

clean:
	rm -f $(EXES)
