#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int main(int argc, char *argv[])
{
    if (argc != 2) {//인자가 제대로 들어왔는지 확인
        fprintf(stderr, "usage: %s [Process ID]\n", argv[0]);
        exit(1);
    }
    else
        kill(atoi(argv[1]), SIGKILL);//인자로 들어온 첫번째 프로그램에 SIGKILL을 보냄
    exit(0);
}
