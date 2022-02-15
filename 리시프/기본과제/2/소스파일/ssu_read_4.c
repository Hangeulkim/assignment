#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define TABLE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc,char *argv[]){
    static struct{
        long offset;
        int length;
    }table [TABLE_SIZE];
    char buf[BUFFER_SIZE];
    long offset;
    int entry;
    int i;
    int length;
    int fd;

    if(argc<2){//읽어올 파일의 제목이 없음
        fprintf(stderr,"usage: %s <file>\n",argv[0]);
        exit(1);
    }

    if((fd = open(argv[1],O_RDONLY))<0){//1번의 파일을 읽기권한으로 읽음
        fprintf(stderr,"open error for %s\n",argv[1]);
        exit(1);
    }

    entry = 0;
    offset = 0;
    while((length = read(fd,buf,BUFFER_SIZE))>0){//파일의 끝까지 1024바이트씩 읽어옴
        for(i = 0 ; i<length ; i++){
            table[entry].length++;
            offset++;

            if(buf[i] =='\n')//개행문자 체크
                table[++entry].offset = offset;
        }
    }
#if DEBUG
    for (i=0;i<entry;i++)
        printf("%d : %ld %d\n",i+1,table[i].offset,table[i].length);
#endif

    while(1){//파일을 한 행씩 읽어온것을 0이하를 입력하기 전까지 출력
        printf("Enter line number : ");
        scanf("%d",&length);

        if(--length < 0)
            break;

        lseek(fd,table[length].offset,0);

        if(read(fd,buf,table[length].length)<=0)
            continue;

        buf[table[length].length] = '\0';//끝을 알리기 위해 널문자 삽입
        printf("%s",buf);
    }
    close(fd);
    exit(0);
}

