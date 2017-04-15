# 2017POSProject-TicketAlgo
POS Ticket Algorithm project repository, 2017



// Notes from forum, jesus fucking christ
* Při překladu kompilátorem gcc povinně zachovejte parametr -Wall.
* Program musí splňovat normy jazyka C (ANSI, C99, C11) a POSIX (doporučuje se používat pro gcc parametr -pedantic, který najde nesrovnalosti).
* Dále je nutno zkontrolovat vložení všech potřebných hlavičkových souborů pro deklarace použitých symbolů. Pokud si překladač na cokoliv stěžuje, mohou za to být strženy body! 
* Odevzdávané soubory budou mít unixové konce řádků (0x0A) a budou v kódování ISO8859-2, jinak nemusí být projekt vůbec hodnocen.


Thread unsafe functions:
* usleep()

Thread safe functions: 
* nanosleep()
* rand_r(3) - https://linux.die.net/man/3/rand_r
* drand48_r(3) https://linux.die.net/man/3/drand48_r


TODO: Ensure thread is explicitly created as joinable 
TODO: Find out whether to call pthread_mutex_destroy() or not
TODO: Ensure distribution of tickets is also thread safe

pthread.h? libc?

pthread_create instead of fork

Functions xbenickova used:
 pthread_creat, pthread_join a potom sve premenne pthread_mutex_t (ma funkciu loc ka unlock, to ti zarusi ze vojde iba jeden)
a pthread_cond_t

REPO: https://github.com/PoRTiLo/POS2/blob/master/proj02.c

/*** CONDITIONAL_WAIT ***/
while(aenter != actualTicket)
	{
		err = pthread_cond_wait(&CSCond, &mutexTicket);
		if (err != 0)
		{
			fprintf(stderr, "pthread_cond_wait() err=%d\n", err);
		}
	}


/****** PARTIAL XBENICKO CODE *******/
threads = (pthread_t*) malloc(N*sizeof(pthread_t));
	myId = (int*) malloc(N*sizeof(int));

	/*creating threads*/
	for (i=0;i<N;i++){
		pthread_mutex_lock(&create_mutex);
		myId[i]=threadNum++;
		pthread_mutex_unlock(&create_mutex);
		pthread_create(&threads[i], NULL, thread_main,&myId[i]);
		
	}

	/*collecting threads*/
	for (i=0;i<N;i++){
		if(pthread_join(threads[i], NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;

		}
	}
	free(threads);
	free(myId);\
