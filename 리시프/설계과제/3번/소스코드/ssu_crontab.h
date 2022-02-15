#ifndef __SSU_CRONDTAB__
#define __SSU_CRONDTAB__

#define SECOND_TO_MICRO 1000000
#define BUF 1024
#define TRUE 1
#define FALSE 0

int errchk(char *str);

void add(char *str);
void del(int num);
void exit_function();

#endif
