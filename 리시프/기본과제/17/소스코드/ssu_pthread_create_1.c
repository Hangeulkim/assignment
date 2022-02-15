#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
void *ssu_thread(void *arg);
int main(void)
{
    pthread_t tid;
    pid_t pid;
    if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {//새 thread를 기본 특성으로  tid를 새 쓰레드 id를 저장 ssu_thread함수에서 실행을 시작하도록 생성
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    pid = getpid();//pid를 가져옴
    tid = pthread_self();//현재 쓰레드의 id를 리턴
    printf("Main Thread: pid %u tid %u \n", (unsigned int)pid, (unsigned int)tid);
    sleep(1);
    exit(0);
}

void *ssu_thread(void *arg) {
    pthread_t tid;
    pid_t pid;
    pid = getpid();//pid를 가져옴
    tid = pthread_self();//현재 쓰레드의 id를 리턴
    printf("New Thread: pid %d tid %u \n", (int)pid, (unsigned int)tid);
    return NULL;
}
