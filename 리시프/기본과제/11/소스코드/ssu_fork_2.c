#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t pid;
    char character, first, last;
    long i;
    if ((pid = fork()) > 0) {//�θ��� ��� first �� last �� ���� �빮�� A,Z�� ����
        first = 'A';
        last = 'Z';
    }
    else if (pid == 0) {//�ڽ��� ��� first�� last�� �ҹ��� a,z�� ����
        first = 'a';
        last = 'z';
    }
    else {//fork ���н�
        fprintf(stderr, "%s\n", argv[0]);
        exit(1);
    }

    for (character = first; character <= last; character++) {//first~last���� ���� ���
        for (i = 0;i <= 100000;i++)
            ;//������ ����
        write(1, &character, 1);//1���ھ� stdout�� ��
    }
    printf("\n");
    exit(0);
}
