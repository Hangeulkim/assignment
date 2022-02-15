#ifndef __SSU_RSYNC__
#define __SSU_RSYNC__

#include<sys/stat.h>

#define BUF 1024
#define COM 10240
#define TRUE 1
#define FALSE 0
#define SECOND_TO_MICRO 1000000

int main(int argc, char *argv[]);

typedef struct NODE{
	struct NODE *next;
	struct NODE *prev;
	struct stat data;
	char real_path[BUF];
}node;

typedef struct SYNC_DATA{
	struct SYNC_DATA *next;
	struct SYNC_DATA *prev;
	struct stat data;
	int files;
	char src_path[BUF];
    char dst_path[BUF];
    int same;
}sync_data;

void check_option(int argc, char *argv[]);
void tOption();
void mOption();

int rmdirs(const char *path, int force);

void run_sync(char *src, char *dst);
void rsync_data(node *src_root, node *dst_root);
void file_sync(sync_data *cur);
void write_sync();
void delete_tmp();

void write_log();
void t_write_log(char *str);

void check_err(char *src, char *dst);

node *make_list(char *path, node *root);
node *create_node(struct stat input, char *r_path);
#endif
