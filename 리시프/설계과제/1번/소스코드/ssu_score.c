#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_score.h"
#include "blank.h"

extern struct ssu_scoreTable score_table[QNUM];//������ ���� ���̺�
extern char id_table[SNUM][10];//�л��� id ���̺�

struct ssu_scoreTable score_table[QNUM];//���ھ� ���̺��� ������
char id_table[SNUM][10];//�л��� id ���̺��� ������

char stuDir[BUFLEN];//�л� �� ���丮
char ansDir[BUFLEN];//���� ���丮
char errorDir[BUFLEN];//���� ���丮
char threadFiles[ARGNUM][FILELEN];//t�ɼ��� ����ϱ� ���� �迭
char iIDs[ARGNUM][FILELEN];//i�ɼ��� ����ϱ� ���� �迭

int eOption = false;//e�ɼ� flag
int tOption = false;//t�ɼ� flag
int mOption = false;//m�ɼ� flag
int iOption = false;//i�ɼ� flag
int first_i = false;//i�ɼ��� ó������ �������� üũ

void ssu_score(int argc, char *argv[])
{//�����Լ�
	int i;
	char saved_path[BUFLEN];//ó�� �۾� ���丮 ����
	char *score="score_table.csv";

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-h")){//h�ɼ� ����
			print_usage();//print usage�� �����ϰ� ����
			return;
		}
	}

	if(argc >= 3 && strcmp(argv[1], "-i") != 0){
		strcpy(stuDir, argv[1]);
		strcpy(ansDir, argv[2]);
	}//ó�����ڰ� i�ɼ��� �ƴҰ�� ���� ��ΰ� �����Ƿ� ��θ� �����Ͽ� ����

	if(!strcmp(argv[1],"-i"))//ó���� i���ڸ� ������ ��θ� �̻��ϰ� �� �� �����Ƿ� ���� üũ
		first_i=true;

	if(!check_option(argc, argv))//� �ɼ����� �����ߴ��� üũ
		exit(1);

	if(first_i){//i�ɼ��� ������ i�ɼ� ����
		do_iOption(iIDs);
		return;
	}

	getcwd(saved_path, BUFLEN);//ó�� �۾� ���丮 ����

	if(chdir(stuDir) < 0){//�л� ���丮�� �ִ��� üũ
		fprintf(stderr, "ssu_score studir %s doesn't exist\n", stuDir);
		return;
	}
	if(getcwd(stuDir, BUFLEN)==NULL){
		fprintf(stderr,"can't use %s\n",stuDir);
		return;
	}//�л� ���丮 �۾� �������� üũ

	chdir(saved_path);
	if(chdir(ansDir) < 0){//���� ���丮�� �ִ��� üũ
		fprintf(stderr, "ssu_score ansdir %s doesn't exist\n", ansDir);
		return;
	}
	if(getcwd(ansDir, BUFLEN)==NULL){
		fprintf(stderr,"can't use %s\n",ansDir);
		return;
	}//���� ���丮 �۾� �������� üũ

	chdir(saved_path);

	set_scoreTable(ansDir);//���ھ� ���̺� �ۼ�
	set_idTable(stuDir);//id���̺� �ۼ�

	if(mOption)
		do_mOption(score);

	printf("grading student's test papers..\n");
	score_students();//�л� ���� �ű�� ����

	if(iOption)//i�ɼ� �÷��� üũ
		do_iOption(iIDs);//i�ɼ� ����

	return;
}

int check_option(int argc, char *argv[])
{//� �ɼ����� �����ߴ��� üũ
	int i, j;
	int c;//�ɼǸ� ����

	while((c = getopt(argc, argv, "e:tmi")) != -1)
	{//e,m,t,i�ɼ� ��� ����
		switch(c){
			case 'e'://e�ɼ����� ����
				eOption = true;//�÷��� ��
				strcpy(errorDir, optarg);//���� ���丮 ��� ����

				if(access(errorDir, F_OK) < 0)
					mkdir(errorDir, 0755);//���丮�� ������ ����
				else{
					rmdirs(errorDir);
					mkdir(errorDir, 0755);
				}//���丮�� �����Ƿ� �����ϰ� ���θ���
				break;
			case 't'://t�ɼ����� ����
				tOption = true;//t�ɼ��÷��� ��
				i = optind;//argv�� ���� ��ġ�� Ȯ���ϱ� ���� ���
				j = 0;//�Ѱ��� ������ ���� üũ

				while(i < argc && argv[i][0] != '-'){//�ٸ� �ɼ��� �����ų� ������ üũ

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);//�ִ� ��� ���� ������ ����
					else
						strcpy(threadFiles[j], argv[i]);//t�ɼ� ���ڷ� �ֱ����� ����
					i++;//���� argv üũ
					j++;//�Ѱ��� t�ɼ� ������ ����
				}
				break;
			case 'm'://m�ɼ� ���� �����ߴ��� üũ
				mOption = true;//m�ɼ� �÷��� ��
				break;
			case 'i'://i�ɼ� ���� �����ߴ��� üũ
				iOption = true;//i�ɼ� �÷��� ��
				i = optind;//argv�� ���� ��ġ�� Ȯ���ϱ� ���� ���
				j = 0;//i�ɼ����� ����ϱ����� �Ѱ��� ������ ����

				while(i < argc && argv[i][0] != '-'){//���� ���ڰ� �ɼ��̰ų� ������ ��� �� �˻��Ҷ� ���� ����

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);//�ִ� ������ ������ ����
					else
						strcpy(iIDs[j], argv[i]);//i�ɼ��� �ֱ����� �л� ��ȣ�� i�迭�� ��������
					i++;//���� ���ڸ� �˻��ϱ� ���� ����
					j++;//i�迭 ������ ������ �����ϱ� ���� ����
				}
				break;
			case '?'://�̻��� �ɼ��� ������ ���
				printf("Unkown option %c\n", optopt);//�̻��� �ɼ��� �������Ƿ� false ����
				return false;
		}
	}

	return true;//�̻���� ��� �ɼ��� ���������Ƿ� true ����
}


