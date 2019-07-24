.PHONY: clean
all: seq para opt1 opt2 fabseq pthread openmp

seq: akari/akari.cpp akari/akari.h akari/test_akari.cpp
	g++ -g akari/akari.cpp akari/test_akari.cpp -o bin/akari

para: akari/akari-para.cpp akari/akari.h akari/test_akari.cpp
	g++ -g akari/akari-para.cpp akari/test_akari.cpp -pthread -o bin/akari-para

opt1: akari/akari-para-opt1.cpp akari/akari.h akari/test_akari.cpp
	g++ -g akari/akari-para-opt1.cpp akari/test_akari.cpp -pthread -o bin/akari-opt1

opt2: akari/akari-para-opt2.cpp akari/akari.h akari/test_akari.cpp
	g++ -g akari/akari-para-opt2.cpp akari/test_akari.cpp -pthread -o bin/akari-opt2

fabseq: fab/fab_seq.c
	gcc -g fab/fab_seq.c -o bin/fab

pthread: fab/fab_pthread.c
	gcc -g fab/fab_pthread.c -lm -lpthread -o bin/fab-pthread

openmp: fab/fab_openmp.c
	gcc -g fab/fab_openmp.c -lm -o bin/fab-openmp

cuda: fab/fab_cuda.cu
	nvcc -g fab/fab_cuda.cu -lm -o bin/fab-cuda

clean:
	rm -f bin/*