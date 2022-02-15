#include <pthread.h>

typedef struct SSU_wait_proc{
	struct SSU_wait_proc *prev;
	struct SSU_wait_proc *next;
	pthread_t who;
} SSU_wait_proc;

typedef struct SSU_Sem {
	int count;
	SSU_wait_proc *plist_head, *plist_tail;
	pthread_spinlock_t spinlock;
} SSU_Sem;

void SSU_Sem_init(SSU_Sem *, int);
void SSU_Sem_up(SSU_Sem *);
void SSU_Sem_down(SSU_Sem *);
