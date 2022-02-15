#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
//�ʿ��ϸ� header file �߰� ����
#define READ_BYTE 100
#define BUFFER_SIZE 1024
//
// argv[1]: ���ڵ� ���ϸ�
//
int main(int argc, char **argv)
{
	struct timeval start,end;
	char buf[BUFFER_SIZE];
	int fd;
	int size;
	int length;


	// ǥ���Է����� ���� ���ڵ� ���Ͽ� ����Ǿ� �ִ� ��ü ���ڵ带 "������"���� �о���̰�, �̶�
	// �ɸ��� �ð��� �����ϴ� �ڵ� ������
	fd=open(argv[1],O_RDONLY);
	size=lseek(fd,(off_t)0,SEEK_END);
	lseek(fd,(off_t)0,SEEK_SET);
	printf("#records: %d ",size/100);
	gettimeofday(&start,NULL);

	while((length=read(fd,buf,READ_BYTE))>0) ;

	gettimeofday(&end,NULL);

	printf("timecase: %ld us\n",end.tv_sec*1000000+end.tv_usec-start.tv_sec*1000000-start.tv_usec);
	exit(0);
}
