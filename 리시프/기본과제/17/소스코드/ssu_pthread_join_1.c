#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *ssu_thread(void *arg);
int main(void)
{
    pthread_t tid1, tid2;
    int thread1 = 1;
    int thread2 = 2;
    int status;
    if (pthread_create(&tid1, NULL, ssu_thread, (void *)&thread1) != 0) {//tid1에 id저장 ssu_thread를 시작함수로 지정 thread1을 인자로 넘겨줌
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    if (pthread_create(&tid2, NULL, ssu_thread, (void *)&thread2) != 0) {//tid2에 id저장 ssu_thread를 시작함수로 지정 thread2을 인자로 넘겨줌
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }
    pthread_join(tid1, (void *)&status);//tid1에 해당하는 쓰레드가 종료되길 기다리고 루틴의 코드를 status에 저장
    pthread_join(tid2, (void *)&status);//tid2에 해당하는 쓰레드가 종료되길 기다리고 루틴의 코드를 status에 저장
    exit(0);
}
void *ssu_thread(void *arg) {
    int thread_index;
    int i;
    thread_index = *((int *)arg);//전달받은 인자를 저장
    for (i = 0; i < 5; i++) {
        printf("%d : %d\n", thread_index, i);
        sleep(1);
    }
    return NULL;
}
