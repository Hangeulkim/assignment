#include <stdio.h>
#include <stdlib.h>
int main(void) {
    printf("abort terminate this program\n");
    abort();//프로그램을 비정상적으로 종료시키고 SIGABRT를 발생시킴
    printf("this line is never reached\n");
    exit(0);
}
