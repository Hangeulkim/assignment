#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

//granularity of memory to mmap from OS
#define PAGESIZE 4096

//minimum allocation size
#define MINALLOC 256

typedef struct DATA{
    int flag;
    int sz;
    int page_num;
    char *start;
    char *end;
}data;

typedef struct PLIST{
    int cnt;
    int page_cnt;
    int fd;
    char *mem[4];
    data list[1001];
}plist;

// function declarations to support
void init_alloc(void);
char *alloc(int);
void dealloc(char *);
void cleanup(void);
