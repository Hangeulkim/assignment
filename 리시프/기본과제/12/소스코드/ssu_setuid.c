#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    char line[256];
    FILE *fp;
    int uid;

    if(argc<2){//인자를 올바르게 주었는지 체크
        fprintf(stderr,"usage: %s file_name\n", argv[0]);
        exit(1);
    }

    printf("initially uid = %d and euid = %d\n",getuid(),geteuid());//uid 와 euid출력
    fp=fopen(argv[1],"r");

    if(fp==NULL){//파일이 제대로 열렸는지 체크
        fprintf(stderr,"first open error for %s\n",argv[1]);
        exit(1);
    }
    else{
        printf("first open successful:\n");//열기 성공

        while(fgets(line,255,fp)!=NULL)
            fputs(line,stdout);//파일 내용을 라인 하나씩 읽어와 출력

        fclose(fp);
    }
    setuid(uid=getuid());
    printf("after setuid(%d):\n uid=%d and euid=%d\n",uid,getuid(),geteuid());//setuid를 진행한이후 uid 와 euid출력
    fp=fopen(argv[1],"r");//파일을 다시 오픈

    if(fp==NULL){
        fprintf(stderr,"second open error for %s\n",argv[1]);
        exit(1);
    }
    else{
        printf("second open successful:\n");

        while(fgets(line,255,fp)!=NULL)//파일을 한줄씩 읽어와 출력
            fputs(line,stdout);

        fclose(fp);
    }
    exit(0);
}
