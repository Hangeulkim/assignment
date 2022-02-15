#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<limits.h>
#include<string.h>
#include<sys/stat.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED 1024

#ifdef LINE_MAX
#define LINE_MAX 2048
#endif

char *pathname;
char command[LINE_MAX],grep_cmd[LINE_MAX];

int ssu_do_grep(void){
    struct dirent *dirp;
    struct stat statbuf;
    char *ptr;
    DIR *dp;

    if(lstat(pathname,&statbuf)<0){//statbuf에 pathname의 stat정보를 저장
        fprintf(stderr,"lstat error for %s\n",pathname);
        return 0;
    }

    if(S_ISDIR(statbuf.st_mode)==0){//statbuf에 저장된 데이터가 dir인지 체크
        sprintf(command, "%s %s",grep_cmd,pathname);
        printf("%s : \n",pathname);
        system(command);//command 실행
        return 0;
    }

    ptr = pathname+strlen(pathname);
    *ptr++='/';//경로를 나타내기 위해 /추가
    *ptr='\0';//마지막 널문자 추가

    if((dp=opendir(pathname))==NULL){//pathname을 열수 있는지 체크
        fprintf(stderr,"opendir error for %s\n",pathname);
        return 0;
    }

    while((dirp = readdir(dp))!=NULL)//dp에서 파일 정보를 하나씩 읽어옴
        if(strcmp(dirp->d_name,".")&&strcmp(dirp->d_name,"..")){//이름이 . 이나 ..이 아닌경우
            strcpy(ptr,dirp->d_name);//ptr에 d_name을 복사

            if(ssu_do_grep()<0)//ssi_do_grep를 다시 실행하여 체크
                break;
        }
    ptr[-1]=0;
    closedir(dp);
    return 0;
}

void ssu_make_grep(int argc, char *argv[]){
    int i;
    strcpy(grep_cmd," grep");//grep_cmd에 grep복사

    for(i=1;i<argc-1;i++){//들어온 인자 체크
        strcat(grep_cmd, " ");
        strcat(grep_cmd,argv[i]);//공백 한칸 후 들어온 인자를 뒤에 붙여넣음
    }
}

int main(int argc,char *argv[])
{
    if(argc<2){//인자를 제대로 줬는지 체크
        fprintf(stderr,"usage: %s <-CVbchilnsvmx> <-num> <-A num> <-B num> <-f file> \n"
                " <-e> expr <directory>\n",argv[0]);
        exit(1);
    }

    if(pathmax==0){//pathmax가 0일경우
        if((pathmax = pathconf("/",_PC_PATH_MAX))<0)//한계값 저장
            pathmax=MAX_PATH_GUESSED;
        else
            pathmax++;
    }

    if((pathname = (char*)malloc(pathmax+1))==NULL){//pathname을 pathmax+1만큼 동적 할당
        fprintf(stderr,"malloc error\n");
        exit(1);
    }

    strcpy(pathname, argv[argc-1]);//마지막 인자를 pathname에 복사
    ssu_make_grep(argc, argv);
    ssu_do_grep();//grep해온뒤 grep 실행
    exit(0);
}
