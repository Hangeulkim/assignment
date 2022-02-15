#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);

int main(void)
{
    char buf[BUFFER_SIZE]; char *fname = "/dev/pts/0";
    FILE *fp;
    if ((fp = fopen(fname, "w")) == NULL) {//fname�� w�� fopen
        fprintf(stderr, "fopen error for %s", fname);
        exit(1);
    }
    ssu_setbuf(fp, buf);//buf�� ���۷� ���
    fprintf(fp, "Hello, "); sleep(1);
    fprintf(fp, "UNIX!!"); sleep(1);
    fprintf(fp, "\n"); sleep(1);

    ssu_setbuf(fp, NULL);//���۸� ������� ����
    fprintf(fp, "HOW"); sleep(1);
    fprintf(fp, " ARE"); sleep(1);
    fprintf(fp, " YOU?"); sleep(1);
    fprintf(fp, "\n"); sleep(1);
    exit(0);
}

void ssu_setbuf(FILE *fp, char *buf) {
    size_t size; int fd; int mode;
    fd = fileno(fp);//������ ����
    if (isatty(fd))//fd�� �͹̳ο� ����Ǿ� �ִ��� Ȯ��
        mode = _IOLBF;//������� ���� ������ ���۸��� �̿�
    else
        mode = _IOFBF;//������� ���� ������ �̿�
    if (buf == NULL) {
        mode = _IONBF;//������� ���۸� ������� �ʰ� �Ǹ� buf�� size�� ����
        size = 0;
    }
    else
        size = BUFFER_SIZE;//���ۻ����� ����

    setvbuf(fp, buf, mode, size);//mode�� �����ϰ� buf�� size��ŭ�� ���۷� ���
}
