#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
int main(void)
{
    char buf[BUFFER_SIZE];

    while (fgets(buf, BUFFER_SIZE, stdin) != NULL)//stdin에서 한 줄씩 읽어와 buf에 저장
        if (fputs(buf, stdout) == EOF) {//eof일 경우 에러처리
        fprintf(stderr, "standard output error\n");
        exit(1);
        }
    if (ferror(stdin)) {//stdin의 에러 여부 확인
        fprintf(stderr, "standard input error\n");
        exit(1);
    }
    exit(0);
}
