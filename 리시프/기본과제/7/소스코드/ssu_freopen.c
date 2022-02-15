#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

int main(void)
{
    char *fname="ssu_test.txt";
    int fd;

    printf("First printf : Hello, OSLAB!!\n");

    if((fd=open(fname,O_RDONLY))<0){//fname을 읽기로 open
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }

    if(freopen(fname,"w",stdout)!=NULL)//stdout을 닫고 fname을 w권한으로 열어서 stdout에 저장하여 stdout을 fname포인터로 사용
        printf("Second printf : Hello, OSLAB!!\n");

    exit(0);

}
