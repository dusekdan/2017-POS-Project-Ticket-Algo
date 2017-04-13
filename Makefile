# Author: Daniel Dusek <xdusek21@stud.fit.vutbr.cz>
# POS Project 2017: Ticket algorithm

TARGET = project.c

all:
	gcc -pedantic -Wall -std=c99 $(TARGET) -o project


run:
	@./project 100 50


clean:
	rm project