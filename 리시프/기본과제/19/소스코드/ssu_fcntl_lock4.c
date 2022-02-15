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
    int fd, recnum, pid;
    long position;
    char ans[5];

    if((fd=open(argv[1],O_RDWR))==-1){//인자로 들어온 파일을 읽고 쓰기로 open
        perror(argv[1]);
        exit(1);
    }

    pid=getpid();
    for(;;){
        printf("\nEnter record number: ");
        scanf("%d",&recnum);
        if(recnum<0)//recnum이 0보다 작으면 탈출
            break;

        position = recnum*sizeof(record);
        lock.l_type=F_RDLCK;//읽기 lock을 설정
        lock.l_whence=0;
        lock.l_start=position;
        lock.l_len=sizeof(record);

        if(fcntl(fd,F_SETLKW,&lock)==-1){//fd에 lock를 설정
            perror(argv[1]);
            exit(2);
        }

        lseek(fd,position,0);//fd위치를 position으로 이동
        if(read(fd,(char*)&record,sizeof(record))==0){//이동시킨 위치에서 record만큼 읽어옴
            printf("record %d not found\n",recnum);
            lock.l_type=F_UNLCK;//unlck를 설정
            fcntl(fd,F_SETLK,&lock);//lock을 재설정
            continue;
        }
        printf("Employee: %s, salary: %d\n",record.name,record.salary);
        printf("Do you want to update salary (y or n)? ");
        scanf("%s",ans);

        if(ans[0] != 'y'){//y이면
            lock.l_type=F_UNLCK;
            fcntl(fd,F_SETLK,&lock);//lock을 해제시킴
            continue;
        }
        lock.l_type=F_WRLCK;//wrlck를 설정
        if(fcntl(fd,F_SETLKW,&lock)==-1){//lock을 재설정
            perror(argv[1]);
            exit(3);
        }
        record.pid=pid;
        printf("Enter new salary: ");
        scanf("%d",&record.salary);

        lseek(fd,position,0);//position위치로 이동
        write(fd,(char*)&record, sizeof(record));//record 를 읽어옴

        lock.l_type=F_UNLCK;//unlck을 설정
        fcntl(fd,F_SETLK,&lock);//lock를 재설정
    }
    close(fd);
}
