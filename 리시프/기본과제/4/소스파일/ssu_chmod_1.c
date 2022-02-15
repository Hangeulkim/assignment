#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int main(void){
    struct stat statbuf;
    char *fname1="ssu_file1";
    char *fname2="ssu_file2";

    if(stat(fname1, &statbuf)<0)//파일에 대한 정보를 확인
        fprintf(stderr,"stat error %s\n",fname1);//실패했을시 에러

    if(chmod(fname1, (statbuf.st_mode & ~S_IXGRP)|S_ISUID)<0)//fname1의 접근 권한을 변경
        fprintf(stderr,"chmod error %s\n",fname1);//접근 권한을 변경하는데 실패함

    if(chmod(fname2,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH|S_IXOTH)<0)//ssu_file2의 접근권한을 0645로 변경
        fprintf(stderr,"chmod errer %s\n",fname2);//접근 권한을 변경하는데 실패함

    exit(0);
}

