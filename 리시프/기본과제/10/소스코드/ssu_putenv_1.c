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
        printf("environ[%d] : %s\n", i, environ[i]);//ȯ�� ���� �� ���
    putenv(glob_var);//ȯ�� ������ glob_var���
    ssu_addone();
    printf("My hobby is %s\n", getenv("HOBBY"));//HOBBY ȯ�溯���� ������
    printf("My lover is %s\n", getenv("LOVER"));//LOVER ȯ�溯���� ������
    strcpy(glob_var + 6, "fishing");//HOBBY=swimming�� HOBBY=fishing���� ����
    for (i = 0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);//ȯ�溯���� ���
    exit(0);
}

void ssu_addone(void) {
    char auto_var[10];
    strcpy(auto_var, "LOVER=js");
    putenv(auto_var);//ȯ�溯���� auto_var ���
}
