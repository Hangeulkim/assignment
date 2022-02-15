#ifndef __SSU_CROND__
#define __SSU_CROND__

#include<time.h>

#define SECOND_TO_MICRO 1000000
#define BUF 1024
#define TRUE 1
#define FALSE 0

typedef struct How{
    int time[60];
}how;

typedef struct When{
    how min;
    how hour;
    how day;
    how mon;
    how seven;
    time_t start;
}when;

typedef struct NODE{
    struct NODE *next;
    struct NODE *prev;
    when data;
    char com[BUF];
    char all[BUF];
    int cnt;
}node;

node *make_list(FILE *fp);
int cnt_chk(node *cur);
void *run(void *arg);
node *del_node(node *cur);
node *add_node(char *str);
void dif_node(node *a_root, node *b_root);
void del_all(node *root);

#endif
