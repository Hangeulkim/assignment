#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(void)
{
    int fd, state;
    state = seteuid(1000);//euid를 1000으로 설정
    if (state < 0) {
        fprintf(stderr, "seteuid error\n");
        exit(1);
    }
    if ((fd = open("ssu_test.txt", O_CREAT | O_RDWR, S_IRWXU)) < 0) {
        fprintf(stderr, "open error\n");//ssu_test.txt를 open하는데 실패
        exit(1);
    }
    close(fd);
    exit(0);
}
