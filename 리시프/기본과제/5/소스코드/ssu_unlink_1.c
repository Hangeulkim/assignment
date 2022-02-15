#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(void)
{
    char *fname="ssu_dump.txt";
    if(open(fname,O_RDWR)<0){//fname을 읽고 쓰기로 open
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }
    if(unlink(fname)<0){//fname을 삭제
        fprintf(stderr,"unlink error for %s\n",fname);
        exit(1);
    }
    printf("File unlinked\n");
    sleep(20);//잠시 대기
    printf("Done\n");
    exit(0);
}
