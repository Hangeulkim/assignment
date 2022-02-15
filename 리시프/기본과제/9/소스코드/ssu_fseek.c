#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long position;
    int character;
    if ((fp = fopen(fname, "r")) == NULL) {//fname�� r�� fopen
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    printf("Input number >>");
    scanf("%ld", &position);//�Է°��� position�� ����
    fseek(fp, position - 1, SEEK_SET);//fp�� �������� position-1�� �̵���Ŵ
    character = getc(fp);//�װ����� �ѱ��ڸ� �о��
    printf("%ldth character => %c\n", position, character);//position��ġ���� �о�� ���ڸ� ���
    exit(0);
}
