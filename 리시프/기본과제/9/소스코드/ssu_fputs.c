#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main (int argc, char *argv[])
{
    char buf[BUFFER_SIZE];
    FILE *fp;
    if (argc != 2) {//���ڰ� ����� ���Դ��� Ȯ��
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "w+")) == NULL) {//argv[1]�� w+(�а� ����, �����ʱ�ȭ)�� fopen
        fprintf(stderr, "fopen error for %s\n", argv[1]);
        exit(1);
    }

    fputs("Input String >> ", stdout);//input string>>�� stdout�� ���
    gets(buf);//������ �о�� buf�� ����
    fputs(buf, fp);//buf�� fp�� ��
    rewind(fp);//fp�� ��ġ�� ó������ �̵�
    fgets(buf, sizeof(buf), fp);//fp���� buf��ŭ ���� ���ڳ� eof�� ���������� �о��
    puts(buf);//buf�� ���
    fclose(fp);//fp����� ����
    exit(0);
}
