#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>

#define MAX_CALL 100
#define MAX_BUF 20000

void serror(char *str)
{
    fprintf(stderr, "%s\n",str);
    exit(1);
}

int main()
{
    int nread, nwrite, val, i=0;
    char *ptr;
    char buf[MAX_BUF];

    int call[MAX_CALL];

    nread = read(STDIN_FILENO,buf,sizeof(buf));//입력값으로 들어온 데이터를 buf에 저장
    fprintf(stderr,"read %d bytes\n",nread);

    if((val = fcntl(STDOUT_FILENO, F_GETFL,0))<0)//stdout의 flag를 가져옴
        serror("fcntl F_GETFL error");
    val |= O_NONBLOCK;//val에 O_NONBLOCK을 추가
    if(fcntl(STDOUT_FILENO, F_SETFL, val)<0)//val로 플래그 재설정
        serror("fcntl F_SETFL error");

    for(ptr = buf; nread > 0; i++){
        errno = 0;
        nwrite = write(STDOUT_FILENO,ptr,nread);//stdout에 입력받은 데이터를 출력

        if(i<MAX_CALL)
            call[i] = nwrite;

        fprintf(stderr,"nwrite = %d, errno = %d\n",nwrite,errno);
        if(nwrite>0){
            ptr+=nwrite;
            nread-=nwrite;
        }
    }

    if((val=fcntl(STDOUT_FILENO, F_GETFL,0))<0)//stdout의 플래그를 val에 저장
        serror("fcntl F_GETFL error");
    val &= ~O_NONBLOCK;//val에서 O_NONBLOCK을 제거
    if(fcntl(STDOUT_FILENO,F_SETFL,val)<0)//val로 flag 재설정
        serror("fcntl F_SETFL error");

    for(i = 0; i<MAX_CALL; i++)
        fprintf(stdout,"call[%d] = %d\n",i, call[i]);

    exit(0);
}