void do_iOption(char (*ids)[FILELEN])
{//i�ɼ��� ����ϱ� ���� �Լ�
	FILE *fp;
	char tmp[BUFLEN];
	char save_qname[QNUM][BUFLEN];
	int i = 0;
	char *p, *saved;

	if((fp = fopen("score.csv", "r")) == NULL){//ä�� ��� ������ �ִ��� üũ
		fprintf(stderr, "file open error for score.csv\n");//ä�� ������ �����Ƿ� �����Ŵ
		return;
	}

	//���� ��ȣ �迭�� ����
	fscanf(fp, "%s\n", tmp);//���̺� ù ���� �о��
	p = strtok(tmp, ",");//ù �� ù��° ���� ������Ŵ
	strcpy(save_qname[i++],p);//ù��° ������ ����
	while((p = strtok(NULL, ",")) != NULL){//���ڿ��� ������ ,�� �������� �ڸ�
		if(strcmp(p,"sum"))
			strcpy(save_qname[i++],p);//���ʴ�� ���� �̸� ����
	}


	while(fscanf(fp, "%s\n", tmp) != EOF)
	{//���̺��� ��� row�� üũ
		p = strtok(tmp, ",");//tmp�� �ִ� ��Ʈ������ ,�� �������� �ڸ�

		if(!is_exist(ids, tmp))//��ġ�ϴ°� ���ڷ� ���°� �ִ��� Ȯ��
			continue;//������� �������� �Ѿ

		printf("%s's wrong answer : \n", tmp);

		i=0;
		while((p = strtok(NULL, ",")) != NULL){//���ڿ��� ������ ,�� �������� �ڸ�
			if(!strcmp(p,"0")){//0���� ��� ������ȣ ���
				printf("%s ", save_qname[i]);
			}
			i++;
		}
		printf("\n");
	}
	fclose(fp);//�˻縦 �������Ƿ� ����
}

