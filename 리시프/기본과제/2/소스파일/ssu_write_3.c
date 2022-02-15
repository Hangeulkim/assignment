#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"ssu_employee.h"

int main(int argc, char *argv[]){
    struct ssu_employee record;
    int fd;

    if(argc < 2){//저장할 파일 이름이 있는지 확인
        fprintf(stderr,"usage : %s file\n",argv[0]);
        exit(1);
    }

    if((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL,0640))<0){//1번에 파일을 쓰기,제작,이미 있을경우 실패함 0640으로 open
        fprintf(stderr,"open error for %s\n",argv[1]);
        exit(1);
    }

    while(1){
        printf("Enter employee name <SPACE> salary: ");
        scanf("%s",record.name);//이름을 입력받음

        if(record.name[0] == '.')//이름이 . 이면 종료
            break;

        scanf("%d",&record.salary);//salary를 입력받음
        record.pid=getpid();
        write(fd,(char*)&record,sizeof(record));//record를 fd에 저장
    }
    close(fd);//파일 종료
    exit(0);
}
