CPP = clang++-10
all_programs := $(patsubst %.cpp, bin/%, $(wildcard *.cpp))


all: $(all_programs)

bin/.dirstamp:
	mkdir -p bin
	touch $@

bin/% : %.cpp bin/.dirstamp
	$(CPP) -std=c++20 -Wall -pedantic -I../include  -o $@ $<

clean:
	rm -f $(all_programs) || true
