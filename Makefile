CXX := clang++
FLAGS := -std=c++11 -levent_core -Werror -Weverything -Wno-c++98-compat -Wno-global-constructors -Wno-exit-time-destructors -Wno-old-style-cast -Wno-c++98-compat-pedantic -Wno-padded
CXXFLAGS := -O $(FLAGS)
CXXFLAGSH := -Ofast $(CXXFLAGS)
CXXFLAGSD := -g $(FLAGS)

ALL_SRCS := $(wildcard src/*.cc)
MAIN_SRCS := $(filter-out src/tests.cc, $(ALL_SRCS))
TEST_SRCS := $(filter-out src/main.cc, $(ALL_SRCS))

default:
	$(CXX) $(CXXFLAGS) $(MAIN_SRCS) -o citybash

debug:
	$(CXX) $(CXXFLAGSD) $(MAIN_SRCS) -o citybash

high-performance:
	$(CXX) $(CXXFLAGSH) $(MAIN_SRCS) -o citybash

tests:
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o tests

clean:
	rm -f citybash tests messages
