#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define THREAD_NUM 8

struct thread_data{
    int thread_index;
    int sum;
    char *message;
};

void *ssu_printhello(void *arg);

struct thread_data thread_data_array[THREAD_NUM];
char *messages[THREAD_NUM];

int main()
{
    pthread_t tid[THREAD_NUM];
    int sum;
    int i;

    sum=0;
    messages[0]="English: Hello World!";
    messages[1]="French: Bonjour, le monde!";
    messages[2]="Spanish: Hola al mundo";
    messages[3]="Klingon: Nuq neH!";
    messages[4]="German: Guten Tag, Welt!";
    messages[5]="Russian: Zdravstvytye, mir!";
    messages[6]="Japan: Sekai e konnichiwa!";
    messages[7]="Laatin: Orbis, te saluto!";

    for(i=0;i<THREAD_NUM;i++){
        sum=sum+i;
        thread_data_array[i].thread_index=i;
        thread_data_array[i].sum=sum;
        thread_data_array[i].message=messages[i];
        printf("Creating thread %d\n",i);

        if(pthread_create(&tid[i],NULL,ssu_printhello,(void*)&thread_data_array[i])!=0){//쓰레드 아이디를 tid[i]에 저장하고 ssu_printhello함수를 시작점으로 하며 thread_data_array[i]를 인자로 넘겨줌
            fprintf(stderr,"pthread_create error\n");
            exit(1);
        }
    }

    sleep(5);
    exit(0);
}

void *ssu_printhello(void *arg){
    struct thread_data *data;
    char *message;
    int thread_index;
    int sum;

    sleep(1);
    data=(struct thread_data *)arg;
    thread_index=data->thread_index;
    sum=data->sum;
    message=data->message;
    printf("Thread %d: %s Sum=%d\n",thread_index,message,sum);//인자로 전달받은 index message sum을 출력
    return NULL;
}
