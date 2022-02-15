#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long fsize;
    if ((fp = fopen(fname, "r")) == NULL) {//fname�� r�� fopen
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);//fp�� �������� �� ���������� �̵�
    fsize = ftell(fp);//fp�� ��ġ������ŭ�� ����Ʈ���� fsize�� ����
    printf("The size of <%s> is %ld bytes\n", fname, fsize);//������ ����Ʈ���� ���
    exit(0);
}
