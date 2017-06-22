/*
* Rohan Krishna Ramkhumar
* rxr353
* Assignment 7: Baboon crossing problem using pThreads and Semaphores
*/

#include <pthread.h>  
#include <semaphore.h> 
#include <stdio.h>    
#include <stdlib.h>
#include <unistd.h>	

#ifndef tNum
#define tNum 20
#endif 
//Thread's data. Contains the thread's ID.
typedef struct _tData_t	{int tID; }tData_t;

sem_t toA, toB, mutex; //Needed crossing Semaphores

int xingCnt, xedCnt, toAwaitCnt, toBwaitCnt;

enum {None, aBnd, bBnd} xingDir; //Crossing Direction enum

//Baboon going toward A and B respectively
void *a(void *arg);
void *b(void *arg);

//Semaphore wait and signal functions
//Changed to now use example program function
void semWait(sem_t *sem);
void semSignal(sem_t *sem);

/*
* Please Note that I've used examples to help
* with some of the functions
*/
int main(){

	//Standard Decalration: Sorry I didn't do this last time!
	//Was kinda rushed to submit :P 
	printf("Rohan Krishna Ramkhumar\n");
	printf("Case ID: rxr353\n");
	printf("Assignment 7: Baboon crossing problem using pThreads and Semaphores\n");
	printf("*****************\n\n");
	fflush(stdout);

	tData_t tData[tNum];
	pthread_t thread[tNum];

	int errCheck;

	if (sem_init(&mutex, 0, (unsigned int)1) < 0
        || sem_init(&toA, 0, (unsigned int)1) < 0
        || sem_init(&toB, 0, (unsigned int)1) < 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    printf("Parent Process PID: %d \n", getpid());//parent PID
    fflush(stdout);

    //The Below function to create threads is from the course website
    for (int i = 0; i < tNum; ++i) {

		void *thread_func;//the function to call

		tData[i].tID = i;//set thread id to current i value

		if (rand()%tNum <=tNum/2) {
			thread_func = a;//Baboon to B
		} 
		else{
			thread_func = b;//Baboon to a
		}

		sleep(1);
		fflush(stdout);
		sleep(1);
		fflush(stdout);

if ((errCheck = pthread_create(&thread[i], NULL, thread_func, &tData[i]))) {
            fprintf(stderr, "error: pthread_create, %d\n", errCheck);
            return EXIT_FAILURE;
        } //end of error check if statement
    }//end of thread creation for loop
    
    
    //Pthread join error check (ref: banking assignemnt)
    for (int i = 0; i < tNum; ++i) {
        if ((errCheck = pthread_join(thread[i], NULL))) {
            fprintf(stderr, "error: pthread_join, %d\n", errCheck);
        }
    }//end of pthread join errorcheck for loop


	return EXIT_SUCCESS;
}

/*
* Function for the Baboon to B
* Portions of this are written with reference to 
* the Course Website Examples
*/
void *b(void *arg){
	tData_t *data = (tData_t *)arg;

	int tIDb = data->tID;
	semWait(&mutex);

	printf("****\nThread %d: Baboon Crossing from A to B\n", tIDb);
	fflush(stdout);

	//Portions of the following if Statement are from my as6
	if ((xingDir == bBnd || xingDir == None) && 
		(xingCnt < 5) &&
		((xedCnt + xingCnt) < 10)){

		printf("Baboon is Crossing from A to B\n");
		fflush(stdout);

		xingDir = bBnd;
		xingCnt++;

		printf("Signaling Mutex\n");
		fflush(stdout);

		semSignal(&mutex);
	}
	else{
		printf("B is now supposed to wait\n");
		fflush(stdout);
		toBwaitCnt++;

		printf("Signalling mutex \n");
		fflush(stdout);
		semWait(&mutex);
		semWait(&toB);

		printf("B was waiting until signal call.");
		printf("Signal has been called \n");
		fflush(stdout);
		toBwaitCnt--;
		xingCnt++;
		xingDir = bBnd;


		printf("Baboon about to cross \n");
		printf("Signaling Mutex\n");
		fflush(stdout);
		semSignal(&mutex);
	}


	//"Critical" Section
	printf ("Rope Crossing\n");
	printf("Rope Crossed. Waiting for mutex \n");
	fflush(stdout);
	semWait(&mutex);
	printf("Mutex Passed\n");
	fflush(stdout);
	xedCnt++;
	xingCnt--;

	//Once again, I would like to thank as6 and as4 for these if statements
	//And don't forget the Course Website!
	if (toBwaitCnt!= 0 &&
		(xingCnt + xedCnt < 10 ||
			(xedCnt + xingCnt>= 10 && toAwaitCnt == 0))){

		printf("Signaling Baboon Crossing to A\n");
		fflush(stdout);

		semSignal(&toB);
	}

	else if(xingCnt == 0 &&
		toAwaitCnt !=0 &&
		(toBwaitCnt == 0 ||
			xedCnt + toBwaitCnt >= 10)){
		printf("Changing Direction to turn toward B\n");
		printf("Signaling Baboon waiting to cross to B\n");
		fflush(stdout);

		xingDir = aBnd;
		xedCnt = 0;
		semSignal(&toA);
	}

	else if(xingCnt == 0 &&
		toBwaitCnt == 0 && 
		toAwaitCnt == 0){
		xingDir = None;
		xedCnt = 0;

		printf("Crossing Direction Reset \n");
		fflush(stdout);
		semSignal(&mutex);
	}

	else{
		printf("Signaling Mutex\n");
		fflush(stdout);
		semSignal(&mutex);
	}

	pthread_exit(NULL);
}


/*
* Function for the Baboon to A
* Portions of this are written with reference to 
* the Course Website Examples
*/
void *a(void *arg){
	tData_t *data = (tData_t *)arg;

	int tIDa = data->tID;
	semWait(&mutex);

	printf("****\nThread %d: About to cross\n", tIDa);
	fflush(stdout);

	//Portions of the following if Statement are from my as6
	if ((xingDir == aBnd || xingDir == None) && 
		xingCnt < 5 &&
		(xedCnt + xingCnt < 10)){

		printf("Baboon is Crossing from B to A\n");
		printf("Crossing Finished\n");
		fflush(stdout);

		xingDir = aBnd;
		xingCnt++;

		printf("Signalling Mutex\n");
		fflush(stdout);

		semSignal(&mutex);
	}
	else{
		printf("A is now supposed to wait\n");
		fflush(stdout);
		toAwaitCnt++;

		printf("Signalling mutex \n");
		fflush(stdout);
		semWait(&mutex);
		semWait(&toA);

		printf("A was waiting until signal call. Signal has been called \n");
		fflush(stdout);
		toAwaitCnt--;
		xingCnt++;
		xingDir = aBnd;

		printf("Baboon about to cross \n");
		fflush(stdout);
		printf("Signaling Mutex\n");
		semSignal(&mutex);
	}


	//"Critical" Section
	printf (" Rope Crossing\n");
	printf("Rope Crossed. Waiting for mutex \n");
	fflush(stdout);
	semWait(&mutex);
	printf("Mutex Passed\n");
	fflush(stdout);
	xedCnt++;
	xingCnt--;

	//Once again, I would like to thank as6 and as4 for these if statements
	//And don't forget the Course Website!
	if (toAwaitCnt!= 0 &&
		(xingCnt + xedCnt < 10 ||
			(xedCnt + xingCnt>= 10 && toBwaitCnt == 0))){

		printf("Signaling Baboon Crossing to A\n");
		fflush(stdout);

		semSignal(&toA);
	}

	else if(xingCnt == 0 &&
		toBwaitCnt !=0 &&
		(toAwaitCnt == 0 ||
			xedCnt + toBwaitCnt >= 10)){
		printf("Changing Direction to turn toward B\n");
		printf("Signaling Baboon waiting to cross to B\n");
		fflush(stdout);

		xingDir = bBnd;
		xedCnt = 0;
		semSignal(&toB);
	}

	else if(xingCnt == 0 &&
		toAwaitCnt == 0 && 
		toBwaitCnt == 0){
		printf("Crossing Direction Reset \n");
		fflush(stdout);
		xingDir = None;
		xedCnt = 0;

		printf("Signaling Mutex\n");
		fflush(stdout);
		semSignal(&mutex);
	}

	else{
		printf("Signaling Mutex\n");
		fflush(stdout);
		semSignal(&mutex);
	}

	pthread_exit(NULL);
}

/*
 *	Error-checked semaphore wait.
 */
void semWait(sem_t *sem) {
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

/*
 *	Error-checked semaphore signal.
 */
void semSignal(sem_t *sem) {
    if (sem_post(sem) < 0) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}
