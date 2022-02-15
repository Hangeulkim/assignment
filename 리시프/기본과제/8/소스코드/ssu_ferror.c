#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    FILE *fp;
    int character;
    if (argc != 2) {//인자가 올바르게 주어졌는지 확인
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "r")) == NULL) {//argv[1]을 r로 fopen
        fprintf(stderr, "fopen error for %s\n", argv[1]);
        exit(1);
    }
    character = fgetc(fp);//fp에서 한글자를 읽어와서 character에 저장

    while (!feof(fp)) {//fp의 eof에 도달했는지 확인
        fputc(character, stdout);//stdout에 character을 putc
        if (ferror(fp)) {//fp의 에러 확인
            fprintf(stderr, "Error detected!!\n");
            clearerr(fp);//eof 플래그를 클리어
        }
        character = fgetc(fp);//fp에서 한글자를 읽어와서 character에 저장
    }
    fclose(fp);//fp의 사용을 종료
    exit(0);
}
