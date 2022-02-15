#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int main(int argc,char *argv[]){
    struct stat file_info;
    char *str;
    int i;

    for(i =1;i<argc;i++){//들어온 인자 전부 체크
        printf("name = %s, type = ",argv[i]);//이름 출력
        if(lstat(argv[i],&file_info)<0){//심볼릭 링크 파일 자체를 리턴
            fprintf(stderr,"lstat error\n");
            continue;
        }
        
        if(S_ISREG(file_info.st_mode))//일반 파일인지 확인
            str="regular";
        else if(S_ISDIR(file_info.st_mode))//디렉토리 파일인지 확인
            str="directory";
        else if(S_ISCHR(file_info.st_mode))//문자 디바이스 인지 확인
            str="character special";
        else if(S_ISBLK(file_info.st_mode))//블록 디바이스 인지 확인
            str="block special";
        else if(S_ISFIFO(file_info.st_mode))//pipe 파일인지 확인
            str="FIFO";
        else if(S_ISLNK(file_info.st_mode))//심볼릭 링크 파일인지 확인
            str="symbolic link";
        else if(S_ISSOCK(file_info.st_mode))//소켓 파일인지 확인
            str="socket";
        else
            str="unknown mode";
        printf("%s\n",str);
    }
    exit(0);
}


