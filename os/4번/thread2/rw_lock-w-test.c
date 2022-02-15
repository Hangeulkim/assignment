#include "rw_lock.h"

void init_rwlock(struct rw_lock * rw)
{
  //	Write the code for initializing your read-write lock.
 	pthread_mutex_init(&rw->use_mutex,NULL);
 	pthread_cond_init(&rw->readers,NULL);
 	pthread_cond_init(&rw->writers,NULL);
 	rw->read_num = 0;
 	rw->write_num = 0;
 	rw->waiting_num = 0;
}

void r_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  	pthread_mutex_lock(&rw->use_mutex);
  	while(rw->write_num||rw->waiting_num){//작업중인 write쓰레드가 있거나 락을 얻기위해 기다리는 write쓰레드가 있는경우 wait시킴
  		pthread_cond_wait(&rw->readers,&rw->use_mutex);
  	}
  	rw->read_num++;
  	pthread_mutex_unlock(&rw->use_mutex);
}

void r_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  	pthread_mutex_lock(&rw->use_mutex);
  	rw->read_num--;
  	if(rw->waiting_num){//기다리는 write쓰레드가 있는 경우 writer쓰레드를 깨움
  		pthread_cond_signal(&rw->writers);
  	}
  	pthread_mutex_unlock(&rw->use_mutex);
}

void w_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  	
  	pthread_mutex_lock(&rw->use_mutex);
  	rw->waiting_num++;
  	while(rw->read_num||rw->write_num){//작업중인 읽기 쓰레드가 있거나 쓰기 쓰레드가 있는 경우 대기시킴
  		pthread_cond_wait(&rw->writers,&rw->use_mutex);
  	}
  	rw->waiting_num--;
  	rw->write_num++;
  	pthread_mutex_unlock(&rw->use_mutex);
}

void w_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  	pthread_mutex_lock(&rw->use_mutex);
  	rw->write_num--;
  	if(rw->waiting_num){//기다리는 쓰기 쓰레드를 깨움
  		pthread_cond_signal(&rw->writers);
  	}
  	pthread_cond_broadcast(&rw->readers);//reader쓰레드를 깨우고 writer의 작업이 끝날때 까지 대기시킴
  	pthread_mutex_unlock(&rw->use_mutex);
  	
}
