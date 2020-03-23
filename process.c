/*
   Alexnder Boyd
   CS4540
   Assigment 3
 */
#include "process.h"


int main(int argc, char const *argv[]) {
	//sem_open both semaphores
	int num_proc=7;
	int error;
	int open_failed=0;
	//fork 7 processes
	//Each process passed index of loop so 1,2,3, or 4
	sem_t * sem_screen, * sem_keyboard;
	// declare two semephores
	sem_screen = sem_open("/screen",O_CREAT,0777,1);
	// open screen semphore
	sem_keyboard= sem_open("/keyboard",O_CREAT,0777,1);
	// open keyboard semephore
	if(sem_keyboard==SEM_FAILED) {
		perror("sem_open keyboard");
		open_failed=1;
	}
	if (sem_screen==SEM_FAILED) {
		perror("sem_open");
		open_failed=1;
	}
	int pids[num_proc];
	int pid;
	int i = 0;
	// if open of both sems is sucessfull then continue
	if(open_failed!=1) {
		do {
			if( (pid = fork()) == 0)
				process(sem_screen, sem_keyboard, i+1);
			else {

				pids[i] = pid;
				i++;
			}
		} while( i < num_proc && pid > 0);
		if(i >= num_proc) {
			// stop forking around
			//   after all die, use single call or loop based on ids saved above
			// sem_unlink both
			for(i=0; i<num_proc; i++) {
				waitpid(pids[i],&error, 0);
				// wait for process to end till close
			}
		}
	}
	if(pid>0) {
		if(sem_unlink("/screen")!=0) {                                                                                                                                                                                                                                                                  // close screen sem
			perror("sem_unlink");
		}
		if (sem_unlink("/keyboard")) {                                                                                                                                                                                                                                                                           //     close keyboard
			perror("sem_unlink");
		}
	}
	return 0;
}

void process (sem_t * screen, sem_t * keyboard, int index){
	// get procces id
	int id = getpid();
	// initalize count of deadlocks to 0
	int count=0;
	// integer to get size of buffer
	int len;
	// string for input of size 80
	char  * buff;
	buff=  malloc(sizeof(char)*80);
	//loop

	do {
		count += getSemaphores(screen,keyboard,index);
		// prompt id + " enter < 80 characters " + id + " or q to quit: "
		printf("%d: Enter < 80 characters or %d  q to quit \n",id,id);
		//   read keboard
		fgets(buff,80,stdin);
		// get rid of newline
		len = strlen(buff);
		if (len > 0 && buff[len-1] == '\n') {
			buff[--len] = '\0';
		}
		// get rid of anything more than 80 characters
		fseek(stdin,0,SEEK_END);
		// echo what was typed with id before and after echo
		printf("%d start %s end %d \n",id,buff,id );

		// sem_post or give back both semaphores
		sem_post(keyboard);
		sem_post(screen);
		//until quit end inner loop in C do ... while not q


	} while (strcmp(buff,"q")!=0);
	count += getSemaphores(screen,keyboard,index);
	//prompt "This process " + id + " had " + count + " deadlocks "
	printf("This process %d had %d deadlocks\n",id,count);
	// sem_post or give back both semaphores
	sem_post(keyboard);
	sem_post(screen);
	//sem_close


}



//function to get semaphores (screen, keyboard, index)
int getSemaphores(sem_t * screen, sem_t * keyboard,int index){
	struct timespec ts;
	// dummy semaphores
	sem_t* first;
	sem_t* second;
	int have_both=0;
	// do we have both sems
	int count = 0;
	//returns count of how many times recovered from deadlock
	//even index gets keyboard first
	if(index%2==0) {
		first=keyboard;
		second=screen;
	}
	//odd index gets screen first
	else{
		first =screen;
		second =keyboard;
	}
	//can use temp and swap semphores
	//loop
	while(have_both!=1) {
		//sem_wait for first
		sem_wait(first);
		//  perror("sem_wait");
		//sleep(1);                // only use this if no deadlocks without it should not be necessary
		//wait with timer for second
		if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
			perror("clock_gettime issue");
		ts.tv_sec += 1;
		int sucess= sem_timedwait(second, &ts);

		//if timeout
		if(sucess!=0) {
			//  give back first, (sem_post)
			sem_post(first);

			//  printf("sleeping for %lu Nanoseconds %d\n",ts.tv_nsec,getpid());
			//wait random time

			srand(index%3);
			ts.tv_nsec+=(rand());
			nanosleep(&ts,NULL);
			//increment count
			count++;


		}else{
			//  have both  true
			have_both=1;
		}
	}



	return count;

}
