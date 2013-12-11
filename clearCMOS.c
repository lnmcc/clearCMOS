#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/io.h>
#include <sys/perm.h>
#include <ctype.h>

#define TAILLE_CMOS 4*0x80
#define TAILLE_CMOS_MAX 4096

#define IO_READ 1
#define IO_WRITE 2
#define PORT_CMOS_0 0x70
#define PORT_CMOS_1 0x71
#define PORT_CMOS_2 0x72
#define PORT_CMOS_3 0x73
#define PORT_CMOS_4 0x74
#define PORT_CMOS_5 0x75
#define PORT_CMOS_6 0x76
#define PORT_CMOS_7 0x77

uint8_t cmos[TAILLE_CMOS_MAX];

void set_permissions()
{
	if(ioperm(PORT_CMOS_0, 4 * 2, IO_READ|IO_WRITE))
	{
		printf("Need to be run as root!\n");
		exit(1);
	}
}

void unset_permissions()
{
	ioperm(PORT_CMOS_0, 4 * 2, 0);
}

static __inline__ void outportb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %0,%1"
				::"a" ((char) value), "d"((uint16_t) port));
}

static __inline__ uint8_t inportb(uint16_t port)
{
	uint8_t _v;
	__asm__ volatile ("inb %1,%0"
				:"=a" (_v):"d"((uint16_t) port));
	return _v;
}

uint8_t read_cmos(const unsigned int cell)
{
	if(cell < 128)
	{
		outportb(PORT_CMOS_0, cell);
		return inportb(PORT_CMOS_1);
	}
	if(cell < 2 * 128)
	{
		outportb(PORT_CMOS_2, cell);
		return inportb(PORT_CMOS_3);
	}
	if(cell < 3 * 128)
	{
		outportb(PORT_CMOS_4, cell);
		return inportb(PORT_CMOS_5);
	}
	if(cell < 4 * 128)
	{
		outportb(PORT_CMOS_6, cell);
		return inportb(PORT_CMOS_7);
	}
	return 0;
}

void write_cmos(const unsigned int cell, const uint8_t value)
{
	if(cell < 128)
	{
		outportb(PORT_CMOS_0, cell);
		outportb(PORT_CMOS_1, value);
		return;
	}
	if(cell < 2 * 128)
	{
		outportb(PORT_CMOS_2, cell);
		outportb(PORT_CMOS_3, value);
	    return;
	}
	if(cell < 3 * 128)
	{
		outportb(PORT_CMOS_4, cell);
		outportb(PORT_CMOS_5, value);
		return;
	}
	if(cell < 4 * 128)
	{
		outportb(PORT_CMOS_6, cell);
		outportb(PORT_CMOS_7, cell);
		return;
	}
}

int load_cmos(const int cmos_size)
{
	int i;
	set_permissions();
	for(i = 0; i < cmos_size; i++)
		cmos[i] = read_cmos(i);
	unset_permissions();
	return 0;
}

int kill_cmos(const int cmos_size)
{
	int i;

	printf("Warning: begin to clear cmos...\n");
	fflush(stdout);
	set_permissions();
	for(i = 0x10; i < cmos_size; i++)
		write_cmos(i, 0);
	unset_permissions();
	printf("cmos cleared!\n");
	return 0;
}

int main(int argc, char *argv[])
{
	printf("ver:1.0\n");
	
	int cmos_size = TAILLE_CMOS;

	memset(cmos, 0, sizeof(cmos));
	kill_cmos(cmos_size);
	return 0;
}


