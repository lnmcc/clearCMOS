cc=gcc
CFLAGS=-Wall -W -O2

cleancmos: cleancmos.c
	$(CC) $(CFLAGS) -o cleancmos cleancmos.c
