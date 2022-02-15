#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

void set_flags(int fd, int flags);
void clr_flags(int fd, int flags);

char buf[500000];

int main()
{
    int ntowrite, nwrite;
    char *ptr;

    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));//stdin에서 데이터를 읽어옴
    fprintf(stderr, "reading %d bytes\n",ntowrite);

    set_flags(STDOUT_FILENO,O_NONBLOCK);//nonblocking 설정

    ptr = buf;
    while(ntowrite>0){
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);//stdout에 입력받은 데이터를 씀
        fprintf(stderr, "nwrite = %d, errno = %d\n",nwrite,errno);

        if(nwrite > 0){//읽어온 데이터가 있으면
            ptr+=nwrite;//다음데이터로 이동시키고
            ntowrite -= nwrite;//길이를 감소시킴
        }
    }
    clr_flags(STDOUT_FILENO,O_NONBLOCK);//nonblocking 해제

    exit(0);
}

void set_flags(int fd, int flags)
{
    int val;

    if((val=fcntl(fd,F_GETFL,0))<0){//fd에 설정된 플래그를 가져옴
        fprintf(stderr,"fcntl F_GETFL failed");
        exit(1);
    }

    val |= flags;//flags를 적용

    if(fcntl(fd,F_SETFL,val)<0){//변경한 플래그로 재설정
        fprintf(stderr,"fcntl F_SETFL failed");
        exit(1);
    }
}

void clr_flags(int fd, int flags)
{
    int val;

    if((val = fcntl(fd, F_GETFL,0))<0){//fd에 설정된 플래그를 가져옴
        fprintf(stderr,"fcntl F_GETFL failed");
        exit(1);
    }

    val &= ~flags;//flags를 사용하지 않도록 변경

    if(fcntl(fd, F_SETFL,val)<0){//변경된 플래그를 재설정
        fprintf(stderr,"fcntl F_SETFL failed");
        exit(1);
    }
}
