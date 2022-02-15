#include <stdio.h>
#include <stdlib.h>
void ssu_printenv(char *label, char ***envpp);
extern char **environ;

int main(int argc, char *argv[], char *envp[])
{
    ssu_printenv("Initially", &envp);//Initially�� envp�� ���ڷ� ssu_printenv����
    putenv("TZ=PST8PDT");//ȯ�溯���� ����
    ssu_printenv("After changing TZ", &envp);//���� �ȵ� envp ���
    putenv("WARNING=Don't use envp after putenv()");//ȯ�� ������ ����
    ssu_printenv("After setting a new variable", &envp);//ssu_printenv����
    printf("value of WARNING is %s\n", getenv("WARNING"));//WARNING=������ ȯ�溯���� ������
    exit(0);
}
void ssu_printenv(char *label, char ***envpp) {
    char **ptr;
    printf("---- %s ---\n", label);//label���
    printf("envp is at %8o and contains %8o\n", envpp, *envpp);//envp���
    printf("environ is at %8o and contains %8o\n", &environ, environ);//environ���
    printf("My environment variable are:\n");
    for (ptr = environ; *ptr; ptr++)
        printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);//environ�� ���� ���
    printf("(%8o) = %8o\n", ptr, *ptr);//������ �ι����� ��ġ�� ���
}
