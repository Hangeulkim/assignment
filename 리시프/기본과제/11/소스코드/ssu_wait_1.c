#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void ssu_echo_exit(int status);

int main(void)
{
    pid_t pid;
    int status;

    if((pid=fork())<0){//�ڽĻ���
        fprintf(stderr,"fork error\n");
        exit(1);
    }

    else if(pid==0)//�ڽ��� 7�� ��ȯ�ϰ� �����Ŵ
        exit(7);

    if(wait(&status)!=pid){//�ڽ��� ����Ǳ⸦ ��ٸ�
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status�� ���� ������  echo

    if((pid=fork())<0){//�ڽĻ���
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if(pid==0)//�ڽ��� abort�Լ��� ����
        abort();

    if(wait(&status) != pid){//�ڽ��� ����Ǳ� ��ٸ�
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status�� ���� ������ ������

    if((pid=fork())<0){//�ڽ��� ����
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if(pid==0)//�ǵ������� �Ұ����� ������ ��Ŵ
        status/=0;

    if(wait(&status)!=pid){//�ڽ��� ����Ǳ� ��ٸ�
        fprintf(stderr,"wait error\n");
        exit(1);
    }

    ssu_echo_exit(status);//status ���¸� ���
    exit(0);
}

void ssu_echo_exit(int status){
    if(WIFEXITED(status))//�ڽ��� ���������� ����Ǿ������� ����
        printf("normal termination, exit status = %d\n",WEXITSTATUS(status));
    else if(WIFSIGNALED(status))//�ڽ� ���μ����� �ñ׳��� �޾����� �װ��� ó������ �ʾ� ������������ �����
        printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? " (core file generated)" : "");//�ھ������� �����Ǿ��°��� ����
#else
            "");
#endif
    else if(WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n",WSTOPSIG(status));//�ڽ����μ����� ���� ����������� �˸�
}
