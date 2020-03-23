/*
   Alexnder Boyd
   CS4540
   Assigment 3
 */

#include "threads.h"
typedef struct holdsem_t {
	sem_t* keyboard;
	sem_t* screen;
	int index;
} sem_hold;

int main (int argc, char const *argv[]) {
	sem_hold* input =malloc(sizeof(sem_hold));

	int num_threads=7;
	sem_hold *test[num_threads];
	int error=0;

	// make an array of threads
	pthread_t threads[num_threads];
	//allocate memory for each semephore
	input->screen= malloc(sizeof(sem_t *));
	input->keyboard=malloc(sizeof(sem_t *));
	for (size_t i = 0; i < num_threads; i++) {
		test[i]=malloc(sizeof(sem_hold));

	}

	if(sem_init(input->screen,0,1)!=0) {                                                                      // open screen semphore
		perror("sem_init:");
		error=1;
	}
	if (sem_init(input->keyboard,0,1)!=0)   {                                                         //open keyboard semephore
		perror("sem_init:");
		error=1;
	}

	int index = 0;
	if (error==0) {
		do {

			// create threads

			test[index]=input;
			test[index]->index =index;
			pthread_create(&threads[index],NULL,process,test[index]);
			//    sleep(1);

			index++;
		}
		while( index < num_threads);
		if(index >= num_threads) {
			// stop forking around
			//   after all die, use single call or loop based on ids saved above
			// sem_unlink both


			for(int i=0; i<num_threads; i++) {
				pthread_join(threads[i],NULL);                                                                                                                                                                                                                                 // wait for process to end till close
			}
		}
	}

	if (sem_destroy(input->screen)!=0) {
		perror("sem_destroy");

	}
	if (sem_destroy(input->keyboard)!=0 ) {
		perror("sem_destroy");
	}


	return 0;

}

void * process (void * input){

	sem_hold data =*(sem_hold*) input;

	int index = data.index;

	int id= pthread_self();
	// initalize count of deadlocks to 0
	int count=0;
	// integer to get size of buffer
	int len;
	// string for input of size 80
	char  * buff;
	buff=  malloc(sizeof(char)*80);
	//  loop

	do {
		count += getSemaphores(data.screen,data.keyboard,index);
		// prompt id + " enter < 80 characters " + id + " or q to quit: "
		printf("%d: Enter < 80 characters or q to quit %d \n",id,id);
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
		sem_post(data.keyboard);
		sem_post(data.screen);
		//until quit end inner loop in C do ... while not q


	} while (strcmp(buff,"q")!=0);
	count += getSemaphores(data.screen,data.keyboard,index);
	//prompt "This process " + id + " had " + count + " deadlocks "
	printf("This process %d had %d deadlocks\n",id,count);
	// sem_post or give back both semaphores
	sem_post(data.keyboard);
	sem_post(data.screen);

//sem_close



	pthread_exit(0);
}



//function to get semaphores (screen, keyboard, index)
int getSemaphores(sem_t * screen, sem_t * keyboard,int index){
	struct timespec ts;
	// arrary of semephores used for first wait
	// amount of time to sleep
	sem_t* first;
	sem_t* second;

	int have_both=0;                                                         // do we have both sems
	int count = 0;                                                          //returns count of how many times recovered from deadlock
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
		//  sleep(1);                // only use this if no deadlocks without it should not be necessary
		//wait with timer for second
		if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
			printf("clock_gettime issue");
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
