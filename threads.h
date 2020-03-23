/*
Alexnder Boyd
CS4540
Assigment 3
*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#ifndef THREADS_H
#define THREADS_H
 extern void * process (void * input);
 extern int getSemaphores(sem_t * screen, sem_t * keyboard,int index);
#endif
