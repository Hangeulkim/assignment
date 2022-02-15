#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(void){
    char c;
    int fd;

    if((fd=open("ssu_test.txt",O_RDONLY))<0){//ssu_test.txt를 읽기 권한으로 엶
        fprintf(stderr,"open error for %s\n","ssu_test.txt");
        exit(1);
    }
    
    while(1){//파일의 끝에 도달할때 까지 한글자씩 읽어와서 출력함
        if(read(fd,&c,1)>0)
            putchar(c);
        else
            break;
    }
    exit(0);
}
