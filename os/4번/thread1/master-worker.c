#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //쓰레드를 잠그기 위해 초기화
pthread_cond_t cond_master = PTHREAD_COND_INITIALIZER; //버퍼공간이 다 찼을경우 마스터 쓰레드를 쉬게 하기 위함

pthread_cond_t cond_work = PTHREAD_COND_INITIALIZER; //버퍼공간을 다 사용했을경우 워커 쓰레드를 쉬게 하기 위함

int item_to_produce, curr_buf_size, item_to_use, curr_using_buf;
int total_items, max_buf_size, num_workers, num_masters;

int *buffer;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {
    pthread_mutex_lock(&mutex);//데이터 잠금 실행
    if(curr_buf_size>=max_buf_size){//버퍼가 가득 찼을경우 데이터를 전부 사용하도록 만듦
    	curr_buf_size=0;
    	pthread_cond_broadcast(&cond_work);
    	pthread_cond_wait(&cond_master, &mutex);
    }
    
    if(item_to_produce >= total_items) {
   	pthread_cond_broadcast(&cond_master);
    	pthread_cond_broadcast(&cond_work);
    	pthread_mutex_unlock(&mutex);//데이터 잠금 해제
	break;
      }
    
    if(buffer[curr_buf_size] != -1){//아직 데이터가 사용이 안됐을 경우 다음 칸으로 패스
    	curr_buf_size++;
    	pthread_mutex_unlock(&mutex);//데이터 잠금 해제
    	continue;
    }

 
      buffer[curr_buf_size++] = item_to_produce;
      print_produced(item_to_produce, thread_id);
      item_to_produce++;
      pthread_mutex_unlock(&mutex);//데이터 잠금 해제
    }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

void *using_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {
    pthread_mutex_lock(&mutex);//데이터 잠금 실행
    
    if(curr_using_buf>=max_buf_size){//데이터를 전부 사용했을경우 데이터를 제작하도록 만듦
    	curr_using_buf=0;
    	pthread_cond_broadcast(&cond_master);
    	pthread_cond_wait(&cond_work, &mutex);
    }
    
    if(item_to_use >= total_items) {
    	pthread_cond_broadcast(&cond_master);
    	pthread_mutex_unlock(&mutex);//데이터 잠금 해제
	break;
      }
    
    if(buffer[curr_using_buf] == -1){//아직 데이터가 할당이 안됐을 경우 패스
    	curr_using_buf++;
    	pthread_mutex_unlock(&mutex);//데이터 잠금 해제
    	continue;
    }
      
      
      print_consumed(buffer[curr_using_buf], thread_id);
      buffer[curr_using_buf++] = -1;
      item_to_use++;
      pthread_mutex_unlock(&mutex);//데이터 잠금 해제
    }
  return 0;
}

int main(int argc, char *argv[])
{
  int *master_thread_id, *worker_thread_id;
  pthread_t *master_thread, *worker_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  item_to_use = 0;//소비값 초기화
  curr_using_buf = 0;//사용한 버퍼값
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

   buffer = (int *)malloc (sizeof(int) * max_buf_size);
   
   for(i=0;i < max_buf_size; i++)
   	buffer[i]=-1;

   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  //create worker consumer threads
	worker_thread_id = (int *)malloc(sizeof(int) * num_workers);//워커 쓰레드 할당
   worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
  for (i = 0; i < num_workers; i++)
    worker_thread_id[i] = i;

  for (i = 0; i < num_workers; i++)
    pthread_create(&worker_thread[i], NULL, using_loop, (void *)&worker_thread_id[i]);//워커 쓰레드 작업 실행
  
  //wait for all threads to complete
  for (i = 0; i < num_workers; i++)
    {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", i);
    }
  
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  free(worker_thread_id);
  free(worker_thread);
  
  return 0;
}
