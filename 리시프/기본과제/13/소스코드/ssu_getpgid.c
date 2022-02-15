#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    pid_t pgid;
    pid_t pid;
    if (argc < 2) {//인자가 제대로 주어졌는지 확인
    fprintf(stderr, "usage: %s <file>\n",argv[0]);
    exit(1);
    }
    pid = getpid();//pid를 가져옴
    pgid = getpgid(atoi(argv[1]));//pgid를 가져옴 인자의
    printf("pid: %d, pgid: %d\n", pid, pgid);//pid와 pgid출력
    exit(0);
}