void do_mOption(char *filename)
{//m�ɼ� ����� ���� �Լ�
	char tmp[BUFLEN];
	char qname[FILELEN];
	int fd;
	int i;
	double new_score;
	int size=sizeof(score_table) / sizeof(score_table[0]);//���̺� �ִ� ��

	if((fd = open(filename, O_RDWR | O_TRUNC)) < 0){//score_table.csv�� 0666�������� OPEN
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	while(1){
		printf("Input question's number to modify >> ");
		scanf("%s",tmp);
		if(!strcmp(tmp,"no"))
			break;

		for(i = 0; i < size; i++)
		{//���ھ� ���̺� �Է¹��� ���� �ֳ� üũ
			if(score_table[i].score == 0){
				printf("can't find %s\n",tmp);
				break;//�Է¹��� ������ȣ�� ã�� ����
			}

			strcpy(qname,score_table[i].qname);
			strcpy(qname,strtok(qname,"."));

			if(!strcmp(qname,tmp)){
				printf("Current score : %.2lf\n",score_table[i].score);//���� ���� ���
				printf("New score : ");
				scanf("%lf",&new_score);//���ο� ������ �Է¹���
				score_table[i].score=new_score;//���� ����
				break;
			}
		}
	}
	write_scoreTable(filename);//������ �����Ͽ����Ƿ� ���̺� �ٽ� ����
}

int is_exist(char (*src)[FILELEN], char *target)
{//�Է¹��� ���ڿ� �����ϴ��� Ȯ��
	int i = 0;

	while(1)
	{
		if(i >= ARGNUM)//�ִ� ���� ���� �Ѿ�� ��� ����
			return false;
		else if(!strcmp(src[i], ""))//�Է¹��� ���ڸ� �̹� �� �˻����� ��� ����
			return false;
		else if(!strcmp(src[i++], target))//Ÿ�ٰ� ��ġ�ϴ� ��츦 ã������� true ����
			return true;
	}
	return false;
}

void set_scoreTable(char *ansDir)
{//���ھ� ���̺� ���� �Լ�
	char filename[FILELEN];//���ϸ� ���� ����

	sprintf(filename, "%s", "score_table.csv");//���ھ� ���̺� �̸� ����

	if(access(filename, F_OK) == 0)
		read_scoreTable(filename);//score table�� �����Ұ�� �����͸� �о��
	else{//���ھ� ���̺��� �������
		make_scoreTable(ansDir);//���ھ� ���̺��� ����
		write_scoreTable(filename);//���ھ� ���̺��� �ۼ�
	}
}

void read_scoreTable(char *path)
{//���ھ� ���̺��� �о���� �Լ�
	FILE *fp;//���ھ� ���̺��� ������ ������
	char qname[FILELEN];//�ӽ÷� �������� ������ �迭
	char score[BUFLEN];//���ھ ����
	int idx = 0;

	if((fp = fopen(path, "r")) == NULL){//���ھ� ���̺��� ������ �ִ��� üũ
		fprintf(stderr, "file open error for %s\n", path);//���ھ� ���̺��� ���µ� ����
		return ;
	}

	while(fscanf(fp, "%[^,],%s\n", qname, score) != EOF){//���ھ� ���̺� ������ ������ �о��
		strcpy(score_table[idx].qname, qname);//���� �̸�
		score_table[idx++].score = atof(score);//������ ���ھ�
	}

	fclose(fp);//���ھ� ������ �� �о�������� ����
}

void make_scoreTable(char *ansDir)
{//���ھ� ���̺��� �����Ƿ� ����
	int type, num;
	double score, bscore, pscore;
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int idx = 0;
	int i;

	num = get_create_type();//� �ɼ����� ���ھ� ���̺��� ���鲫�� ����

	if(num == 1)
	{//1�� �ɼ����� ����
		printf("Input value of blank question : ");//��ĭ ���� ���� �Է�
		scanf("%lf", &bscore);
		printf("Input value of program question : ");//���α׷� ���� ���� �Է�
		scanf("%lf", &pscore);
	}

	if((dp = opendir(ansDir)) == NULL){//���� ���丮�� ���� �ִ��� üũ
		fprintf(stderr, "open dir error for %s\n", ansDir);//���н� �����޽���
		return;
	}

	while((dirp = readdir(dp)) != NULL)
	{//���丮�� �ִ� ������ �ϳ��� �о��
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;//. or ..�� ��� �н�

		if((type = get_file_type(dirp->d_name)) < 0)//���� Ÿ���� �������µ� �����ߴ��� üũ
			continue;

		sprintf(score_table[idx++].qname, "%s", dirp->d_name);//tmp�� ���� ���� �̸� ����
	}

	closedir(dp);//�� ����Ͽ����Ƿ� dp�� ����
	sort_scoreTable(idx);//���ھ� ���̺� ����

	for(i = 0; i < idx; i++)
	{//���� ���� ���ھ �Ҵ�
		type = get_file_type(score_table[i].qname);//��ĭ �������� ���α׷� �������� ����

		if(num == 1)
		{
			if(type == TEXTFILE)
				score = bscore;//�ؽ�Ʈ ������ ��� ��ĭ���� ���� �Ҵ�
			else if(type == CFILE)
				score = pscore;//c������ ��� ���α׷� ���� ���� �Ҵ�
		}
		else if(num == 2)
		{
			printf("Input of %s: ", score_table[i].qname);
			scanf("%lf", &score);//������ �Է¹޾� �Ҵ�
		}

		score_table[i].score = score;//i��° ���� ������ score�� ����
	}
}

void write_scoreTable(char *filename)
{//�Է¹��� ������ ���ھ� ���̺� �ۼ�
	int fd;
	char tmp[BUFLEN];
	int i;
	int num = sizeof(score_table) / sizeof(score_table[0]);//���̺� �ִ� ��

	if((fd = open(filename, O_RDWR | O_TRUNC | O_CREAT,0666)) < 0){//score_table.csv�� 0666�������� OPEN
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	for(i = 0; i < num; i++)
	{//���ھ� ���̺� ��ü üũ
		if(score_table[i].score == 0)
			break;

		sprintf(tmp, "%s,%.2f\n", score_table[i].qname, score_table[i].score);//���̺��� �����̸�, ���ھ�� �ۼ�
		write(fd, tmp, strlen(tmp));//���Ͽ� tmp���̸�ŭ tmp�� �ۼ� - 1 row�� �ۼ�
	}

	close(fd);//���� �ۼ��� ���������Ƿ� ����
}


void set_idTable(char *stuDir)
{//id���̺� ���� �Լ�
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int num = 0;

	if((dp = opendir(stuDir)) == NULL){//�Է¹��� �л� �� ���丮�� �����Ҽ� �ִ��� üũ
		fprintf(stderr, "opendir error for %s\n", stuDir);//�����ϴµ� ���� ���� ��� ����
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){//dirp�� dp�� �ִ� ������ ���� �ϳ��� �ҷ���
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;//. �� ..�� ����

		sprintf(tmp, "%s/%s", stuDir, dirp->d_name);//stuDir�� �ִ� ���丮 ���� tmp�� ����
		stat(tmp, &statbuf);//������ ���������� ��ȸ�ߴ��� üũ

		if(S_ISDIR(statbuf.st_mode))//�޾ƿ� ������ ���丮 ���� üũ
			strcpy(id_table[num++], dirp->d_name);//�޾ƿ� ������ ���丮 �� ����
		else
			continue;
	}

	sort_idTable(num);//id���̺� ����
}

void sort_idTable(int size)
{//id���̺� ���� �Լ�
	int i, j;
	char tmp[10];//���̵� �ӽ÷� ������ ���� �迭

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 -i; j++){//id�� �������� ������������ �������
			if(strcmp(id_table[j], id_table[j+1]) > 0){//�տ� id���� �ڿ� id������ Ŭ��� ����
				strcpy(tmp, id_table[j]);
				strcpy(id_table[j], id_table[j+1]);
				strcpy(id_table[j+1], tmp);
			}
		}
	}
}

