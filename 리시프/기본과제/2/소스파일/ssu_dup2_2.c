#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024

int main(void){
    char buf[BUFFER_SIZE];
    char *fname = "ssu_test.txt";
    int fd;
    int length;

    if((fd = open(fname,O_RDONLY,0644))<0){//SSU_TEST.TXT를 읽기 0644로 open
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }

    if(dup2(1,4) != 4){//4번을 표준 출력으로 복제
        fprintf(stderr,"dup2 call failed\n");
        exit(1);
    }

    while(1){
        length = read(fd,buf,BUFFER_SIZE);//SSU_TEST.TXT를 읽어옴

        if(length<=0)
            break;

        write(4,buf,length);//4번 디스크립터에 읽어온 파일을 write
    }
    exit(0);
}
