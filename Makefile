# Author: Daniel Dusek <xdusek21@stud.fit.vutbr.cz>
# POS Project 2017: Ticket algorithm

# ISSUES:
# -ansi | -std=c99 -- and a load of different stuff that compiler is not capable of handling 
# REMOVE -std=gnu99, that is false!

TARGET = project.c

all:
	gcc -std=c11 -pedantic -Wall $(TARGET) -o project -lpthread


run:
	@./project 100 50


clean:
	rm project