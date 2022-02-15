#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct rw_lock
{
	int read_num;			//진행중인 읽기 쓰레드의 숫자
	int write_num;			//진행중인 쓰기 쓰레드의 숫자
	int waiting_num;		//락을 획득하기 위해 기다리는 쓰레드의 숫자
	pthread_mutex_t use_mutex;	//락을 얻기 위한 mutex
    	pthread_cond_t  readers;	//reader의 wait cond
    	pthread_cond_t  writers;	//writer의 wait cond
};

void init_rwlock(struct rw_lock * rw);
void r_lock(struct rw_lock * rw);
void r_unlock(struct rw_lock * rw);
void w_lock(struct rw_lock * rw);
void w_unlock(struct rw_lock * rw);
long *max_element(long* start, long* end);
long *min_element(long* start, long* end);
