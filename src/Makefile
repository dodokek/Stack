#Environment settings

CC = g++
CFLAGS = -g -Wall

#Variables

MAIN   = stack

#Body

all : $(MAIN)

$(MAIN)     : $(MAIN).o  
	$(CC) $(CFLAGS) $^ -o $@   

$(MAIN).o   : $(MAIN).cpp
	$(CC) $(CFLAGS) $< -c 
	  

clean:
	rm -rf *.o *.save $(MAIN)