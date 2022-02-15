#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long position;
    int character;
    if ((fp = fopen(fname, "r")) == NULL) {//fname을 r로 fopen
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    printf("Input number >>");
    scanf("%ld", &position);//입력값을 position에 저장
    fseek(fp, position - 1, SEEK_SET);//fp의 오프셋을 position-1로 이동시킴
    character = getc(fp);//그곳에서 한글자를 읽어옴
    printf("%ldth character => %c\n", position, character);//position위치에서 읽어온 글자를 출력
    exit(0);
}
