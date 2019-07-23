.PHONY: clean
all: seq para opt1 opt2

seq: akari.cpp akari.h test_akari.cpp
	g++ -g akari.cpp test_akari.cpp -o akari

para: akari-para.cpp akari.h test_akari.cpp
	g++ -g akari-para.cpp test_akari.cpp -pthread -o akari-para

opt1: akari-para-opt1.cpp akari.h test_akari.cpp
	g++ -g akari-para-opt1.cpp test_akari.cpp -pthread -o akari-opt1

opt2: akari-para-opt2.cpp akari.h test_akari.cpp
	g++ -g akari-para-opt2.cpp test_akari.cpp -pthread -o akari-opt2

clean:
	rm -f akari akari-para akari-opt1 akari-opt2