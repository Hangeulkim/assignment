#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREAD_NUM 5
void *ssu_printhello(void *arg);
int main(void)
{
    pthread_t tid[THREAD_NUM];
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        printf("In main: creating thread %d\n", i);
        if (pthread_create(&tid[i], NULL, ssu_printhello, (void *)&i) != 0) {//tid[i]에 생성된 쓰레드 id를 저장하고 기본 특성으로 생성하며 시작함수는 ssu_printhello로 하고 매개변수로 i를 넘겨줌
            fprintf(stderr, "pthread_create error\n");
            exit(1);
        }
    }
    pthread_exit(NULL);//쓰레드를 종료시킴
    exit(0);
}
void *ssu_printhello(void *arg) {
    int thread_index;
    thread_index = *((int *)arg);
    printf("Hello World! It's me, thread #%d!\n", thread_index);//전달 받은 인자를 출력
    pthread_exit(NULL);//쓰레드를 종료시킴
    return NULL;
}
