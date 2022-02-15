#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long fsize;
    if ((fp = fopen(fname, "r")) == NULL) {//fname을 r로 fopen
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);//fp의 오프셋을 맨 마지막으로 이동
    fsize = ftell(fp);//fp의 위치까지만큼의 바이트수를 fsize에 저장
    printf("The size of <%s> is %ld bytes\n", fname, fsize);//파일의 바이트수를 출력
    exit(0);
}