void sort_scoreTable(int size)
{//���ھ� ���̺� �����Լ�
	int i, j;
	struct ssu_scoreTable tmp;//�ӽ÷� ���� ������ ���� ����ü
	int num1_1, num1_2;
	int num2_1, num2_2;

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 - i; j++){//qname�� �������� �������� �������

			get_qname_number(score_table[j].qname, &num1_1, &num1_2);//���� ù��° �� ������
			get_qname_number(score_table[j+1].qname, &num2_1, &num2_2);//���� �ι�° �� ������


			if((num1_1 > num2_1) || ((num1_1 == num2_1) && (num1_2 > num2_2))){//�տ� ���ڰ� ũ�ų� �տ����ڰ� ������ �ڿ����ڰ� Ŭ��� ����

				memcpy(&tmp, &score_table[j], sizeof(score_table[0]));
				memcpy(&score_table[j], &score_table[j+1], sizeof(score_table[0]));
				memcpy(&score_table[j+1], &tmp, sizeof(score_table[0]));
			}
		}
	}
}

void get_qname_number(char *qname, int *num1, int *num2)
{//qname�� �������� �Լ�
	char *p;
	char dup[FILELEN];

	strncpy(dup, qname, strlen(qname));//qname�� ���̸�ŭ qname�� dup�� �����ؿ� ����
	*num1 = atoi(strtok(dup, "-."));//-.�� �տ��� ã�Ƽ� �� �տ��� ��Ʈ������ ��ȯ�Ͽ� ����

	p = strtok(NULL, "-.");//�� ���� -.�� ã�Ƽ� ����
	if(p == NULL)
		*num2 = 0;//ã�� ���� ������� 0���� ����
	else
		*num2 = atoi(p);//���� ��� ��Ʈ������ ��ȯ�� ����
}

int get_create_type()
{//���ھ� ���̺��� ��� �������� ���� �Լ�
	int num;

	while(1)
	{
		printf("score_table.csv file doesn't exist in TREUDIR!\n");
		printf("1. input blank question and program question's score. ex) 0.5 1\n");//��ĭ����, ���α׷� ������ ���� ��ü ���ھ ����
		printf("2. input all question's score. ex) Input value of 1-1: 0.1\n");//������ ���ھ� ����
		printf("select type >> ");
		scanf(" %d", &num);

		if(num != 1 && num != 2)//�־��� �ɼ��� �ƴ� �ٸ� ��츦 �Է�������
			printf("not correct number!\n");
		else
			break;
	}

	return num;//�Է� ���� �� ��ȯ
}

void score_students()
{//�л������� ���ھ �ű�� �Լ�
	int num;//�ݺ��� ����
	int fd;//score.csv�� ������ ����
	char tmp[BUFLEN];
	double score=0;
	int size = sizeof(id_table) / sizeof(id_table[0]);//�ִ� ������

	if((fd = creat("score.csv", 0666)) < 0){//0666�������� score.csv������ ����
		fprintf(stderr, "creat error for score.csv");//score ������ ����µ� ����
		return;
	}
	write_first_row(fd);//ù��° row�� �ۼ��ϴ� �Լ�

	for(num = 0; num < size; num++)
	{
		if(!strcmp(id_table[num], ""))
			break;//���̵� ���̺��� ����� ��� �� �ڴ� ���°��̹Ƿ� ����

		sprintf(tmp, "%s,", id_table[num]);//tmp�� �й�,����
		write(fd, tmp, strlen(tmp));//ù��° Į���� ���� �� ���̹Ƿ� tmp���̸�ŭ tmp�� ���̺� ��

		score+=score_student(fd, id_table[num]);//sum���� ������ ���� ������ �����ϱ� ���� score_student ����
	}

	printf("Total average : %.2f\n", score / num);
	close(fd);
}

