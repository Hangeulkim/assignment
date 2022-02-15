#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024 //읽어올 버퍼 사이즈

int main(void){
    char buf[BUFFER_SIZE];//읽어올 버퍼
    char *fname = "ssu_test.txt";//읽어올 파일
    int count;
    int fd1, fd2;

    fd1 = open(fname,O_RDONLY,0644);//ssu_test.txt를 읽기 권한으로 엶
    fd2 = open(fname,O_RDONLY,0644);//ssu_tst.txxt를 읽기 권한으로 엶
    
    if(fd1 < 0 || fd2 < 0){//파일 여는것을 실패햇는지 체크
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }

    count = read(fd1,buf,25);//fd1의 25바이트 만큼 읽어옴
    buf[count] = 0;
    printf("fd1's first printf : %s\n",buf);
    lseek(fd1,1,SEEK_CUR);//개행 문자 건너뛰기
    count = read(fd1,buf,24);//fd1의 24바이트 만큼 읽어옴
    buf[count] = 0;
    printf("fd1's second printf : %s\n",buf);
    count = read(fd2,buf,25);//fd2에 25바이트 만큼 읽어옴
    buf[count]=0;
    printf("fd2's first printf : %s\n",buf);
    lseek(fd2,1,SEEK_CUR);//개행문자 건너 뛰기
    count = read(fd2,buf,24);//fd2에 24바이트 만큼 읽어옴
    buf[count]=0;
    printf("5d2's second printf : %s\n",buf);

    exit(0);
}
