#include<stdio.h>
#include<sys/types.h>
#include<time.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

//�ʿ��ϸ� header file �߰� ����

#define READ_BYTE 100
#define BUFFER_SIZE 1024
#define SUFFLE_NUM	10000	// �� ���� ������� ������ �� ����

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// �ʿ��� �Լ��� ������ �� �߰��� �� ����

//
// argv[1]: ���ڵ� ���ϸ�
//
int main(int argc, char **argv)
{
	struct timeval start,end;
	char buf[BUFFER_SIZE];
	int *read_order_list;
	int num_of_records;
	int size;
	int fd;
	int length;

	fd=open(argv[1],O_RDONLY);
	size=lseek(fd,(off_t)0,SEEK_END);
	num_of_records=size/100;
	read_order_list=(int*)calloc(num_of_records+1,sizeof(int));
	printf("#records: %d ",num_of_records);

	// �Ʒ� �Լ��� �����ϸ� 'read_order_list' �迭�� ���� �����ϰ� �о�� �� ���ڵ� ��ȣ���� ������� �����Ǿ� �����
            // 'num_of_records'�� ���ڵ� ���Ͽ� ����Ǿ� �ִ� ��ü ���ڵ��� ���� �ǹ���
	GenRecordSequence(read_order_list, num_of_records);


	// 'read_order_list'�� �̿��Ͽ� ǥ�� �Է����� ���� ���ڵ� ���Ϸκ��� ���ڵ带 random �ϰ� �о���̰�,
            // �̶� �ɸ��� �ð��� �����ϴ� �ڵ� ������

	gettimeofday(&start,NULL);

	for(int i=0;i<num_of_records;i++){
		lseek(fd,(off_t)read_order_list[i]*100,SEEK_SET);
		length=read(fd,buf,READ_BYTE);
	}

	gettimeofday(&end,NULL);

	printf("timecase: %ld us\n",end.tv_sec*1000000+end.tv_usec-start.tv_sec*1000000-start.tv_usec);
	exit(0);
}

void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}

	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}

	return;
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;

	return;
}