double score_student(int fd, char *id)
{//�л� ��� ä�� �Լ�
	int type;
	double result;
	double score = 0;
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);//�ִ� ������

	for(i = 0; i < size ; i++)
	{
		if(score_table[i].score == 0)
			break;//���ھ� ���̺� ���ھ ������� ����� ���� �� �̹Ƿ� ����

		sprintf(tmp, "%s/%s/%s", stuDir, id, score_table[i].qname);//�л� ��� ���丮�� �� �й��� �л� �� ��θ� tmp�� ����

		if(access(tmp, F_OK) < 0)//�л� ��ȿ� ���� �������� üũ
			result = false;//���� �������� ��� false
		else
		{
			if((type = get_file_type(score_table[i].qname)) < 0)//���� ������ ���α׷� �������� ��ĭ �������� üũ
				continue;//�Ѵ� �ƴϹǷ� �ѱ�

			if(type == TEXTFILE)
				result = score_blank(id, score_table[i].qname);//�ؽ�Ʈ ������ ��� ��ĭ ���� ä�� ����
			else if(type == CFILE)
				result = score_program(id, score_table[i].qname);//���α׷� ������ ��� ���α׷� ä�� ����
		}

		if(result == false)//������ Ʋ�����Ƿ� ����
			write(fd, "0,", 2);//0�� ó��
		else{
			if(result == true){//���� �̹Ƿ� ���ھ ������
				score += score_table[i].score;//������ ���ϱ� ���� score�� ���� ���ھ� �ջ�
				sprintf(tmp, "%.2f,", score_table[i].score);//���̺� �������� tmp�� ���ھ� ����
			}
			else if(result < 0){//������ �¾����� ��Ȯ�ϰ� ���� �ʾ����Ƿ� �׸�ŭ �����ϰ� �ջ�
				score = score + score_table[i].score + result;//������ ���ϱ����� ���ھ�+���� ���� ����+result�� ���
				sprintf(tmp, "%.2f,", score_table[i].score + result);//tmp�� �������ھ� + result���� ��
			}
			write(fd, tmp, strlen(tmp));//fd�� tmp�� ���̸�ŭ tmp �ۼ�
		}
	}

	printf("%s is finished.. score : %.2f\n", id, score);//id ä�� ����

	sprintf(tmp, "%.2f\n", score);//������ �������� tmp �� score ����
	write(fd, tmp, strlen(tmp));//������ fd�� tmp���̸�ŭ tmp�� ��

	return score;
}

void write_first_row(int fd)
{//ù��° row�� �ۼ�
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);

	write(fd, ",", 1);//ó�� ���Ҵ� ��ĭ

	for(i = 0; i < size; i++){
		if(score_table[i].score == 0)
			break;//���ھ 0���� ��� ä�� ������ �����Ƿ� ����

		sprintf(tmp, "%s,", score_table[i].qname);//tmp�� ���ھ� ���̺� �ִ� ���� ���� �̸���,�� ����
		write(fd, tmp, strlen(tmp));//���Ͽ� tmp�� ���̸�ŭ tmp�� ��
	}
	write(fd, "sum\n", 4);//���հ� ���� �ο�� �ѱ�� ���� \n����
}

char *get_answer(int fd, char *result)
{//���� �������� �Լ�
	char c;
	int idx = 0;

	memset(result, 0, BUFLEN);//result�� BUFLEN��ŭ 0���� ä��
	while(read(fd, &c, 1) > 0)//fd���� �ѱ��� �� EOF�� �����ų� ������ �߻��Ҷ� ���� �о�ͼ� c�� ����
	{
		if(c == ':')
			break;//c�� : �ϰ�� Ż��

		result[idx++] = c;//�ѱ��ھ� result�� ����
	}
	if(result[strlen(result) - 1] == '\n')
		result[strlen(result) - 1] = '\0';//���� '\n'�ϰ�� �ι��ڷ� �����Ͽ� ���� �˸�

	return result;
}

