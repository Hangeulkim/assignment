#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<signal.h>
#include<string.h>
#include<time.h>
#include"ssu_check.h"

FILE *fp;//log.txt를 가르킬 포인터
char chkpath[BUFLEN];//check디렉토리의 경로를 저장할 배열
char fpath[BUFLEN];//log.txt의 경로를 저장할 배열

void ssu_check(char *dir_name){
//int main(){
    node *before_root;
    node *after_root;
    struct stat fstat;
    int fd,maxfd;

    /*
    char dir_name[BUFLEN];
    getcwd(dir_name,BUFLEN);
    pid_t pid;
    pid=fork();
    if(pid!=0)
        exit(0);
    */

    sprintf(fpath,"%s/%s",dir_name,"log.txt");//log.txt 경로 저장
    sprintf(chkpath,"%s/%s",dir_name,"check");//check디렉토리 경로 저장

    setsid();//새로운 세션을 생성
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);//시그널 연결을 끊음
    maxfd=getdtablesize();//테이블 사이즈를 가져옴

    for(fd=0;fd<maxfd;fd++)
        close(fd);//열려있는 fd를 전부 닫음

    umask(0);//umask를 0으로 설정
    chdir("/");//최상위 디렉토리로 작업경로 이동


    if(chdir("/")<0){//프로세스의 작업 디렉토리를 이동
        fprintf(stderr,"can't open dir %s",dir_name);
        exit(1);
    }
    fd = open("/dev/null",O_RDWR);//표준 입출력과 에러를 다시설정
    dup(0);
    dup(0);


    fp=fopen(fpath,"w+");//기록할 log.txt파일 생성

    lstat(chkpath,&fstat);
    before_root=create_node(fstat,"check",chkpath);//before_root의 맨앞 노드 생성
    make_list(chkpath,before_root,2);//check디렉토리를 1자형 list화하여 before_root에 저장
    before_root->depth=1;


    node *cur=before_root;

    while(1){
        fclose(fp);
        //변경된 사항을 저장하기 위해 닫고 다시열음
        fp=fopen(fpath,"a+");
        after_root=create_node(fstat,"check",chkpath);//before_root의 맨앞 노드 생성
        make_list(chkpath,after_root,2);//check디렉토리를 1자형 list화하여 비교하기위해 after_root에 저장
        after_root->depth=1;//맨 처음 check 디렉토리의 depth는 1로 설정

        write_data(before_root,after_root);//이전 데이터와 현재 데이터를 비교하여 차이점 기록
        free_tree(before_root);//before_root의 데이터를 다 사용하였으므로 해제
        before_root=after_root;//다시 비교하기 위해 before_root에 after_root를 할당

        sleep(1);
    }

    fprintf(stderr, "ssu_check end\n");
    exit(0);
}

long dir_size(node *root){//디렉토리 하부의 파일들의 크기를 구하기 위한 함수
    node *cur=root;
    char buf[FILELEN];
    strcpy(buf,cur->real_path);
    strcat(buf,"/");//폴더의 경로를 확인하기 위해 /를 붙여줌

    long size=0;
    while(cur->next!=NULL){
        cur=cur->next;
        if(strstr(cur->real_path,buf)!=NULL&&!S_ISDIR(cur->data.st_mode)){//내부 파일인 경우 size를 더해줌
            size+=cur->data.st_size;
        }
    }
    return size;
}

void size_list(node *root, int depth){//depth에 맞춰 list를 보여주는 함수
    node *cur=root;
    long size=0;
    printf("\n");
    while(1){
        if(cur->depth<=depth){
            if(S_ISDIR(cur->data.st_mode)){//디렉토리 일경우 하부 파일들의 사이즈를 구해야하므로 따로 구분
                size=dir_size(cur);
            }
            else{
                size=cur->data.st_size;
            }
            printf("%ld\t\t%s%s\n",size,"./",strstr(cur->real_path,"check"));//파일 데이터 출력
        }

        if(cur->next!=NULL)
            cur=cur->next;//다음칸으로 이동
        else
            break;//없으므로 종료
    }
}

