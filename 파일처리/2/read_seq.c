#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
//필요하면 header file 추가 가능
#define READ_BYTE 100
#define BUFFER_SIZE 1024
//
// argv[1]: 레코드 파일명
//
int main(int argc, char **argv)
{
	struct timeval start,end;
	char buf[BUFFER_SIZE];
	int fd;
	int size;
	int length;


	// 표준입력으로 받은 레코드 파일에 저장되어 있는 전체 레코드를 "순차적"으로 읽어들이고, 이때
	// 걸리는 시간을 측정하는 코드 구현함
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
