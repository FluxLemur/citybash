CXX := g++
CXXFLAGS := -O -std=c++11 -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
# for more info on flags: http://stackoverflow.com/questions/3375697/useful-gcc-flags-for-c

ALL_SRCS := $(wildcard src/*.cc)

default:
	$(CXX) $(ALL_SRCS) -o citybash

run: default
	./citybash

clean:
	rm citybash
