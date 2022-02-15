#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>// oflag사용을 위함
#include<sys/stat.h>//사용 하진 않으나 open을 위한 기본선언
#include<sys/types.h>//lseek을 위한 선언
#include<unistd.h>//lseek을 위한 선언

int main(void){
    char *file_name="ssu_test.txt";//오픈할 파일명
    off_t fsize;//파일의 오프셋
    int fd;//파일의 디스크립터 위치

    if((fd = open(file_name,O_RDONLY))<0){//파일을 여는데 실패했는지 판별
        fprintf(stderr,"open error for %s\n",file_name);//에러가 났을경우 에러 메시지 출력
        exit(1);
    }

    if((fsize = lseek(fd,(off_t)0,SEEK_END))<0){//파일의 크기를 확인하기 위함
        fprintf(stderr,"lseek\n");//에러 메시지
        exit(1);
    }

    printf("The size of <%s> is %ld bytes.\n",file_name,fsize);//파일의 이름 및 크기 출력

    exit(0);
}
