EXES=day8-code day9-code day10-code day11-code day12-code day13-code \
     day14-code day15-code day16 day17 day18 day19
CFLAGS+=-std=c99 -g
CXXFLAGS+=-std=c++11

all: $(EXES)

clean:
	rm -f $(EXES)
