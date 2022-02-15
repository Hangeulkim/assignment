#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {
    printf("this is the original program\n");
    execl("./ssu_execl_test_1", "ssu_execl_test_1","param1", "param2", "param3", (char *)0);//리스트 형태로 인자를 넘겨주어서 ssu_execl_test_1를 실행
    printf("%s\n", "this line should never get printed\n");
    exit(0);
}
