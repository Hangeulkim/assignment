#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void) {
    char buf[BUFFER_SIZE];
    int a, b;
    int i ;
    setbuf(stdin, buf);//buf�� bufsize��ŭ�� char�迭�� ���۷� ���
    scanf("%d %d", &a, &b);
    for (i = 0; buf[i] != '\n';i++)
        putchar(buf[i]);//buf[i]�� \n�� �ƴҶ� ���� ���
    putchar('\n');
    exit(0);
}
