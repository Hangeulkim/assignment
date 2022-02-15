#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t pid;
    char character, first, last;
    long i;
    if ((pid = fork()) > 0) {//부모의 경우 first 와 last 를 각각 대문자 A,Z로 설정
        first = 'A';
        last = 'Z';
    }
    else if (pid == 0) {//자식의 경우 first와 last를 소문자 a,z로 설정
        first = 'a';
        last = 'z';
    }
    else {//fork 실패시
        fprintf(stderr, "%s\n", argv[0]);
        exit(1);
    }

    for (character = first; character <= last; character++) {//first~last까지 전부 출력
        for (i = 0;i <= 100000;i++)
            ;//딜레이 설정
        write(1, &character, 1);//1글자씩 stdout에 씀
    }
    printf("\n");
    exit(0);
}
