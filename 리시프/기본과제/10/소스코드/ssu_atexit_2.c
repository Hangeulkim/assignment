#include <stdio.h>
#include <stdlib.h>
void ssu_out(void);
int main(void)
{
    if (atexit(ssu_out)) {//ateexit�� ssu_out�� ���ڷ� �ְ� ȣ��
        fprintf(stderr, "atexit error\n");
        exit(1);
    }
    exit(0);
}

void ssu_out(void) {
    printf("atexit succeeded!\n");//����� �ڵ����� ���
}
