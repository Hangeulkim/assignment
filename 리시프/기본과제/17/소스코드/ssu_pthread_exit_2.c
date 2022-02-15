#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
void *ssu_thread(void *arg);
int main(void)
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {//tid에 쓰레드 id를 저장하고 ssu_thread를 시작함수로 지정
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    sleep(1);
    printf("쓰레드가 완료되기전 main 함수가 먼저 종료되면 실행중 쓰레드 소멸\n");
    printf("메인 종료\n");
    exit(0);
}
void *ssu_thread(void *arg) {
    printf("쓰레드 시작\n");
    sleep(5);
    printf("쓰레드 수행 완료\n");
    pthread_exit(NULL);//쓰레드 종료
    return NULL;
}
