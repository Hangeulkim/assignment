#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int character;
    while ((character = getc(stdin)) != EOF)//stdin���� EOF�� ���������� �о�� character�� ����

    if (putc(character, stdout) == EOF) {//stdout�� character�� putc
        fprintf(stderr, "standard output error\n");
        exit(1);
    }
    if (ferror(stdin)) {//stdin�� ���� �߻� ���� Ȯ��
        fprintf(stderr, "standard input error\n");
        exit(1);
    }
    exit(0);
}
