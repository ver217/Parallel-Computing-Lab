.PHONY: clean
CFLAGS = -g --std=c99
CXXFLAGS = -g --std=c++11
all: seq para opt1 opt2 opt3 fabseq pthread openmp fabopt

seq: akari/akari.cpp akari/akari.h akari/test_akari.cpp
	g++ $(CXXFLAGS) akari/akari.cpp akari/test_akari.cpp -o bin/akari

para: akari/akari-para.cpp akari/akari.h akari/test_akari.cpp
	g++ $(CXXFLAGS) akari/akari-para.cpp akari/test_akari.cpp -pthread -o bin/akari-para

opt1: akari/akari-para-opt1.cpp akari/akari.h akari/test_akari.cpp
	g++ $(CXXFLAGS) akari/akari-para-opt1.cpp akari/test_akari.cpp -pthread -o bin/akari-opt1

opt2: akari/akari-para-opt2.cpp akari/akari.h akari/test_akari.cpp
	g++ $(CXXFLAGS) akari/akari-para-opt2.cpp akari/test_akari.cpp -pthread -o bin/akari-opt2

opt3: akari/akari-para-opt3.cpp akari/akari.h akari/test_akari.cpp
	g++ $(CXXFLAGS) akari/akari-para-opt3.cpp akari/test_akari.cpp -pthread -o bin/akari-opt3

fabseq: fab/fab_seq.c
	gcc $(CFLAGS) fab/fab_seq.c -o bin/fab

pthread: fab/fab_pthread.c
	gcc $(CFLAGS) fab/fab_pthread.c -lm -lpthread -o bin/fab-pthread

openmp: fab/fab_openmp.c
	gcc $(CFLAGS) fab/fab_openmp.c -lm -o bin/fab-openmp

mpi: fab/fab_mpi.c
	gcc $(CFLAGS) fab/fab_mpi.c -lm -o bin/fab-mpi

cuda: fab/fab_cuda.cu
	nvcc -g fab/fab_cuda.cu -lm -o bin/fab-cuda

fabopt: fab/fab_opt.c
	gcc $(CFLAGS) fab/fab_opt.c -lm -lpthread -o bin/fab-opt

clean:
	rm -f bin/*
