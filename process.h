/*
Alexnder Boyd
CS4540
Assigment 3
*/
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


#ifndef PROCESS_H
#define PROCESS_H
extern void process(sem_t * screen,sem_t *keyboard,int index);
extern int getSemaphores(sem_t * screen, sem_t * keyboard,int index);
#endif
