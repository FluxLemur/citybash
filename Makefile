CXX := clang++
CXXFLAGS := -O -std=c++11 -Werror -Weverything -Wno-c++98-compat -Wno-global-constructors -Wno-exit-time-destructors -Wno-old-style-cast

ALL_SRCS := $(wildcard src/*.cc)
MAIN_SRCS := $(filter-out src/tests.cc, $(ALL_SRCS))
TEST_SRCS := $(filter-out src/main.cc, $(ALL_SRCS))

default:
	$(CXX) $(CXXFLAGS) $(MAIN_SRCS) -o citybash

testing:
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o tests

run: default
	./citybash

tests: testing
	./tests

clean:
	rm -f citybash tests
