#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 4096
int main(void) {
    char buf[MAX_LINE];
    char name[L_tmpnam];
    FILE *fp;
    printf("temp file 1 : %s\n", tmpnam(NULL));//���� ���۸� �̿��� �� �����͸� ����
    tmpnam(name);//name�� ��ο� �ش��ϴ� �ӽ� ���� �̸��� ����
    printf("temp file 2 : %s\n", name);
    if ((fp = tmpfile()) == NULL) {//w+���� �ӽ� ������ ����
        fprintf(stderr, "tmpfile error\n");
        exit(1);
    }
    fputs("tmpfile created temporary file.\n", fp);//�ӽ� ���Ͽ� ��Ʈ�� �Է�
    fseek(fp, 0, SEEK_SET);//fp�� �������� ó������ �̵�
    if (fgets(buf, sizeof(buf), fp) == NULL) {//fp���� ������ �о�� buf�� ����
        fprintf(stderr, "fgets error\n");
        exit(1);
    }
    fputs(buf, stdout);//buf�� stdout���� ���
    exit(0);
}
