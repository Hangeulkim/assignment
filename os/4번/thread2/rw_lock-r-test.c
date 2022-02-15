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
  	rw->read_num++;
  	pthread_mutex_unlock(&rw->use_mutex);
}

void r_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  	pthread_mutex_lock(&rw->use_mutex);
  	rw->read_num--;
  	if(!rw->read_num&&rw->waiting_num){//락을 획득 하기위해 대기중인 쓰레드가 있고, 읽기 쓰레드의 작업이 전부 종료된 경우 writer를 깨움
  		pthread_cond_signal(&rw->writers);
  	}
  	pthread_mutex_unlock(&rw->use_mutex);
}

void w_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.

  	pthread_mutex_lock(&rw->use_mutex);
  	rw->waiting_num++;
  	while(rw->read_num>0){//작업중인 read쓰레드가 있는경우 wait시킴
  		pthread_cond_wait(&rw->writers,&rw->use_mutex);
  	}
  	rw->write_num++;
  	rw->waiting_num--;
  	pthread_mutex_unlock(&rw->use_mutex);
}

void w_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  	pthread_mutex_lock(&rw->use_mutex);
  	rw->write_num--;
  	if(!rw->read_num&&rw->waiting_num){//아직 깨어나지 못한 wait 쓰레드가 있는경우 깨움
  		pthread_cond_signal(&rw->writers);
  	}
  	pthread_mutex_unlock(&rw->use_mutex);

}
