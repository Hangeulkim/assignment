#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    FILE *fp;
    int character;
    if (argc != 2) {//���ڰ� �ùٸ��� �־������� Ȯ��
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "r")) == NULL) {//argv[1]�� r�� fopen
        fprintf(stderr, "fopen error for %s\n", argv[1]);
        exit(1);
    }
    character = fgetc(fp);//fp���� �ѱ��ڸ� �о�ͼ� character�� ����

    while (!feof(fp)) {//fp�� eof�� �����ߴ��� Ȯ��
        fputc(character, stdout);//stdout�� character�� putc
        if (ferror(fp)) {//fp�� ���� Ȯ��
            fprintf(stderr, "Error detected!!\n");
            clearerr(fp);//eof �÷��׸� Ŭ����
        }
        character = fgetc(fp);//fp���� �ѱ��ڸ� �о�ͼ� character�� ����
    }
    fclose(fp);//fp�� ����� ����
    exit(0);
}
