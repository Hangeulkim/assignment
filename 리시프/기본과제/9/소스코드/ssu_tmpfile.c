#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void)
{
    char buf[BUFFER_SIZE];
    char name[L_tmpnam];
    FILE *fp;
    printf("Temporary filename <<%s>>\n", tmpnam(name));//name�� �ӽ����� �̸��� �����ϰ� ���
    if ((fp = tmpfile()) == NULL) {//w+�� �ӽ����� ����
        fprintf(stderr, "tmpfile create error!!\n");
        exit(1);
    }
    fputs("create tmpfile success!!\n", fp);//fp�� ��Ʈ�� �Է�
    rewind(fp);//fp�� ó������ ������ �̵�
    fgets(buf, sizeof(buf), fp);//fp���� ������ �о�� buf�� ����
    puts(buf);//buf�� ���
    exit(0);
}
