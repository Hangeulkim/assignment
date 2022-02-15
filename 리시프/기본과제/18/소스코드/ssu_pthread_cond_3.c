#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define VALUE_DONE 10
#define VALUE_STOP1 3
#define VALUE_STOP2 6

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int glo_val = 0;

int main()
{
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,&ssu_thread1,NULL);//ssu_thread1을 실행하는 새로운 쓰레드 생성
    pthread_create(&tid2,NULL,&ssu_thread2,NULL);//ssu_thread2을 실행하는 새로운 쓰레드 생성
    pthread_join(tid1,NULL);//tid1의 종료를 기다림
    pthread_join(tid2,NULL);//tid2의 종료를 기다림
    printf("final value: %d\n",glo_val);
    exit(0);
}

void *ssu_thread1(void *arg){
    while(1){
        pthread_mutex_lock(&lock);//다른 쓰레드에서의 접근을 막음
        pthread_cond_wait(&cond,&lock);//cond에서 대기하도록 만듬
        glo_val++;
        printf("global value ssu_thread1: %d\n",glo_val);
        pthread_mutex_unlock(&lock);//다른 쓰레드에서의 접근을 허가

        if(glo_val>=VALUE_DONE)
            return NULL;
    }
}

void *ssu_thread2(void *arg){
    while(1){
        pthread_mutex_lock(&lock);//다른 쓰레드 에서의 접근을 막음
        if(glo_val<VALUE_STOP1||glo_val>VALUE_STOP2)
            pthread_cond_signal(&cond);//cond에서 대기하고 있는 mutex를 깨움
        else{
            glo_val++;
            printf("global value ssu_thread2: %d\n",glo_val);
        }

        pthread_mutex_unlock(&lock);//다른 쓰레드에서의 접근을 허가

        if(glo_val >= VALUE_DONE)
            return NULL;
    }
}
