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