int score_blank(char *id, char *filename)
{//��ĭ ���� ä÷
	char tokens[TOKEN_CNT][MINLEN];
	node *std_root = NULL, *ans_root = NULL;
	int idx, start;
	char tmp[BUFLEN];//�ӽ� ��Ʈ�� ���� �迭
	char s_answer[BUFLEN], a_answer[BUFLEN];
	char qname[FILELEN];
	int fd_std, fd_ans;
	int result = true;
	int has_semicolon = false;

	memset(qname, 0, sizeof(qname));//qname ������ �����ŭ 0���� ����
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname�� Ȯ������ ���� ��ŭ �� filename�� ����

	sprintf(tmp, "%s/%s/%s", stuDir, id, filename);//tmp�� �л����丮/id/���ϸ��� �����Ͽ� ����
	fd_std = open(tmp, O_RDONLY);//fd_std�� �л� ���� ������ �б� �������� open
	strcpy(s_answer, get_answer(fd_std, s_answer));//�л� ������ �о�ͼ� s_answer�� ����

	if(!strcmp(s_answer, "")){//������ ""�� ���Ͽ� ���� ���°�� 0�� ó��
		close(fd_std);//������ �л��� ������ �ݰ� false ����
		return false;
	}

	if(!check_brackets(s_answer)){//()�� üũ
		close(fd_std);//��ȣ�� ������ �ٸ��Ƿ� Ʋ�� ���� �л� �� ������ �ݰ� false ����
		return false;
	}

	strcpy(s_answer, ltrim(rtrim(s_answer)));//s_answer�� ���� ������ ������ �����Ͽ� s_answer�� �ٽ� ����

	if(s_answer[strlen(s_answer) - 1] == ';'){//�л� ���� �� ������ ���� ;�ϰ��
		has_semicolon = true;//�����ݷ� �÷��� Ʈ��� ����
		s_answer[strlen(s_answer) - 1] = '\0';//�� ���ڷ� ����
	}

	if(!make_tokens(s_answer, tokens)){//s_answer�� ��ūȭ
		close(fd_std);//�������� ��� fd_std�� �ݰ� false ����
		return false;
	}

	idx = 0;
	std_root = make_tree(std_root, tokens, &idx, 0);//tokens�� Ʈ��ȭ

	sprintf(tmp, "%s/%s", ansDir, filename);
	fd_ans = open(tmp, O_RDONLY);//qname�� �ش��ϴ� ���� ������ open

	while(1)
	{//:�� �����Ͽ� ���ε��� üũ�ϱ����� �ݺ���Ŵ
		ans_root = NULL;
		result = true;

		for(idx = 0; idx < TOKEN_CNT; idx++)
			memset(tokens[idx], 0, sizeof(tokens[idx]));//������ ��ūȭ ��Ű������ ��ū �ʱ�ȭ

		strcpy(a_answer, get_answer(fd_ans, a_answer));//a_answer�� ���� ����

		if(!strcmp(a_answer, ""))//�о�� ���� ������� Ż��
			break;

		strcpy(a_answer, ltrim(rtrim(a_answer)));//a_answer�� ���� ������ ���� ����

		if(has_semicolon == false){
			if(a_answer[strlen(a_answer) -1] == ';')//has_semicolon�� false�϶� ������ ������ ���ڰ� ;�ΰ�� ���� ������
				continue;
		}

		else if(has_semicolon == true)
		{
			if(a_answer[strlen(a_answer) - 1] != ';')//has_semicolon�� true�϶� ������ ������ ���ڰ� ;�� �ƴѰ�� ���� ������
				continue;
			else
				a_answer[strlen(a_answer) - 1] = '\0';//;�� ��� �ι��ڷ� ����
		}

		if(!make_tokens(a_answer, tokens))
			continue;//a_answer�� ��ūȭ �����Ͽ��� ��� ���� ������ �Ѿ

		idx = 0;
		ans_root = make_tree(ans_root, tokens, &idx, 0);//��ū�� Ʈ��ȭ ��Ŵ

		compare_tree(std_root, ans_root, &result);//�л� ��� ���� Ʈ���� ��

		if(result == true){//�¾��� ���
			close(fd_std);
			close(fd_ans);

			if(std_root != NULL)
				free_node(std_root);
			if(ans_root != NULL)
				free_node(ans_root);
			return true;//�Ҵ��� �޸𸮸� �����ϰ� true ��ȯ

		}
	}

	close(fd_std);
	close(fd_ans);

	if(std_root != NULL)
		free_node(std_root);
	if(ans_root != NULL)
		free_node(ans_root);

	return false;//�Ҵ��� �޸𸮸� �����ϰ� false ��ȯ
}

double score_program(char *id, char *filename)
{//���α׷� ä��
	double compile;
	int result;

	compile = compile_program(id, filename);//�л� ���̵� �ش��ϴ� ������ ������

	if(compile == ERROR || compile == false)
		return false;//������ ������ ������ false ����

	result = execute_program(id, filename);//���� ������ ����� �޾ƿ�

	if(!result)
		return false;//������ Ʋ���� ��� false����

	if(compile < 0)
		return compile;//warning�� ������� �Ѱܹ����� ����

	return true;
}

int is_thread(char *qname)
{//t�ɼǿ� �ش��ϴ��� üũ
	int i;
	int size = sizeof(threadFiles) / sizeof(threadFiles[0]);//�ִ� ����

	for(i = 0; i < size; i++){
		if(!strcmp(threadFiles[i], qname))
			return true;//threadFiles�� qname�� �ִ��� �˻� ������� true ����
	}
	return false;
}

