#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define EXPECTED_NUMBER_OF_PARAMETERS 3
#define RAND_EXCLUSIVE_RANGE 501
#define NANOSEC_MULTIPLIER 1000000
#define DEBUG_YELLOW "\x1B[33m"
#define DEBUG_DEFAULT "\x1B[0m"
#define DECADIC_BASE 10
#define DEBUG 0

int getticket (void);
void await (int aenter);
void advance (void);
void debugMessage(char* message);
void processParameters(char* argv[]);
int getticket();
void *threadWork(void *threadId);
unsigned int sleepThread(unsigned int *seed);
void showHelp();
unsigned int getRndThreadSleepTime(unsigned int *seed);

struct Parameters
{
    long threadCount;
    long passes;
};
struct Parameters P;

pthread_mutex_t ticketDistributionMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entryMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wakeCond = PTHREAD_COND_INITIALIZER;
int currentTicket = 1;
int lastticket = 0;


int main (int argc, char* argv[])
{
    if (argc < EXPECTED_NUMBER_OF_PARAMETERS)
    {
        showHelp();
        exit(1);
    }

    processParameters(argv);

    if (DEBUG)
    {
        debugMessage("Parameter parsing was successfull, program will now run.");
        printf ("%s", DEBUG_YELLOW);
        printf("Program will be run with N=%ld, M=%ld\n", P.threadCount, P.passes);
        printf ("%s", DEBUG_DEFAULT);
    }

    // Spawn threads, prepare counters
    // TODO: Make this more GNU like? 
    pthread_t threads[P.threadCount];
    long threadCreationCode;    // FUTURE-TODO: Use this to detect error code 11 (EAGAIN), wait for a bit then try again 
    int i;

    for (i = 0; i < P.threadCount; i++)
    {
        threadCreationCode = pthread_create (&threads[i], NULL, threadWork, (void *)(intptr_t)i);
        if (threadCreationCode)
        {
            printf ("ERROR; return code from pthread_create() is %ld\n", threadCreationCode);
            exit (-1);
        }
    }

    // Clean up threads
    for (i = 0; i < P.threadCount; i++)
    {
        pthread_join (threads[i], NULL);
        
        if (DEBUG)
            printf ("Thread %i was collected.\n", i);
    }
    
    // Ensure the rest of the threads is not killed off when main thread reaches this point
    pthread_exit (NULL);

    return 0;
}


/**
  * threadWork()
  * Start routine for pthread_create() that contains main skelet for the project
  * void *threadId is a pthread API specific argument which in my case serves to
  * pass a identification to thread.
  */
void *threadWork(void *threadId)
{
    // Thread-local variables
    int ticket;
    long tid;
    tid = (intptr_t) threadId;

    // Prepare seed for random generator
    unsigned int seed = (unsigned int) (time(NULL) + tid);

    // Debug timing variables
    unsigned int debugSleepTimer1;
    unsigned int debugSleepTimer2;


    while ((ticket = getticket()) <= P.passes)
    {
        // Initial sleep
        debugSleepTimer1 = sleepThread(&seed);
        if (DEBUG)
            printf ("Thread #%ld was put to sleep for %ums\n", tid, debugSleepTimer1);

        // Protect entry to critical section
        await(ticket);
        if (DEBUG)
            printf ("Thread #%ld passed through await() function.\n", tid);


        // Print who is in critical section
        printf("%d\t(%d)\n", ticket, (int) tid);


        // Leavy protected critical section and allow next one in
        advance();
        if (DEBUG)
            printf ("Tread #%ld passed through advance() function.\n", tid);

        // Termination sleep
        debugSleepTimer2 = sleepThread(&seed);
        if (DEBUG)
            printf ("Thread #%ld was put to sleep for %ums\n", tid, debugSleepTimer2);
    }

    pthread_exit(NULL);
}


/**
  * await()
  * Protects critical section from multiple threads entering it at the same time
  * int aenter - Integer ticket number identification of a thread that wants to enter
  */
