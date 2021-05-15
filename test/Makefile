CPP = clang++
all_programs := $(patsubst %.cpp, bin/%, $(wildcard *.cpp))

all: $(all_programs)

bin/% : %.cpp
	$(CPP) -std=c++20 -Wall -pedantic -I../include  -o $@ $<

clean:
	rm $(all_programs)
