#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void) {
    char buf[BUFFER_SIZE];
    int a, b;
    int i ;
    setbuf(stdin, buf);//buf를 bufsize만큼의 char배열을 버퍼로 사용
    scanf("%d %d", &a, &b);
    for (i = 0; buf[i] != '\n';i++)
        putchar(buf[i]);//buf[i]가 \n이 아닐때 까지 출력
    putchar('\n');
    exit(0);
}
