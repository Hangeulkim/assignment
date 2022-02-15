#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);
extern char **environ;
char glob_var[] = "HOBBY=swimming";

int main(void)
{
    int i;
    for (i = 0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);//환경 변수 값 출력
    putenv(glob_var);//환경 변수에 glob_var등록
    ssu_addone();
    printf("My hobby is %s\n", getenv("HOBBY"));//HOBBY 환경변수를 가져옴
    printf("My lover is %s\n", getenv("LOVER"));//LOVER 환경변수를 가져옴
    strcpy(glob_var + 6, "fishing");//HOBBY=swimming를 HOBBY=fishing으로 변경
    for (i = 0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);//환경변수를 출력
    exit(0);
}

void ssu_addone(void) {
    char auto_var[10];
    strcpy(auto_var, "LOVER=js");
    putenv(auto_var);//환경변수에 auto_var 등록
}