void write_data(node *root1, node *root2){//변경사항을 파일에 쓰는 함수
    char date_str[BUFLEN];
    char tmp[BUFLEN];
    node *cur1;
    node *cur2;
    struct tm *when;
    time_t temp;

    cur1=root1;

    while(cur1->next!=NULL){
        cur1=cur1->next;
        cur2=root2;
        int fflag=false;//찾았는지를 확인하는 플래그

        while(cur2->next!=NULL){
            cur2=cur2->next;

            if(cur1->data.st_ino==cur2->data.st_ino||!strcmp(cur1->realative_path,cur2->realative_path)){//이전에 있던 파일인지 확인
                fflag=true;//찾았으므로 플래그 true 설정
                if(cur1->data.st_mtime!=cur2->data.st_mtime){//수정된 시간이 다르므로 modify 작동
                    when=localtime(&cur2->data.st_mtime);
                    strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
                    fprintf(fp,"%s [modify_%s]\n",date_str,cur1->realative_path);
                }
                else if(strcmp(cur1->name,cur2->name)){//vi의 수정시 inode가 바뀌므로 이름으로 체크
                    temp=time(NULL);
                    when=localtime(&temp);
                    strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
                    fprintf(fp,"%s [modify_%s]\n",date_str,cur1->realative_path);
                }
                break;
            }
        }
        if(!fflag){//후의 파일목록에 없으므로 지운것을 의미 delete 작동
            temp=time(NULL);
            when=localtime(&temp);
            strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
            fprintf(fp,"%s [delete_%s]\n",date_str,cur1->realative_path);
        }
    }
    cur2=root2;

    while(cur2->next!=NULL){
        cur2=cur2->next;
        int fflag=false;//찾았는지를 확인하는 플래그

        cur1=root1;
        while(cur1->next!=NULL){
            cur1=cur1->next;

            if(!strcmp(cur1->realative_path,cur2->realative_path)||cur1->data.st_ino==cur2->data.st_ino){
                fflag=true;//vi의 수정시 inode가 바뀌므로 상대경로로 체크
                break;
            }
        }
        if(!fflag){//전의 폴더에서 못찾은 것이므로 새로 생긴 것임을 의미 create 작동
            when=localtime(&cur2->data.st_ctime);
            strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
            fprintf(fp,"%s [create_%s]\n",date_str,cur2->realative_path);
        }
    }
}


node *create_node(struct stat input, char *path,char *r_path){
    //노드 생성함수
    node *new;
    char *ptr;
    new=(node*)malloc(sizeof(node));//새로운 노드를 생성

    new->next=NULL;
    new->prev=NULL;
    new->child=NULL;
    new->parent=NULL;

    new->data=input;

    ptr=strstr(r_path,"/check");
    strcat(new->realative_path,ptr+strlen("/check"));//check의 상대 경로 삽입
    strcpy(new->real_path,r_path);//절대 경로 삽입
    strcpy(new->name,path);//현재 이름을 삽입

    return new;
}

void print_tree(node *root){//tree를 보여주는 함수
    node *cur;
    cur = root;

    printf("\n");
    while(cur->parent!=NULL||cur->next!=NULL){//부모도 없고 다음 노드도 없는경우 종료
        while(cur->parent!=NULL&&cur->next==NULL){//child의 모든 노드를 검사했을 경우 parent로 이동
            cur=cur->parent;
        }
        if(cur->next==NULL) break;
        cur=cur->next;

        if(cur->parent!=NULL&&cur!=cur->parent->child->next){
            for(int i=1;i<cur->depth;i++){
                printf("\t");
            }
        }
        printf("->%s\t",cur->name);//경로의 이름 출력
        if(cur->child!=NULL&&cur->child->next!=NULL){
            cur=cur->child;//자식이 있을경우 자식노드로 이동
        }
        else{
            printf("\n\n");
        }
    }
}

