#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/resource.h>

#define ssu_show(limit) ssu_show_limit(limit, #limit)

void ssu_show_limit(int rlim_type, char *rlim_name);

int main(void)
{
    printf("    resource soft limit hard limit  \n\n");
    ssu_show(RLIMIT_CPU);//���μ����� �Һ��� �� �ִ� �ִ� CUP�ð� ���
    ssu_show(RLIMIT_FSIZE);//���� ���� �� ������ �ִ� ũ�� ���
    ssu_show(RLIMIT_DATA);//�ڷ� ���� ��ü�� �ִ� ũ�� ���
    ssu_show(RLIMIT_STACK);//���� �ִ� ũ�� ���
    ssu_show(RLIMIT_CORE);//�� �ھ� ������ �ִ� ũ�� ���
    #ifdef RLIMIT_RSS
        ssu_show(RLIMIT_RSS);//�޸� ���� ��Ʈ ũ���� �ִ밪 ���
    #endif
    #ifdef RLIMIT_MEMLOCK
        ssu_show(RLIMIT_MEMLOCK);//�� ���μ����� mlock�� ��� �� �ִ� �޸��� �ִ� �뷮 ���
    #endif
    #ifdef RLIMIT_NPROC
        ssu_show(RLIMIT_NPROC);//���� ����� id�� �ִ� �ڽ� ���μ��� ���� ���
    #endif
    #ifdef RLIMIT_NOFILE
        ssu_show(RLIMIT_NOFILE);//�� ���μ����� ����� �� �ִ� �ִ� ���� ���� ���
    #endif
    #ifdef RLIMIT_VMEN
        ssu_show(RLIMIT_VMEN);//�� ���μ����� �� ���� �޸� �뷮�� �ִ밪 ���
    #endif

    exit(0);
}

void ssu_show_limit(int rlim_type, char *rlim_name){
    struct rlimit rlim;
    char cur[11],max[11];

    getrlimit(rlim_type,&rlim);

    if(rlim.rlim_cur == RLIM_INFINITY)//���� �Ѱ踦 ������
        strcpy(cur,"infinity");
    else
        sprintf(cur,"%10ld",rlim.rlim_cur);

    if(rlim.rlim_max == RLIM_INFINITY)//�ִ� �Ѱ踦 ������
        strcpy(max,"infinity");
    else
        sprintf(max, "%10ld",rlim.rlim_max);

    printf("%15s : %10s %10s\n",rlim_name,cur,max);
}
