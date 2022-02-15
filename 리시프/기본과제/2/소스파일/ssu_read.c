#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"ssu_employee.h"

int main(int argc, char *argv[]){//프로그램 실행시 인자를 전달받기 위함
    struct ssu_employee record;//employee를 보여주기 위한 구조체
    int fd;//파일 디스크립터
    int record_num;//저장할 번호

    if(argc<2){//열 파일의 인자를 주지않음
        fprintf(stderr,"Usage : %s file\n",argv[0]);
        exit(1);
    }

    if((fd=open(argv[1],O_RDONLY))<0){//1번에 있는 파일을 읽기 권한으로 엶
        fprintf(stderr,"open error for %s\n",argv[1]);
        exit(1);
    }

    while(1){
        printf("Enter record number : ");
        scanf("%d",&record_num);//읽어올 레코드 번호를 입력

        if(record_num < 0)//레코드 번호를 음수로 입력받을 경우 종료
            break;

        if(lseek(fd,(long)record_num *sizeof(record),0)<0){//레코드 수만큼 파일의 오프셋을  이동시킴
            fprintf(stderr,"lseek error\n");
            exit(1);
         }

        if(read(fd,(char*)&record,sizeof(record))>0)//레코드 위치의 자료를 읽어옴
            printf("Employee : %s Salary : %d\n",record.name,record.salary);
        else
            printf("Record %d not found\n",record_num);
    }
    close(fd);//열어본 파일을 닫음
    exit(0);
}

