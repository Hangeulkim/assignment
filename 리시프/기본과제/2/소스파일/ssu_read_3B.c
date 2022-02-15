#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(void){
    char character;
    int fd;
    int line_count = 0;

    if((fd = open("ssu_test.txt",O_RDONLY))<0){//SSU_TEST.TXT를 읽기 권한으로 엶
        fprintf(stderr,"open error for %s\n", "ssu_test.txt");
        exit(1);
    }

    while(1){
        if(read(fd,&character,1) > 0){//한글자를 읽어와서 chracter에 저장
            if(character == '\n')//읽어온 글자가 개행일경우 라인카운터 증가
                line_count++;
        }
        else
            break;
    }

    printf("Total line : %d\n",line_count);
    exit(0);
}