double compile_program(char *id, char *filename)
{//���α׷� ������ �Լ�
	int fd;
	char tmp_f[BUFLEN], tmp_e[BUFLEN];
	char command[BUFLEN];
	char qname[FILELEN];
	int isthread;
	off_t size;
	double result;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname�� �ʱ�ȭ ��Ų�� Ȯ���ڸ� ������ �����̸��� qname�� ����

	isthread = is_thread(qname);//thread flag üũ

	sprintf(tmp_f, "%s/%s", ansDir, filename);//�������� ���� ���� ���
	sprintf(tmp_e, "%s/%s.exe", ansDir, qname);//�������ؼ� ���� ���� ���� �̸�

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);//t�ɼ��� true�� ���
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);//t�ɼ��� false�� ���

	sprintf(tmp_e, "%s/%s_error.txt", ansDir, qname);
	fd = creat(tmp_e, 0666);//���� ������ ���� �ؽ�Ʈ ����

	redirection(command, fd, STDERR);//���� üũ
	size = lseek(fd, 0, SEEK_END);//���� ������ ũ��
	close(fd);
	unlink(tmp_e);//�����ߴ� ������ ����

	if(size > 0)
		return false;//������ �����Ƿ� ����

	sprintf(tmp_f, "%s/%s/%s", stuDir, id, filename);//�л� �� ���ϸ�
	sprintf(tmp_e, "%s/%s/%s.stdexe", stuDir, id, qname);//�л� �� ���� ����

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);//t�ɼ��� ������
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);//t�ɼ��� ������

	sprintf(tmp_f, "%s/%s/%s_error.txt", stuDir, id, qname);
	fd = creat(tmp_f, 0666);//���� ���� ����

	redirection(command, fd, STDERR);
	size = lseek(fd, 0, SEEK_END);//�������� �ִ��� üũ
	close(fd);

	if(size > 0){//���� ������ �ִ°��
		if(eOption)
		{//e�ɼ� �����
			sprintf(tmp_e, "%s/%s", errorDir, id);//���� ���丮 ���
			if(access(tmp_e, F_OK) < 0)
				mkdir(tmp_e, 0755);//0755�������� ���丮 ����

			sprintf(tmp_e, "%s/%s/%s_error.txt", errorDir, id, qname);//���� ���� ��� ����
			rename(tmp_f, tmp_e);//tmp_e�� tmp_f�� �ٲ�

			result = check_error_warning(tmp_e);//���� üũ
		}
		else{
			result = check_error_warning(tmp_f);//���� üũ
			unlink(tmp_f);//tmp_f���� ����
		}

		return result;//���� ī��Ʈ�� ������ �ִ��� ����
	}

	unlink(tmp_f);//���� ������ true ����
	return true;
}

double check_error_warning(char *filename)
{//error�� warning�� üũ
	FILE *fp;
	char tmp[BUFLEN];
	double warning = 0;

	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", filename);
		return false;
	}//������ r�������� ��

	while(fscanf(fp, "%s", tmp) > 0){
		if(!strcmp(tmp, "error:"))
			return ERROR;//tmp�� error�� �����ϴ��� üũ
		else if(!strcmp(tmp, "warning:"))
			warning += WARNING;//���Ͽ��� �о�� ��Ʈ���� warning�� ���������� ī��Ʈ ����
	}

	return warning;//���� ī��Ʈ ����
}

int execute_program(char *id, char *filename)
{//���α׷� ������
	char std_fname[BUFLEN], ans_fname[BUFLEN];
	char tmp[BUFLEN];
	char qname[FILELEN];
	time_t start, end;
	pid_t pid;
	int fd;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname�� �ʱ�ȭ ��Ű�� Ȯ���ڸ� �����Ѱ� ����

	sprintf(ans_fname, "%s/%s.stdout", ansDir, qname);
	fd = creat(ans_fname, 0666);//���� ���� ��� ������ ���� ����

	sprintf(tmp, "%s/%s.exe", ansDir, qname);//���� ���� command �Ѱ���
	redirection(tmp, fd, STDOUT);//���� ���� �Ͽ� ��� fd�� ����
	close(fd);

	sprintf(std_fname, "%s/%s/%s.stdout", stuDir, id, qname);
	fd = creat(std_fname, 0666);//�л� �� ���� ��� ���� ���� ����

	sprintf(tmp, "%s/%s/%s.stdexe &", stuDir, id, qname);//�л� ���� ���� ��ɾ� ����

	start = time(NULL);//���� �ð� üũ
	redirection(tmp, fd, STDOUT);

	sprintf(tmp, "%s.stdexe", qname);
	while((pid = inBackground(tmp)) > 0){//�ð� üũ�� �ϱ����� ��׶��忡�� ����
		end = time(NULL);

		if(difftime(end, start) > OVER){//5�ʸ� �Ѿ����� ���α׷� ���� ����
			kill(pid, SIGKILL);
			close(fd);
			return false;
		}
	}

	close(fd);//���� ����

	return compare_resultfile(std_fname, ans_fname);//�л� �� �������ϰ� ���� �������� �� �� ����
}

