.PHONY: clean
all: akari.cpp akari.h test_akari.cpp
	g++ -g -Wall akari.cpp akari.h test_akari.cpp -o akari

clean:
	rm -f akari