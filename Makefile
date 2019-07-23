.PHONY: clean
all: seq para

seq: akari.cpp akari.h test_akari.cpp
	g++ -g akari.cpp test_akari.cpp -o akari

para: akari-para.cpp akari.h test_akari.cpp
	g++ -g akari-para.cpp test_akari.cpp -pthread -o akari-para

clean:
	rm -f akari akari-para