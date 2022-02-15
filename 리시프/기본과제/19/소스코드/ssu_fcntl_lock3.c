#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define NAMESIZE 50

struct employee{
    char name[NAMESIZE];
    int salary;
    int pid;
};

int main(int argc, char *argv[])
{
    struct flock lock;
    struct employee record;
    int fd, recnum,pid;
    long position;

    if((fd=open(argv[1],O_RDWR))==-1){//인자로 들어온 파일을 읽고 쓰기로 open
        perror(argv[1]);
        exit(1);
    }

    pid=getpid();
    for(;;){
        printf("\nEnter record number: ");
        scanf("%d",&recnum);
        if(recnum<0)//입력받은 레코드가 0번보다 작으면 탈출
            break;

        position=recnum*sizeof(record);
        lock.l_type=F_WRLCK;//wrlock을 설정
        lock.l_whence=0;
        lock.l_start = position;//시작점을 지정
        lock.l_len=sizeof(record);//길이는 record
        if(fcntl(fd,F_SETLKW,&lock)==-1){//fd에 lock을 설정
            perror(argv[1]);
            exit(2);
        }
        lseek(fd,position,0);//fd의 위치를 position으로 이동
        if(read(fd, (char*)&record,sizeof(record))==0){//사이즈만큼 읽어옴
            printf("record %d not found\n",recnum);
            lock.l_type=F_UNLCK;
            fcntl(fd,F_SETLK,&lock);
            continue;
        }
        printf("Employee: %s, salary: %d\n",record.name,record.salary);//데이터 출력
        record.pid=pid;
        printf("Enter new salary: ");
        scanf("%d",&record.salary);//갱신할 salary입력받음
        write(fd, (char*)&record,sizeof(record));

        lock.l_type=F_UNLCK;//unlck을 설정
        fcntl(fd,F_SETLK,&lock);//lock을 해제
    }
    close(fd);
}
