#include <stdio.h>
#include <stdlib.h>
void ssu_printenv(char *label, char ***envpp);
extern char **environ;

int main(int argc, char *argv[], char *envp[])
{
    ssu_printenv("Initially", &envp);//Initially와 envp를 인자로 ssu_printenv실행
    putenv("TZ=PST8PDT");//환경변수에 삽입
    ssu_printenv("After changing TZ", &envp);//변경 된뒤 envp 출력
    putenv("WARNING=Don't use envp after putenv()");//환경 변수에 삽입
    ssu_printenv("After setting a new variable", &envp);//ssu_printenv실행
    printf("value of WARNING is %s\n", getenv("WARNING"));//WARNING=형태의 환경변수를 가져옴
    exit(0);
}
void ssu_printenv(char *label, char ***envpp) {
    char **ptr;
    printf("---- %s ---\n", label);//label출력
    printf("envp is at %8o and contains %8o\n", envpp, *envpp);//envp출력
    printf("environ is at %8o and contains %8o\n", &environ, environ);//environ출력
    printf("My environment variable are:\n");
    for (ptr = environ; *ptr; ptr++)
        printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);//environ을 전부 출력
    printf("(%8o) = %8o\n", ptr, *ptr);//마지막 널문자의 위치를 출력
}
