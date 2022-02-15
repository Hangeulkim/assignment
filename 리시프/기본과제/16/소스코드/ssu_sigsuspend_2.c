#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>

static void ssu_func(int signo);
void ssu_print_mask(const char *str);

int main(void)
{
    sigset_t new_mask, old_mask, wait_mask;

    ssu_print_mask("program start: ");
    if(signal(SIGINT,ssu_func)==SIG_ERR){//SIGINT발생시 ssu_func를 실행
        fprintf(stderr,"signal(SIGINT) error\n");
        exit(1);
    }

    sigemptyset(&wait_mask);//wait_mask를 초기화
    sigaddset(&wait_mask,SIGUSR1);//wait_mask에 SIGUSR1을 추가
    sigemptyset(&new_mask);//new_mask를 초기화
    sigaddset(&new_mask,SIGINT);//new_mask에 SIGINT를 추가

    if(sigprocmask(SIG_BLOCK, &new_mask, &old_mask)<0){//new_mask에 해당하는 시그널이 들어올시 block시키고 이전 시그널 값을 old_mask에 저장
        fprintf(stderr,"SIG_BLOCK() error\n");
        exit(1);
    }

    ssu_print_mask("in critical region: ");

    if(sigsuspend(&wait_mask)!=-1){//wait_mask를 블록시킬 시그널로 재설정
        fprintf(stderr,"sigsuspend() error\n");
        exit(1);
    }

    ssu_print_mask("after return from sigsuspend: ");

    if(sigprocmask(SIG_SETMASK,&old_mask,NULL)<0){//old_mask를 다시 mask로 설정
        fprintf(stderr,"SIG_SETMASK() error\n");
        exit(1);
    }

    ssu_print_mask("program exit: ");
    exit(0);
}

void ssu_print_mask(const char *str){
    sigset_t sig_set;
    int err_num;

    err_num = errno;

    if(sigprocmask(0,NULL, &sig_set)<0){//sig_set에 이전 마스크를 저장
        fprintf(stderr,"sigprocmask() error\n");
        exit(1);
    }

    printf("%s",str);

    if(sigismember(&sig_set,SIGINT))//sig_set에 SIGINT가 있는지 확인
        printf("SIGINT ");
    if(sigismember(&sig_set,SIGQUIT))//sig_set에 SIGQUIT가 있는지 확인
        printf("SIGQUIT ");
    if(sigismember(&sig_set,SIGUSR1))//sig_set에 SIGUSR1가 있는지 확인
        printf("SIGUSR1 ");
    if(sigismember(&sig_set,SIGALRM))//sig_set에 SIGALRM가 있는지 확인
        printf("SIGALRM ");

    printf("\n");
    errno = err_num;
}

static void ssu_func(int signo){
    ssu_print_mask("\nin ssu_func: ");
}
