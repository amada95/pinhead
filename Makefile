CC		= gcc
CSRC	= pinhead.c 
CFLAGS	= -O2 -Wall -Wpedantic -std=c99
COUT	= pinhead.bin

all:
	$(CC) -o $(COUT) $(CSRC) $(CFLAGS)
	
clean:
	rm -rf $(COUT)
