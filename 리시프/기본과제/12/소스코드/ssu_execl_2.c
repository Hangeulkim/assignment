#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    if (fork() == 0) {
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);//echo에 리스트형으로 인자를 넘겨주어 실행
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    if (fork() == 0) {
        execl("/bin/echo", "echo", "this is", "message two", (char *)0);//echo에 리스트형으로 인자를 넘겨주어 실행
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    if (fork() == 0) {
        execl("/bin/echo", "echo", "this is", "message three", (char *)0);//echo에 리스트형으로 인자를 넘겨주어 실행
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    printf("Parent program ending\n");
    exit(0);
}
