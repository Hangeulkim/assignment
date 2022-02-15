#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void)
{
    char *fname = "ssu_test.dat";
    char name[BUFFER_SIZE];
    FILE *fp;
    int age;

    fp = fopen(fname, "r");//fname을 r로 fopen
    fscanf(fp, "%s%d", name, &age);//fp에서 %s%d형식으로 읽어옴
    fclose(fp);//fp사용 종료
    fp = fopen(fname, "w");//fname을 w로 fopen
    fprintf(fp, "%s is %d years old\n", name, age);//파일에 이러한 스트링의 형태로 바꾸어 씀
    fclose(fp);//fp사용 종료
    exit(0);
}
