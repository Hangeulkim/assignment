#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *ssu_thread(void *arg);
int main(void)
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {//쓰레드 id를 tid에 저장하고 ssu_thread를 시작함수로 지정
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    printf("%u\n", (unsigned int)tid);
    if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {//쓰레드 id를 tid에 저장하고 ssu_thread를 시작함수로 지정
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    printf("%u\n", (unsigned int)tid);
    sleep(1);
    exit(0);
}

void *ssu_thread(void *arg) {
    pthread_t tid;
    tid = pthread_self();//tid에 현재 쓰레드 id를 저장
    printf("->%u\n", (unsigned int)tid);
    return NULL;
}
