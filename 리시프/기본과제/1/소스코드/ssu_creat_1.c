#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//열어본 파일을 닫기 위해
#include<sys/stat.h>//사용하진 않으나 creat을 위해 선언
#include<sys/types.h>//사용하진 않으나 creat을 위해 선언
#include<fcntl.h>//파일을 만들때 권한 설정을 위해 선언

int main(void){
    char *file_name="ssu_test.txt";//불러올 파일의 이름
    int fd;//불러온 파일의 디스크립터
    
    if((fd=creat(file_name,0666))<0){//파일이 0666권한으로 성공적으로 만들어 졌는지 판별
        fprintf(stderr,"creat error for %s\n",file_name);//에러 메시지
        exit(1);
    }
    else{
        printf("Sucess!\nFilename : %s\nDescriptor : %d\n",file_name,fd);//파일이 성공적으로 만들어짐
        close(fd);//만들어진 파일을 닫음
    }

    exit(0);
}
