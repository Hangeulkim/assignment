#ifndef __SSU_CHECK__
#define __SSU_CHECK__

#include<sys/stat.h>

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif

#define BUFLEN 1024
#define FILELEN 256

typedef struct NODE{
	struct NODE *parent;
	struct NODE *child;
	struct NODE *next;
	struct NODE *prev;
	struct stat data;
	char real_path[FILELEN];
	char realative_path[FILELEN];
	char name[FILELEN];
	int depth;
}node;


void ssu_check(char *name);

node *create_node(struct stat input, char *path, char *r_path);
void free_tree(node *root);

long dir_size(node *root);
void size_list(node *root, int depth);
void print_tree(node *root);

void write_data(node *root1, node *root2);

node *make_tree(char *path, int depth);
node *make_list(char *path, node *root, int depth);

#endif
