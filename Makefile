# Author: Daniel Dusek <xdusek21@stud.fit.vutbr.cz>
# POS Project 2017: Ticket algorithm

# ISSUES:
# -ansi | -std=c99 -- and a load of different stuff that compiler is not capable of handling 
# REMOVE -std=gnu99, that is false!

TARGET = project.c

all:
	gcc -ansi -pedantic -Wall $(TARGET) -o project -lpthread -O -g


run:
	@./project 100 50

wis-pack:
	zip "xdusek21.zip" "project.c" "Makefile"

clean:
	rm project
