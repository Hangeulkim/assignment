#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "ssu_score.h"

#define SECOND_TO_MICRO 1000000

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

int main(int argc, char *argv[])
{
	struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL);//���۽ð� ����

	ssu_score(argc, argv);//ä�� ���α׷� ����

	gettimeofday(&end_t, NULL);//�����ð� ����
	ssu_runtime(&begin_t, &end_t);//�ɸ��ð� ���ϴ� �Լ� ����

	exit(0);//����
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{//����ð� üũ �Լ�
	end_t->tv_sec -= begin_t->tv_sec;//���� �ð����� ���۽ð��� �ʸ� ��

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}//usec�� ���������� ū��� end�� 1�ʸ� usec�� ��ȯ

	end_t->tv_usec -= begin_t->tv_usec;//���� �ð����� ���۽ð��� usec�� ��
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);//�ɸ� �ð��� ���
}
