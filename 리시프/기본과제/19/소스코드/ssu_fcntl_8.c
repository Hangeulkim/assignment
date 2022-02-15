#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"ssu_employee.h"

#define DUMMY 0

int main(int argc, char *argv[])
{
    struct ssu_employee record;
    int fd;
    int flags;
    int length;
    int pid;

    if(argc<2){//인자가 제대로 주어졌는지 확인
        fprintf(stderr, "Usage : %s file\n", argv[0]);
        exit(1);
    }

    if((fd=open(argv[1],O_RDWR))<0){//인자로 들어온 파일을 읽고 쓰기로 open
        fprintf(stderr,"open error for %s\n",argv[1]);
        exit(1);
    }

    if((flags = fcntl(fd, F_GETFL,DUMMY))==-1){//fd의 파일 상태 플래그를읽어와 flags에 저장
        fprintf(stderr, "fcntl F_GETFL error\n");
        exit(1);
    }

    flags |= O_APPEND;//flags에 O_APPEND를 추가

    if(fcntl(fd,F_SETFL,flags)==-1){//fd의 파일 상태 플래그를 flags로 재설정
        fprintf(stderr, "fcntl F_SETFL error\n");
        exit(1);
    }

    pid=getpid();//현재 pid를 가져옴

    while(1){
        printf("Enter employee name : ");
        scanf("%s",record.name);//record.name을 입력받음

        if(record.name[0]=='.')//이름의 첫글자가 .이면 종료
            break;

        printf("Enter employee salary : ");
        scanf("%d",&record.salary);//salary를 입력받아 저장
        record.pid=pid;//현재 pid를 저장
        length = sizeof(record);

        if(write(fd, (char*)&record,length)!=length){
            fprintf(stderr,"record write error\n");
            exit(1);
        }//입력받은 record를 파일에 저장
    }
    close(fd);
    exit(0);
}
