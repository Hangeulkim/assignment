#include <stdio.h>
#include <stdlib.h>
void ssu_out(void);
int main(void)
{
    if (atexit(ssu_out)) {//ateexit에 ssu_out을 인자로 주고 호출
        fprintf(stderr, "atexit error\n");
        exit(1);
    }
    exit(0);
}

void ssu_out(void) {
    printf("atexit succeeded!\n");//종료시 자동으로 출력
}
