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
	gettimeofday(&begin_t, NULL);//시작시각 저장

	ssu_score(argc, argv);//채점 프로그램 실행

	gettimeofday(&end_t, NULL);//끝난시각 저장
	ssu_runtime(&begin_t, &end_t);//걸린시간 구하는 함수 실행

	exit(0);//종료
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{//실행시간 체크 함수
	end_t->tv_sec -= begin_t->tv_sec;//끝난 시각에서 시작시각의 초를 뺌

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}//usec가 시작지점이 큰경우 end의 1초를 usec로 변환

	end_t->tv_usec -= begin_t->tv_usec;//끝난 시각에서 시작시각의 usec를 뺌
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);//걸린 시각을 출력
}
