#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<error.h>
#include<errno.h>
#include<fcntl.h>

#define NAMESIZE 50
#define MAXTRIES 5

struct employee{
    char name[NAMESIZE];
    int salary;
    int pid;
};

int main(int argc, char *argv[])
{
    struct flock lock;
    struct employee record;
    int fd, sum=0, try=0;

    sleep(10);

    if((fd=open(argv[1],O_RDONLY))==-1){//인자로 받은 파일을 읽기 전용으로 open
        perror(argv[1]);
        exit(1);
    }
    lock.l_type=F_RDLCK;//읽기 lock을 설정
    lock.l_start=0L;
    lock.l_len=0L;

    while(fcntl(fd,F_SETLK,&lock)==-1){//fd의 lock설정을 lock로 설정
        if(errno == EACCES){
            if(try++ < MAXTRIES){
                sleep(1);
                continue;
            }
            printf("%s busy -- try later\n",argv[1]);
            exit(2);
        }
        perror(argv[1]);
        exit(3);
    }
    sum=0;
    while(read(fd,(char*)&record,sizeof(record))>0){
        printf("Employee: %s, Salary: %d\n",record.name, record.salary);
        sum+=record.salary;
    }//fd에서 데이터를 읽어오고 출력
    printf("\nTotal salary: %d\n",sum);//총합 출력

    lock.l_type=F_UNLCK;//lock을 다시 해제
    fcntl(fd,F_SETLK,&lock);//fd의 lock를 lock로 설정
    close(fd);
}
