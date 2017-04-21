# 2017POSProject-TicketAlgo
POS Ticket Algorithm project repository, Brno, University of Technology, 2017

Feel free to get inspired, but tread carefully, Preston, you may be treading on your dreams in case you decide to copy paste.


According to our class forum, the project should be able to compile with -ansi -std=c99 and -std=c11 without onscreen errors and screams, hence the commentaries and weird coding style.



_____
Thread unsafe functions:
* usleep()

Thread safe functions: 
* nanosleep()
* rand_r(3) - https://linux.die.net/man/3/rand_r
* drand48_r(3) https://linux.die.net/man/3/drand48_r