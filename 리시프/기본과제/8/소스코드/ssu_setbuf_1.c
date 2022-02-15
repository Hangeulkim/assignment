#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
int main(void)
{
	char buf[BUFFER_SIZE];
	setbuf(stdout, buf);//buf를 bufsize만큼의 char 버퍼로 사용
	printf("Hello, ");	sleep(1);
	printf("OSLAB!!");	sleep(1);
	printf("\n");	sleep(1);

	setbuf(stdout, NULL);//버퍼 사용하지 않음
	
	printf("How");	sleep(1);
	printf(" are");	sleep(1);
	printf(" you?");	sleep(1);
	printf("\n");

	exit(0);
}
