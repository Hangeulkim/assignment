#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<sys/time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

int glo_val1=1,glo_val2=2;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int main()
{
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,ssu_thread1,NULL);//ssu_thread1을 실행하는 새로운 쓰레드 생성
    pthread_create(&tid2,NULL,ssu_thread2,NULL);//ssu_thread2을 실행하는 새로운 쓰레드 생성
    pthread_join(tid1,NULL);//tid1의 종료를 기다림
    pthread_join(tid2,NULL);//tid2의 종료를 기다림
    pthread_mutex_destroy(&lock);//lock를 제거
    pthread_cond_destroy(&cond);//cond를 제거

    exit(0);
}

void *ssu_thread1(void *arg){
    sleep(1);
    glo_val1 =2;
    glo_val2=1;

    if(glo_val1>glo_val2)
        pthread_cond_broadcast(&cond);//cond의 모든 쓰레드를 깨움

    printf("ssu_thread1 end\n");
    return NULL;
}

void *ssu_thread2(void *arg){
    struct timespec timeout;
    struct timeval now;

    pthread_mutex_lock(&lock);//다른 쓰레드 에서의 접근을 막음
    gettimeofday(&now,NULL);

    timeout.tv_sec = now.tv_sec + 5;
    timeout.tv_nsec = now.tv_usec * 1000;

    if(glo_val1 <= glo_val2){
        printf("ssu_thread2 sleep\n");
        if(pthread_cond_timedwait(&cond,&lock,&timeout)==ETIMEDOUT)//대기시킴
            printf("timeout\n");
        else
            printf("glo_val1 = %d, glo_val2 = %d\n",glo_val1,glo_val2);
    }

    pthread_mutex_unlock(&lock);//다른 쓰레드에서의 접근을 허가
    printf("ssu_thread2 end\n");
    return NULL;
}
