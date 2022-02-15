#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char *argv[] = {
    "ssu_execl_test_1", "param1", "param2", (char *)0
    };
    printf("this is the original program\n");
    execv("./ssu_execl_test_1", argv);//argv를 넘겨주고 ssu_execl_test_1를 실행
    printf("%s\n", "This line should never get printed\n");
    exit(0);
}
