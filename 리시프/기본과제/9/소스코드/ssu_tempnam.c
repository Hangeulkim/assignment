#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    char *arg_directory = NULL;
    char *arg_prefix = NULL;
    if (argc != 3) {//���ڰ� ����� �־������� Ȯ��
        fprintf(stderr, "usage: %s <directory> <prefix>\n", argv[0]);
        exit(1);
    }
    arg_directory = argv[1][0] != ' ' ? argv[1] : NULL;//ó���� ��ĭ�� �ƴϸ� argv[1]�� ����
    arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL;//ó���� ��ĭ�̾ƴϸ� argv[2]�� ����
    printf("created : %s\n", tempnam(arg_directory, arg_prefix));//arg_directory�� arg_prefix�� �տ� �ٿ��� �ӽ������� ����
    exit(0);
}
