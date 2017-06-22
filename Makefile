#Rohan Krishna Ramkhumar
#rxr353
#Makefile for assignment 6: System V Semaphores
#Reference used: Makefile from Jan 24 Recitation

CC = gcc
OUT = output.o
SRC = baboon.c
TXT1 = output1.txt
TXT2 = output2.txt
all: 	as7

as7: baboon.c 
	$(CC) -Wall -pthread -std=c99 -o $(OUT) $(SRC)
	chmod 700 $(OUT)
	 ./$(OUT) >$(TXT1)
	 ./$(OUT) >$(TXT2)

	
clean:
	rm -f $(OUT)
	rm -f $(SOUT)	
