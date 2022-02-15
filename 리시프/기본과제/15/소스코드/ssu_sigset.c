#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int main(void)
{
    sigset_t set;
    sigemptyset(&set);//set인자가 가리키는 시그널 집합을 공집합으로 만듬
    sigaddset(&set, SIGINT);//SIGINT에 대한 시그널을 set에 추가
    switch (sigismember(&set, SIGINT))//SIGINT가 시그널 집합에 포함되어 있는지 확인
    {
        case 1 :
            printf("SIGINT is included. \n");//포함됨
            break;
        case 0 :
            printf("SIGINT is not included. \n");//포함안됨
            break;
        default :
            printf("failed to call sigismember() \n");//실행을 실패함
    }

    switch (sigismember(&set, SIGSYS))//SIGSYS가 SET에 포함되어 있는지 확인
    {
        case 1 :
            printf("SIGSYS is included. \n");//포함됨
            break;
        case 0 :
            printf("SIGSYS is not included. \n");//포함안됨
            break;
        default :
            printf("failed to call sigismember() \n");//실행에 실패함
    }
    exit(0);
}
