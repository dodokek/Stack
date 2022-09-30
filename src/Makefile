#Environment settings

CC = g++
CFLAGS = -g -Wall

#Variables

MAIN   = main
STACK  = stack

#Body

all : $(MAIN)

$(MAIN)     : $(MAIN).o $(STACK).o  
	$(CC) $(CFLAGS) $^ -o $@   

$(MAIN).o   : $(MAIN).cpp
	$(CC) $(CFLAGS) $< -c 

$(STACK).o  : $(STACK).cpp
	$(CC) $(CFLAGS) $< -c	  

clean:
	rm -rf *.o *.save $(MAIN)