CC=gcc
CFLAGS = -ggdb -Wall -pedantic -std=c11 -pthread -lrt
LDFLAGS=-pthread -lrt
all: threads  process
process: process.o
process.o: process.h
threads: threads.o
threads.o: threads.h
run_threads:
	./threads
run_process:
	./process
clean:
	rm -f *.o process threads core
