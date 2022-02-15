#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>

#define DIRECTORY_SIZE MAXNAMLEN

int main(int argc, char *argv[])
{
    struct dirent *dentry;
    struct stat statbuf;
    char filename[DIRECTORY_SIZE+1];
    DIR *dirp;

    if(argc<2){//인자가 정해진 대로 주어졌는지 체크
        fprintf(stderr,"usage: %s <directory>\n",argv[0]);
        exit(1);
    }

    if((dirp=opendir(argv[1]))==NULL||chdir(argv[1])==-1){//디렉토리를 열고 현재 작업디렉토리를 변경
        fprintf(stderr,"opendir, chdir error for %s\n",argv[1]);
        exit(1);
    }

    while((dentry = readdir(dirp))!=NULL){//읽어올 파일이 없을때까지 체크
        if(dentry->d_ino = 0)//inode에 저장된 파일인지 체크
            continue;

        memcpy(filename,dentry->d_name,DIRECTORY_SIZE);//filename에 디렉토리에서 읽어온 파일 이름 저장

        if(stat(filename,&statbuf)==-1){//filname의 stat을 statbuf에 저장
            fprintf(stderr,"stat error for %s\n",filename);
            break;
        }

        if((statbuf.st_mode & S_IFMT)==S_IFREG)//일반 파일인지 체크
            printf("%-14s %ld\n",filename, statbuf.st_size);
        else
            printf("%-14s\n",filename);
    }
    exit(0);
}
