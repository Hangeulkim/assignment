#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<setjmp.h>
#include<signal.h>

static void ssu_signal_handler1(int signo);
static void ssu_signal_handler2(int signo);

sigjmp_buf jmp_buf1;
sigjmp_buf jmp_buf2;

int main(void)
{
    struct sigaction act_sig1;
    struct sigaction act_sig2;
    int i, ret;

    printf("My PID is %d\n",getpid());//현재 pid 출력
    ret = sigsetjmp(jmp_buf1,1);//jmp의 위치를 지정

    if(ret == 0){
        act_sig1.sa_handler = ssu_signal_handler1;//act_sig1.sa_handler을 ssu_signal_handler1로 지정
        sigaction(SIGINT, &act_sig1, NULL);//SIGINT시 act_sig1을 실행
    }
    else if(ret == 3)
        printf("---------------\n");

    printf("Starting\n");
    sigsetjmp(jmp_buf2,2);//jmp의 위치 지정
    act_sig2.sa_handler = ssu_signal_handler2;//핸들러를 ssu_signal_handler2로 설정
    sigaction(SIGUSR1,&act_sig2,NULL);//SIGUSR1이 울릴시 act_sig2를 실행

    for(i=0;i<20;i++){
        printf("i = %d\n",i);
        sleep(1);
    }

    exit(0);
}

static void ssu_signal_handler1(int signo){
    fprintf(stderr, "\nInterrupted\n");
    siglongjmp(jmp_buf1,3);//jmp_buf1의 위치로 이동
}

static void ssu_signal_handler2(int signo){
    fprintf(stderr,"\nSIGUSR1\n");
    siglongjmp(jmp_buf2,2);//jmp_buf2의 위치로 이동
}
