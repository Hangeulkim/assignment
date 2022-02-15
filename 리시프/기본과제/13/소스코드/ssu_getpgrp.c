#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    pid_t pgid;
    pid_t pid;
    pid = getpid();//pid 검색
    pgid = getpgrp();//프로세스 그룹 id 검색
    printf("pid: %d, pgid: %d\n", pid, pgid);//프로세스 id와 프로세스 그룹 id 출력
    exit(0);
}