node *make_list(char *path, node *root, int depth){//1자형 리스트를 만드는 함수
    node *cur;
    int files_num;
    int i;
    struct dirent **items;//데이터를 저장할 포인터
    char tmp[BUFLEN];//부모의 이름을 저장하기 위한 임시 배열

    files_num=scandir(path,&items,NULL,alphasort);//현재 폴더의 정보를 가져옴

    cur=root;//cur을 루트로 이동
    for(i=0;i<files_num;i++){
        struct stat input;//파일 상태를 저장하기 위한 struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.과 ..은 패스
        }


        sprintf(ppath,"%s/%s",path,items[i]->d_name);
        lstat(ppath,&input);//작업 파일의 정보를 불러옴
        cur->next=create_node(input,items[i]->d_name,ppath);
        cur->next->prev=cur;
        cur=cur->next;//한칸씩 옮겨가며 체크
        cur->depth=depth+1;

        //디렉토리 일경우 재귀 호출
        if(S_ISDIR(input.st_mode)){
            cur=make_list(ppath,cur,depth+1);//자식과 부모를 연결
        }
        free(items[i]);
    }
    free(items);
    return cur;
}

node *make_tree(char *path,int depth)
{//디렉토리를 트리로 만드는 함수
    node *root;
    node *cur;
    int files_num;
    int i;
    struct dirent **items;//데이터를 저장할 포인터
    char tmp[BUFLEN];//부모의 이름을 저장하기 위한 임시 배열

    root=(node*)malloc(sizeof(node));

    root->parent=NULL;
    root->next=NULL;
    root->prev=NULL;
    root->child=NULL;
    root->depth=depth;

    files_num=scandir(path,&items,NULL,alphasort);//현재 폴더의 정보를 가져옴

    cur=root;//cur을 루트로 이동
    for(i=0;i<files_num;i++){
        struct stat input;//파일 상태를 저장하기 위한 struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.과 ..은 패스
        }


        sprintf(ppath,"%s/%s",path,items[i]->d_name);
        lstat(ppath,&input);//작업 파일의 정보를 불러옴
        cur->next=create_node(input,items[i]->d_name,ppath);
        cur->next->prev=cur;
        cur=cur->next;//한칸씩 옮겨가며 체크
        cur->depth=depth+1;

        //디렉토리 일경우 재귀 호출
        if(S_ISDIR(input.st_mode)){
            cur->child=make_tree(ppath,depth+1);//자식과 부모를 연결
            cur->child->parent=cur;
            node *tmp;//자식 전부를 부모와 연결 하기 위해 사용
            tmp=cur->child;
            while(tmp->next!=NULL){
                tmp=tmp->next;
                tmp->parent=cur;
            }
        }
        free(items[i]);
    }
    free(items);
    return root;
}

void free_tree(node *root)//가장 우측의 맨 아래쪽 자식부터 할당 해제
{//트리 삭제 함수
    node* cur=root;
    node *tmp;
    while(cur->next!=NULL)//cur을 가장 우측으로 이동시킴
        cur=cur->next;

    while(cur->prev!=NULL){
        if(cur->child!=NULL){
            free_tree(cur->child);
            cur->child=NULL;
        }
        tmp=cur;
        cur=cur->prev;

        tmp->parent=NULL;//노드 정보 삭제
        tmp->next=NULL;
        tmp->prev=NULL;
        memset(tmp->name,0,FILELEN);
        memset(tmp->real_path,0,FILELEN);
        memset(tmp->realative_path,0,FILELEN);
        tmp->depth=0;

        cur->next=NULL;
        free(tmp);
    }
    cur->parent=NULL;
    cur->child=NULL;
    cur->prev=NULL;
    cur->next=NULL;
    memset(cur->name,0,FILELEN);
    memset(cur->real_path,0,FILELEN);
    memset(cur->realative_path,0,FILELEN);
    cur->depth=0;

    free(cur);
}
