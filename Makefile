.PRECIOUS: %.o
.SUFFIXES:
.PHONY: compile test checkstyle clean

MAIN_EXEC = $(basename $(wildcard *Main.cpp))
TEST_EXEC = $(basename $(wildcard *Test.cpp))
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))

CXX = g++ -Wall -pedantic -std=c++11
HEADERS = $(wildcard *.h)
LIBS = -lncurses

all: compile test checkstyle

compile: $(MAIN_EXEC) $(TEST_EXEC)

debug: CXX += -g
debug: compile

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ -lgtest -lgtest_main -lpthread $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) -c $<

checkstyle:
	python ../cpplint.py --repository=. *.h *.cpp

test: $(TEST_EXEC)
	for test in $^ ; do ./$$test; done

clean:
	rm -f *.o
	rm -f *Main
	rm -f *Test
