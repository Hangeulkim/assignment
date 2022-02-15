#ifndef __SSU_MNTR__
#define __SSU_MNTR__

#include<time.h>

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif

#define MAX_BYTE 2000
#define FILELEN 256
#define BUFLEN 1024

typedef struct info{
	struct info *next;
	struct info *prev;
	char where[FILELEN];//files�� ��θ� ������ �迭
	char delete_name[FILELEN];//������ ���� �̸�
	char real_path[FILELEN];//������ ���� ���
	struct tm m_time;//���� ���� �ð�
	struct tm d_time;//������ ���� �ð�
}info;

void delete_files();
void delete_files_info();

void ssu_mntr();

void chk_option_delete(char *input);
void chk_option_size(char *input);
void chk_option_recover(char *input);

void delete_data();
void init_info(char *info_path);
void sort_info();
void delete_info();

void recover(char *input);
void delete(char *input);
void size(char *input);
void tree();
void help();
void Exit();
void delete_infos();

#endif
