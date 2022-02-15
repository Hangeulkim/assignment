#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int character;
    while ((character = getc(stdin)) != EOF)//stdin에서 EOF를 만날때까지 읽어와 character에 저장

    if (putc(character, stdout) == EOF) {//stdout에 character을 putc
        fprintf(stderr, "standard output error\n");
        exit(1);
    }
    if (ferror(stdin)) {//stdin의 에러 발생 여부 확인
        fprintf(stderr, "standard input error\n");
        exit(1);
    }
    exit(0);
}