void await (int aenter)
{
    // First thing to do is to prevent other threads from entering
    if (pthread_mutex_lock(&entryMutex) != 0)
    {
        fprintf (stderr, "Error while using pthread_mutex_lock.\n");
    }

    // Using while + passive waiting to protect critical zone as suggested on:
    // https://computing.llnl.gov/tutorials/pthreads/
    while (currentTicket != aenter)
    {
        if (pthread_cond_wait(&wakeCond, &entryMutex) != 0)
            fprintf(stderr, "Error while using pthread_cond_wait().\n");
    }
}


/**
  * advance()
  * Unlocks critical section and sends wake up signal to all threads,
  * threads whose ticket number is 'called' proceeds to await(), the
  * rest is put to sleep again.
  */
void advance ()
{
    if (pthread_mutex_unlock (&entryMutex) != 0)
    {
        fprintf (stderr, "Error while unlocking mutex.\n");
    }
    else
    {
        // There is no need to lock mutex before incrementation, all other threads are asleep
        currentTicket++;
        if (pthread_cond_broadcast (&wakeCond))
        {
            fprintf(stderr, "Waking broadcast error!\n");
        }
    }
}


/**
  * getticket()
  * Gets a waiting ticket to critical section for thread, is mutex protected.
  *
  * NOTE: last ticket does not have to be declared static sinec it is considered
  * shared among threads.
  */
int getticket ()
{
    // Mutex-Protected ticket number incrementation
    pthread_mutex_lock (&ticketDistributionMutex);
    lastticket += 1;
    pthread_mutex_unlock (&ticketDistributionMutex);
    
    return lastticket;
}


/**
  * processParameters()
  * Handles parameter processing for application
  */
void processParameters (char* argv[])
{

    char *endptr;

    // Here should come some sort of a check whether this works.
    // TODO: Sanitize the input and buffer underflow, inspiration here: http://stackoverflow.com/questions/26080829/detecting-strtol-failure

    if (argv[1])
        P.threadCount = strtol(argv[1], &endptr, DECADIC_BASE);

    if (argv[2])
        P.passes = strtol(argv[2], &endptr, DECADIC_BASE);
}


/**
  * sleepThread()
  * Sleeps thread for a random amount of time between 0.000s and 0.500s based on seed
  * unsigned int* seed - time based seed for a thread
  * returns number of miliseconds that thread was put to sleep
  */
unsigned int sleepThread (unsigned int *seed)
{
    // Get random sleep time from range
    unsigned int sleepTime = getRndThreadSleepTime (seed);

    // Compose timespec structure for nanosleep() function
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = (sleepTime * NANOSEC_MULTIPLIER);

    nanosleep (&time, NULL);

    // Return amount of miliseconds that were slept (debug purposes, mainly)
    return sleepTime;
}


/**
  * getRndThreadSleepTime()
  * Based on seed returns random number from within range 0-500
  * unsigned int* seed
  *
  * NOTE: Since it is not the sole purpose of this project, I am satisfied with thread-safe
  * pseudo-generator where randomness is not uniformly distributed. For the purpose of this
  * project, this solution should be sufficient.
  */
unsigned int getRndThreadSleepTime (unsigned int *seed)
{
    return (rand_r(seed) % RAND_EXCLUSIVE_RANGE);
}


/**
  * Displays program's help screen
  */
void showHelp ()
{
    printf ("Ticket algorithm synchronization demo.\n\n");
    printf ("Usage: ./XXX N M\n\n");
    printf ("Creates N threads and simulates M number of total passes through critical section protected by ticket algorithm.\n\n");
    printf ("No other available options. Wrong parameters to the program shows this help.\n");
}


/**
  * Displays yellow debug message passed as parameter
  * char* message - string message to be displayed in debug colors
  */
void debugMessage (char* message)
{
    printf ("%s", DEBUG_YELLOW);
    printf ("%s\n", message);
    printf ("%s", DEBUG_DEFAULT);
}