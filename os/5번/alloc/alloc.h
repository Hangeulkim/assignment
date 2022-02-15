#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

typedef struct DATA{
    int flag;
    int sz;
    char *start;
    char *end;
}data;

typedef struct PLIST{
    int cnt;
    int fd;
    char *mem;
    data list[201];
}plist;


// function declarations
int init_alloc();
int cleanup();
char *alloc(int);
void dealloc(char *);
