cc=gcc
CFLAGS=-Wall -W -O2

clearCMOS: clearCMOS.c
	$(CC) $(CFLAGS) -o clearCMOS clearCMOS.c
