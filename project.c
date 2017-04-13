#include <stdlib.h> // Contains even thread safe functions like rand_r and drand48_x
#include <stdio.h>

int getticket (void);
void await (int aenter);
void advance (void);
void debugMessage(char* message);
void processParameters(char* argv[]);

#define DEBUG 1
#define DEBUG_YELLOW "\x1B[33m"
#define DEBUG_DEFAULT "\x1B[0m"

#define DECADIC_BASE 10

struct Parameters
{
    long threadCount;
    long passes;
};

struct Parameters P;

void showHelp ()
{
    printf ("Ticket algorithm synchronization demo.\n\n");
    printf ("Usage: ./XXX N M\n\n");
    printf ("Creates N threads and simulates M number of total passes through critical section protected by ticket algorithm.\n\n");
    printf ("No other available options. Wrong parameters to the program shows this help.\n");
}

int main (int argc, char* argv[])
{
    if (argc < 3)
    {
        showHelp();
        exit(1);
    }

    processParameters(argv);

    if (DEBUG)
    {
        debugMessage("Parameter parsing was successfull, program will now run.");   // God, I wish it was easier to work with integers, so I could print something more meaningful
        printf ("%s", DEBUG_YELLOW);
        printf("Program will be run with N=%ld, M=%ld\n", P.threadCount, P.passes);
        printf ("%s", DEBUG_DEFAULT);
    }


    //int ticket;
    //while ((ticket = getticket()) < P.)

}

void debugMessage(char* message)
{
    printf("%s", DEBUG_YELLOW);
    printf ("%s\n", message);
    printf("%s", DEBUG_DEFAULT);
}

/**
  * processParameters()
  * Takes char* argv from main, converts its contents to integer and fills the Parameters P global variable
  */
void processParameters(char* argv[])
{

    char *endptr;

    // Here should come some sort of a check whether this works.
    // TODO: Sanitize the input and buffer underflow, inspiration here: http://stackoverflow.com/questions/26080829/detecting-strtol-failure
    // if ()

    if (argv[1])
        P.threadCount = strtol(argv[1], &endptr, DECADIC_BASE);

    if (argv[2])
        P.passes = strtol(argv[2], &endptr, DECADIC_BASE);
}




/*
    
    RECOMMENDED PROGRAM SKELETON

while ((ticket = getticket()) < M) 
{ 
                                        // Pøidìlení lístku
                                        // Náhodné èekání v intervalu <0,0 s, 0,5 s> 
    await(ticket);                      //Vstup do KS 
    printf("%d\t(%d)\n", ticket, id);   // fflush(stdout); 
    advance();                          //Výstup z KS 
                                        // Náhodné èekání v intervalu <0,0 s, 0,5 s> 
}

*/