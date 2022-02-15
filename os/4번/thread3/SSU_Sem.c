#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "SSU_Sem.h"
#include <unistd.h>

void sig_hand(int signo){
} 

void SSU_Sem_init(SSU_Sem *s, int value) {
	pthread_spin_init(&s->spinlock, 0);
	pthread_spin_lock(&s->spinlock);
	s->count=value;
	s->plist_head=(SSU_wait_proc *)malloc(sizeof(SSU_wait_proc));
	s->plist_tail=(SSU_wait_proc *)malloc(sizeof(SSU_wait_proc));
	s->plist_head->next=s->plist_tail;
	s->plist_head->prev=NULL;
	s->plist_tail->next=NULL;
	s->plist_tail->prev=s->plist_head;
	pthread_spin_unlock(&s->spinlock);
}

void SSU_Sem_down(SSU_Sem *s) {
	pthread_spin_lock(&s->spinlock);
	s->count--;
	if(s->count<0){
		SSU_wait_proc *now=(SSU_wait_proc *)malloc(sizeof(SSU_wait_proc));
		now->next=s->plist_tail;
		now->prev=s->plist_tail->prev;
		s->plist_tail->prev->next=now;
		s->plist_tail->prev=now;
		now->who=pthread_self();
	
		sigset_t sig;//쓰레드 대기를 위해 sigwait시킴
		int signum;
		sigemptyset(&sig);
		sigaddset(&sig,SIGUSR1);
		pthread_spin_unlock(&s->spinlock);
		signal(SIGUSR1,sig_hand);
		sigwait(&sig,&signum);
	}
	else
		pthread_spin_unlock(&s->spinlock);
}

void SSU_Sem_up(SSU_Sem *s) {
	pthread_spin_lock(&s->spinlock);
	s->count++;
	if(s->plist_head->next!=s->plist_tail){//만약 기다리는 친구가있다면
		SSU_wait_proc *now=s->plist_head->next;
		s->plist_head->next=now->next;//맨 앞껀 꺼내 썻으므로 이동시킴
		now->next->prev=now->prev;
		pthread_kill(now->who,SIGUSR1);
		free(now);
	}
	pthread_spin_unlock(&s->spinlock);
}
