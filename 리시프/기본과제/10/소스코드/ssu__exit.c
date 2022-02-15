#include <stdio.h>
#include <unistd.h>
int main(void)
{
    printf("Good afternoon?");
    _exit(0);//시그널 처리를 하지 않고 바로 종료
}
