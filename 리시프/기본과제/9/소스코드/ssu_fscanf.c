#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void)
{
    char *fname = "ssu_test.dat";
    char name[BUFFER_SIZE];
    FILE *fp;
    int age;

    fp = fopen(fname, "r");//fname�� r�� fopen
    fscanf(fp, "%s%d", name, &age);//fp���� %s%d�������� �о��
    fclose(fp);//fp��� ����
    fp = fopen(fname, "w");//fname�� w�� fopen
    fprintf(fp, "%s is %d years old\n", name, age);//���Ͽ� �̷��� ��Ʈ���� ���·� �ٲپ� ��
    fclose(fp);//fp��� ����
    exit(0);
}