pid_t inBackground(char *name)
{//��׶��忡�� �����ϴ� �Լ�
	pid_t pid;
	char command[64];
	char tmp[64];
	int fd;
	off_t size;

	memset(tmp, 0, sizeof(tmp));
	fd = open("background.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);//backgroun.txt�� �б� ����, ����, ������� �ʱ�ȭ 0666�������� open

	sprintf(command, "ps | grep %s", name);
	redirection(command, fd, STDOUT);//name�� �� ���μ��� üũ

	lseek(fd, 0, SEEK_SET);
	read(fd, tmp, sizeof(tmp));//tmp�� �����ŭ ������� �о��

	if(!strcmp(tmp, "")){
		unlink("background.txt");
		close(fd);
		return 0;
	}//tmp�� ���� ������� backgroun.txt�� ����� ������ ����

	pid = atoi(strtok(tmp, " "));//������ ã�� �ڸ��� ��Ʈ�� ����
	close(fd);

	unlink("background.txt");
	return pid;//��� ������ ã�� pid ����
}

int compare_resultfile(char *file1, char *file2)
{//��� ���� ��
	int fd1, fd2;
	char c1, c2;
	int len1, len2;

	fd1 = open(file1, O_RDONLY);//ù��° ������ �б����� �������� ��
	fd2 = open(file2, O_RDONLY);//�ι�° ������ �б����� �������� ��

	while(1)//������ ����� ������ üũ
	{
		while((len1 = read(fd1, &c1, 1)) > 0){
			if(c1 == ' ')
				continue;//��ĭ�� �ǳ� ��
			else
				break;//�ƴҰ�� ���ϱ����� Ż��
		}
		while((len2 = read(fd2, &c2, 1)) > 0){
			if(c2 == ' ')
				continue;//��ĭ�� �ǳ� ��
			else
				break;//�ƴҰ�� ���ϱ����� Ż��
		}

		if(len1 == 0 && len2 == 0)//�� �� �о���� �� ������ ��� Ż��
			break;

		to_lower_case(&c1);//file1���� �о�� ���ڸ� �ҹ��ڷ� ġȯ
		to_lower_case(&c2);//file1���� �о�� ���ڸ� �ҹ��ڷ� ġȯ

		if(c1 != c2){//���� Ʋ�����Ƿ� ������ ������ �ݰ� false ����
			close(fd1);
			close(fd2);
			return false;
		}
	}
	close(fd1);
	close(fd2);
	return true;//���� ��ġ�ϹǷ� ������ ������ �ݰ� true ����
}

void redirection(char *command, int new, int old)
{//�翬�� �Լ�
	int saved;

	saved = dup(old);//saved�� old�� ���
	dup2(new, old);//old�� new�� ���

	system(command);//command ����

	dup2(saved, old);//old�� saved�� �ٲ㼭 ������ ����ϴ� old������ �ٽ� ������
	close(saved);//saved����� ����
}

int get_file_type(char *filename)//���� Ÿ���� ������
{
	char *extension = strrchr(filename, '.');//������ .��ġ�� ���Ϲ޾� ����

	if(!strcmp(extension, ".txt"))
		return TEXTFILE;//.txt ������ ��� TEXTFILE(3) ����
	else if (!strcmp(extension, ".c"))
		return CFILE;//.c ������ ��� CFILE(4) ����
	else
		return -1;//�Ѵ� �ƴϹǷ� -1����
}

void rmdirs(const char *path)//���丮 ����
{
	struct dirent *dirp;//readdir�� ������ ������ ����
	struct stat statbuf;//�о�� ������ ���� �� ������ ������ ����ü
	DIR *dp;//���� ���丮 ���� DIR
	char tmp[BUFLEN];//������ ��θ� �ӽ� ������ �迭

	if((dp = opendir(path)) == NULL)
		return;//���丮�� �������°Ϳ� ���� ����ó��

	while((dirp = readdir(dp)) != NULL)//���丮
	{
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))//.�̰ų� ..�ϰ�� �н�
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name);//tmp�� ���/���ϸ� ����

		if(lstat(tmp, &statbuf) == -1)//���� ������ �������µ� ����ó��
			continue;

		if(S_ISDIR(statbuf.st_mode))
			rmdirs(tmp);//���丮�ϰ�� rmdirs�� ����
		else
			unlink(tmp);//������ ��� unlink�� ���� ����
	}

	closedir(dp);//������ ���丮�� ����
	rmdir(path);//path����� ���丮 ����
}

void to_lower_case(char *c)//�빮�� �ҹ��ڷ� ġȯ
{
	if(*c >= 'A' && *c <= 'Z')
		*c = *c + 32;
}

void print_usage()//-h�ɼ� ���� ����
{
	printf("Usage : ssu_score <STUDENTDIR> <TRUEDIR> [OPTION]\n");
	printf("Option : \n");
	printf(" -m <IDS>          modify question's score\n");
	printf(" -e <DIRNAME>      print error on 'DIRNAME/ID/qname_error.txt' file \n");
	printf(" -t <QNAMES>       compile QNAME.C with -lpthread option\n");
	printf(" -i <IDS>          print student's wrong questions\n");
	printf(" -h                print usage\n");
}
