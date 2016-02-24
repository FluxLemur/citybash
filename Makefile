CXX := g++
CXXFLAGS := -O -std=c++11 -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
# for more info on flags: http://stackoverflow.com/questions/3375697/useful-gcc-flags-for-c

ALL_SRCS := $(wildcard src/*.cc)
MAIN_SRCS := $(filter-out src/tests.cc, $(ALL_SRCS))
TEST_SRCS := $(filter-out src/main.cc, $(ALL_SRCS))

default:
	$(CXX) $(MAIN_SRCS) -o citybash

testing:
	$(CXX) $(TEST_SRCS) -o tests

run: default
	./citybash

tests: testing
	./tests

clean:
	rm -f citybash tests
