#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

int main()
{
    int flag;

    if((flag=fcntl(STDOUT_FILENO,F_DUPFD))==-1){//인자가 부족하므로 에러 발생
        fprintf(stderr,"Error : Checking CLOSEE_ON_EXEC\n");
        exit(1);
    }

    printf("CLOSE ON EXEC flag is = %d\n",flag);
    exit(0);
}
