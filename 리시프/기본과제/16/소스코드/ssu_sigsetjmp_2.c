#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<setjmp.h>
#include<signal.h>
#include<time.h>

static void ssu_alarm(int signo);
static void ssu_func(int signo);
void ssu_mask(const char *str);

static volatile sig_atomic_t can_jump;
static sigjmp_buf jump_buf;

int main(void)
{
    if(signal(SIGUSR1,ssu_func)==SIG_ERR){//SIGUSR1발생시 ssu_func를 실행
        fprintf(stderr,"SIGUSR1 error");
        exit(1);
    }

    if(signal(SIGALRM,ssu_alarm)==SIG_ERR){//SIGALRM발생시 ssu_alarm을 실행
        fprintf(stderr,"SIGALRM error");
        exit(1);
    }

    ssu_mask("starting main: ");

    if(sigsetjmp(jump_buf,1)){//jmp위치를 지정
        ssu_mask("ending main: ");
        exit(0);
    }

    can_jump = 1;

    while(1)
        pause();

    exit(0);
}

void ssu_mask(const char *str){
    sigset_t sig_set;
    int err_num;

    err_num = errno;

    if(sigprocmask(0,NULL,&sig_set)<0){
        printf("sigprocmask() error");
        exit(1);
    }

    printf("%s",str);

    if(sigismember(&sig_set, SIGINT))//sig_set에 SIGINT가 있으면
        printf("SIGINT ");
    if(sigismember(&sig_set, SIGQUIT))//sig_set에 SIGQUIT가 있으면
        printf("SIGQUIT ");
    if(sigismember(&sig_set, SIGUSR1))//sig_set에 SIGUSR1가 있으면
        printf("SIGUSR1 ");
    if(sigismember(&sig_set,SIGALRM))//sig_set에 SIGALRM가 있으면
        printf("SIGALRM ");

    printf("\n");
    errno = err_num;
}

static void ssu_func(int signo){
    time_t start_time;

    if(can_jump == 0)
        return;

    ssu_mask("starting ssu_func: ");
    alarm(3);//3초뒤 SIGALRM 전송
    start_time = time(NULL);

    while(1)
        if(time(NULL)>start_time+5)//시작한 시간 이후 5초뒤 종료
            break;

    ssu_mask("ending ssu_func: ");
    can_jump = 0;
    siglongjmp(jump_buf,1);//sigsetjmp의 위치로 점프
}

static void ssu_alarm(int signo){
    ssu_mask("in ssu_alarm: ");
}
