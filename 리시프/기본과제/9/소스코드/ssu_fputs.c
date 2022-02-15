#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main (int argc, char *argv[])
{
    char buf[BUFFER_SIZE];
    FILE *fp;
    if (argc != 2) {//인자가 제대로 들어왔는지 확인
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "w+")) == NULL) {//argv[1]을 w+(읽고 쓰기, 내용초기화)로 fopen
        fprintf(stderr, "fopen error for %s\n", argv[1]);
        exit(1);
    }

    fputs("Input String >> ", stdout);//input string>>을 stdout에 출력
    gets(buf);//한줄을 읽어와 buf에 저장
    fputs(buf, fp);//buf를 fp에 씀
    rewind(fp);//fp의 위치를 처음으로 이동
    fgets(buf, sizeof(buf), fp);//fp에서 buf만큼 개행 문자나 eof를 만날때까지 읽어옴
    puts(buf);//buf를 출력
    fclose(fp);//fp사용을 종료
    exit(0);
}
