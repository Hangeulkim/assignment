#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(void){
    char buf[BUFFER_SIZE];
    int length;

    length = read(0,buf,BUFFER_SIZE);//버퍼에서 데이터를 읽어옴
    write(1,buf,length);//buf에서 length만큼 읽어와서 표준 출력으로 보냄
    exit(0);
}
