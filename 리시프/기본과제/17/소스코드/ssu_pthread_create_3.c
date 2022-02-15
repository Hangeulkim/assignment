#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *ssu_thread(void *arg);
pthread_t glo_tid;
int main(void)
{
    pthread_t loc_tid;
    if (pthread_create(&loc_tid, NULL, ssu_thread, NULL) != 0) {//쓰레드 id를 loc_tid에 저장하고 ssu_thread를 시작함수로 지정
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    sleep(5);
    if (pthread_equal(loc_tid, glo_tid) == 0) {//loc_tid와 glo_tid의 쓰레드 아이디가 동일한지 확인
        printf("다른 쓰레드\n");
        exit(0);
    }
    printf("동일한 쓰레드\n");
    exit(0);
}

void *ssu_thread(void *arg) {
    printf("쓰레드에서 자신의 쓰레드 ID를 전역변수에 할당 \n");
    glo_tid = pthread_self();//glo_tid에 현재 쓰레드의 아이디를 저장
    return NULL;
}
