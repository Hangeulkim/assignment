#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>// oflag사용을 위함
#include<sys/stat.h>//사용 하진 않으나 open을 위한 기본선언
#include<sys/types.h>//사용 하진 않으나 open을 위한 기본 선언

int main(void){
    char *file_name="ssu_test.txt";//오픈할 파일명
    int fd;//파일의 디스크립터 위치

    if((fd = open(file_name,O_RDONLY))<0){//파일을 여는데 실패했는지 판별
        fprintf(stderr,"open error for %S\n",file_name);//에러가 났을경우 에러 메시지 출력
        exit(1);
    }

    else{
        printf("Sucess!\nFilename : %s\nDescriptor : %d\n",file_name, fd);// 파일을 여는데 성공 했을경우 sucess와 파일명 디스크립터 출력
    }

    exit(0);
}
