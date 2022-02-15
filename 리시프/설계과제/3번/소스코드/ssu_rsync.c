#include "ssu_rsync.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<sys/types.h>
#include<dirent.h>
#include<utime.h>
#include <signal.h>

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

struct timeval begin_t, end_t;//시작 시간과 종료시간을 기록

char dst[BUF];//dst위치를 저장할 버퍼
char src[BUF];//src위치를 저장할 버퍼
char log_path[BUF]="ssu_rsync_log";//log파일 위치를 기록할 버퍼
int rFlag=FALSE;//r옵션 플래그
int tFlag=FALSE;//t옵션 플래그
int mFlag=FALSE;//m옵션 플래그

node *src_root=NULL;
node *dst_root=NULL;
sync_data *sync_root=NULL;
sync_data *loc_sync=NULL;

FILE *log_file;

int sFlag=FALSE;//파일을 옮겼는지 확인하기 위함

static void signal_handler(int signo);

int main(int argc, char *argv[]){
    char tar_path[BUF];

    gettimeofday(&begin_t, NULL);//시작시각 저장
    memset(src,'\0',BUF);
    memset(dst,'\0',BUF);//src와 dst를 초기화
    check_option(argc, argv);//옵션을 체크
    check_err(src,dst);//에러가 있는지 체크
    signal(SIGINT, signal_handler);//SIGINT가 발생할 시 동기화를 취소시키는 작업
    run_sync(src,dst);//동기화 데이터를 가져옴
    if(!mFlag&&sync_root->files==0){//동기화 시킬 데이터가 없으므로 종료시킴
        exit(0);
    }

    if(tFlag){
        tOption();//압축하여 동기화 시키고 그에 대한 로그를 작성
    }
    else{
        write_sync();//가져온 데이터들을 동기화시킴
    }

    signal(SIGINT,SIG_IGN);//동기화 작업을 끝냈으므로 시그널을 무시하게 만듦
    delete_tmp();//동기화가 취소 됐을때를 대비해 만든 tmp파일들을 제거

    log_file=fopen(log_path,"a");
    time_t t;
    t=time(NULL);
    char tmp[BUF];
    strcpy(tmp,ctime(&t));
    tmp[strlen(tmp)-1]='\0';
    fprintf(log_file,"[%s] ssu_rsync ",tmp);//시간을 기록
    if(rFlag)
        fprintf(log_file,"-r ");//어떠한 옵션으로 실행했는지 기록
    if(tFlag)
        fprintf(log_file,"-t ");
    if(mFlag)
        fprintf(log_file,"-m ");
    fprintf(log_file,"src dst\n");
    if(tFlag){
        sprintf(tar_path,"%s/%s",src,"tmp.tar");
        t_write_log(tar_path);//압축하여 동기화 시킨 로그를 작성
    }
    else{
        write_log();//동기화 시킨 파일 로그를 남김
    }
    if(mFlag){
        mOption();//삭제시키고 그에 대한 파일을 정보를 기록
    }
    fclose(log_file);//로그파일 기록 완료
    gettimeofday(&end_t, NULL);//끝난시각 저장
    ssu_runtime(&begin_t, &end_t);//걸린시간 구하는 함수 실행
    exit(0);
}

void check_err(char *src,char *dst){
    struct stat statbuf;
    stat(dst,&statbuf);
    if(access(src,F_OK)==-1||access(src,R_OK)==-1||access(src,W_OK)==-1||access(dst,F_OK)==-1||access(dst,R_OK)==-1||access(dst,W_OK)==-1||!S_ISDIR(statbuf.st_mode)){
        fprintf(stderr, "ssu_rsync [option] <src> <dst>\n");
        exit(0);
    }//파일의 존재유무와 접근권한 및 dst가 디렉토리 인지 확인
}

void check_option(int argc, char *argv[]){
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"-r"))//r옵션 체크
            rFlag=TRUE;
        else if(!strcmp(argv[i],"-t"))//t옵션 체크
            tFlag=TRUE;
        else if(!strcmp(argv[i],"-m"))//m옵션 체크
            mFlag=TRUE;
        else{
            if(src[0]!='\0'){//src에 이미 인자가 있는경우 dst에 복사
                realpath(argv[i],dst);
            }
            else{
                realpath(argv[i],src);//src에 복사
            }
        }
    }
}

void run_sync(char *src, char *dst){
    struct stat fstat;
    lstat(src,&fstat);
    src_root=create_node(fstat,src);//before_root의 맨앞 노드 생성
    if(S_ISDIR(fstat.st_mode)){
        make_list(src,src_root);//디렉토리를 1자형 list화하여 src_root에 저장
    }

    lstat(dst,&fstat);
    dst_root=create_node(fstat,dst);//before_root의 맨앞 노드 생성
    make_list(dst,dst_root);//디렉토리를 1자형 list화하여 dst_root에 저장

    rsync_data(src_root, dst_root);
}

void rsync_data(node *src_root, node *dst_root){//데이터가 일치하는게 있는지 확인하고 없을경우 동기화
    node *cur_src=src_root;
    node *cur_dst=dst_root;
    sync_data *cur_sync;

    sync_root=(sync_data*)malloc(sizeof(sync_data));
    sync_root->next=NULL;
    sync_root->prev=NULL;
    sync_root->same=FALSE;
    sync_root->files=0;
    cur_sync=sync_root;

    if(!S_ISDIR(cur_src->data.st_mode)){//src인자가 디렉토리가 아닌 경우
        int fFlag=FALSE;
        while(cur_dst->next!=NULL){
            cur_dst=cur_dst->next;
            if(!strcmp(strrchr(cur_src->real_path,'/')+1,cur_dst->real_path+strlen(dst)+1)&&cur_src->data.st_size==cur_dst->data.st_size&&cur_src->data.st_mtime==cur_dst->data.st_mtime){//파일 이름, 사이즈, 수정시간이 같은 것이 존재하는지 체크
                fFlag=TRUE;
                break;
            }
        }
        if(!fFlag){
            cur_sync->next=(sync_data*)malloc(sizeof(sync_data));
            cur_sync->next->prev=cur_sync;
            cur_sync=cur_sync->next;
            cur_sync->next=NULL;
            strcpy(cur_sync->src_path,cur_src->real_path);
            sprintf(cur_sync->dst_path,"%s/%s",dst,cur_src->real_path+strlen(src)+1);
            cur_sync->data=cur_src->data;
            if(access(cur_sync->dst_path,F_OK)==0){
                cur_sync->same=TRUE;
            }
            else
                cur_sync->same=FALSE;
            sync_root->files+=1;
        }//동기화 시켜야하는데 필요한 데이터 정보를 sync struct에 저장
    }
    else{
        while(cur_src->next!=NULL){
            int fFlag=FALSE;
            cur_src=cur_src->next;
            cur_dst=dst_root;
            while(cur_dst->next!=NULL){
                cur_dst=cur_dst->next;
                if(!strcmp(cur_src->real_path+strlen(src)+1,cur_dst->real_path+strlen(dst)+1)&&cur_src->data.st_size==cur_dst->data.st_size&&cur_src->data.st_mtime==cur_dst->data.st_mtime){//파일 이름, 사이즈, 수정시간이 같은 것이 존재하는지 체크
                    fFlag=TRUE;
                    break;
                }
            }
            if(!fFlag){
                cur_sync->next=(sync_data*)malloc(sizeof(sync_data));
                cur_sync->next->prev=cur_sync;
                cur_sync=cur_sync->next;
                cur_sync->next=NULL;
                strcpy(cur_sync->src_path,cur_src->real_path);
                sprintf(cur_sync->dst_path,"%s/%s",dst,cur_src->real_path+strlen(src)+1);
                cur_sync->data=cur_src->data;
                if(access(cur_sync->dst_path,F_OK)==0&&!S_ISDIR(cur_sync->data.st_mode)){
                    cur_sync->same=TRUE;
                }
                else
                    cur_sync->same=FALSE;
                if(!S_ISDIR(cur_src->data.st_mode)){
                    sync_root->files+=1;
                }
            }//동기화 시켜야하는데 필요한 데이터 정보를 sync struct에 저장
        }
    }
}

void write_sync(){
    loc_sync=sync_root;
    while(loc_sync->next!=NULL){
        loc_sync=loc_sync->next;
        if(S_ISDIR(loc_sync->data.st_mode)){
            if(!rFlag)
                continue;
            mkdir(loc_sync->dst_path,0777);//파일을 동기화 시키기 위해 폴더를 생성
            continue;
        }
        file_sync(loc_sync);//파일 동기화 진행
    }
}

void delete_tmp(){
    while(loc_sync->prev!=NULL){
        if(loc_sync->same==TRUE){//같은게 존재할경우 임시 tmp파일 삭제
            char tmp[BUF+10];
            sprintf(tmp,"%s.tmp",loc_sync->dst_path);
            remove(tmp);
        }
        loc_sync=loc_sync->prev;
    }
}

void write_log(){
    loc_sync=sync_root;

    while(loc_sync->next!=NULL){
        loc_sync=loc_sync->next;
        if(S_ISDIR(loc_sync->data.st_mode))
            continue;
        fprintf(log_file,"\t%s %ldbytes\n",loc_sync->dst_path+strlen(dst)+1,loc_sync->data.st_size);//동기화 정보를 기록
    }
}

void t_write_log(char *str){
    struct stat data;

    stat(str,&data);
    fprintf(log_file,"\ttotalSize %ldbytes\n",data.st_size);//tar파일 사이즈 기록

    loc_sync=sync_root;

    while(loc_sync->next!=NULL){
        loc_sync=loc_sync->next;
        if(S_ISDIR(loc_sync->data.st_mode))//디렉토리 일경우 패스
            continue;
        fprintf(log_file,"\t%s\n",loc_sync->dst_path+strlen(dst)+1);//동기화 정보를 기록
    }
    remove(str);//압축파일을 삭제
}

void file_sync(sync_data *cur){
    char where[BUF];
    char buf[BUF];
    FILE *in, *out;
    size_t len;
    struct utimbuf attr;
    struct stat input;

    sprintf(where,"%s",cur->dst_path);
    if(access(where,F_OK)==0){
        char tmp[BUF];
        sprintf(tmp,"%s.tmp",where);
        rename(where,tmp);
    }//동기화가 취소되었을 경우를 대비해 기존 파일의 이름을 바꿔놈
    out=fopen(where,"wb");

    sprintf(where,"%s",cur->src_path);
    in=fopen(where,"rb");//동기화 시킬 파일을 엶

    while((len=fread(buf,sizeof(char),BUF,in))>0)
        fwrite(buf,sizeof(char),len,out);//buf에 데이터를 읽어온다음 dst에 경로로 복사 붙여넣기


    stat(cur->src_path,&input);
    attr.actime = input.st_atime;
    attr.modtime = input.st_mtime;
    utime(cur->dst_path, &attr);//수정시간과 접근 시간 복사
    chmod(cur->dst_path, input.st_mode);//파일의 권한 복사
    fclose(in);
    fclose(out);
}

node *create_node(struct stat input, char *r_path){
    //노드 생성함수
    node *new;
    char *ptr;
    new=(node*)malloc(sizeof(node));//새로운 노드를 생성

    new->next=NULL;
    new->prev=NULL;
    new->data=input;

    strcpy(new->real_path,r_path);//절대 경로 삽입

    return new;
}

node *make_list(char *path, node *root){//1자형 리스트를 만드는 함수
    node *cur;
    int files_num;
    int i;
    struct dirent **items;//데이터를 저장할 포인터

    files_num=scandir(path,&items,NULL,alphasort);//현재 폴더의 정보를 가져옴

    cur=root;//cur을 루트로 이동
    for(i=0;i<files_num;i++){
        struct stat input;//파일 상태를 저장하기 위한 struct
        char ppath[BUF];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.과 ..은 패스
        }


        sprintf(ppath,"%s/%s",path,items[i]->d_name);
        lstat(ppath,&input);//작업 파일의 정보를 불러옴

        cur->next=create_node(input,ppath);
        cur->next->prev=cur;
        cur=cur->next;

        //디렉토리 일경우 재귀 호출
        if(S_ISDIR(input.st_mode)&&rFlag){
            node *tmp_node=cur;
            cur=make_list(ppath,cur);//자식과 부모를 연결
            char tmp[BUF];
            strcpy(tmp,cur->real_path);
            strcat(tmp,"/");
            if(tmp_node->next==NULL){//다음 노드가 없는 경우 폴더에 아무 파일이 없다는 의미 이므로 동기화 대상에서 제외
                cur->prev->next=cur->next;
                node *del_node=cur;
                cur=cur->prev;
                free(del_node);
            }
        }
        free(items[i]);
    }
    free(items);
    return cur;
}

void tOption(){
    char com[COM];
    char origin[BUF];
    struct stat input;

    getcwd(origin,BUF);//src 폴더 안의 파일을 가져오기 위해 경로를 옮겨야 하므로 현재 작업 경로를 저장
    sprintf(com,"tar -cf tmp.tar ");

    chdir(src);//압축할 파일을 가져올 경로로 이동
    loc_sync=sync_root;
    while(loc_sync->next!=NULL){
        loc_sync=loc_sync->next;
        lstat(loc_sync->src_path,&input);
        if(S_ISDIR(input.st_mode))
            continue;
        sprintf(com,"%s%s ",com,strstr(loc_sync->src_path,"src")+4);
    }//동기화 시킬 파일들을 압축
    system(com);//명령어를 실행
    sprintf(com,"%s\n",com);

    memset(com,'\0',COM);
    sprintf(com,"tar -xf tmp.tar -C %s\n",dst);//압축 해제 명령어 실행
    system(com);//명령어를 실행

    chdir(origin);//원래의 작업 경로로 이동
}

void mOption(){
    node *cur_src=src_root;
    node *cur_dst=dst_root;

    while(cur_dst->next!=NULL){
        int fFlag=FALSE;
        cur_dst=cur_dst->next;
        cur_src=src_root;
        while(cur_src->next!=NULL){
            cur_src=cur_src->next;
            if(!strcmp(cur_src->real_path+strlen(src)+1,cur_dst->real_path+strlen(dst)+1)){//파일 이름이 같은 것이 존재하는지 체크
                fFlag=TRUE;
                break;
            }
        }
        if(!fFlag){
            rmdirs(cur_dst->real_path,1);//모든 경로 삭제 함수 실행

            fprintf(log_file,"\t%s delete\n",cur_dst->real_path+strlen(dst)+1);//동기화 정보를 기록
        }
    }
}

int rmdirs(const char *path, int force) {
    DIR * dir_ptr = NULL;
    struct dirent *file = NULL;
    struct stat buf;
    char filename[1024]; /* 목록을 읽을 디렉토리명으로 DIR *를 return 받습니다. */
    if((dir_ptr = opendir(path)) == NULL) { /* path가 디렉토리가 아니라면 삭제하고 종료합니다. */
        return unlink(path);
    } /* 디렉토리의 처음부터 파일 또는 디렉토리명을 순서대로 한개씩 읽습니다. */
    while((file = readdir(dir_ptr)) != NULL) {
        // readdir 읽혀진 파일명 중에 현재 디렉토리를 나타네는 . 도 포함되어 있으므로
        // 무한 반복에 빠지지 않으려면 파일명이 . 이면 skip 해야 함
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            continue;
        }
        sprintf(filename, "%s/%s", path, file->d_name); /* 파일의 속성(파일의 유형, 크기, 생성/변경 시간 등을 얻기 위하여 */
        if(lstat(filename, &buf) == -1) {
            continue;
        }
        if(S_ISDIR(buf.st_mode)) {
            // 검색된 이름의 속성이 디렉토리이면
            /* 검색된 파일이 directory이면 재귀호출로 하위 디렉토리를 다시 검색 */
            if(rmdirs(filename, force) == -1 && !force) {
                return -1;
            }
        }
        else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) {
            // 일반파일 또는 symbolic link 이면
            if(unlink(filename) == -1 && !force) {
                return -1;
            }
        }
    } /* open된 directory 정보를 close 합니다. */
    closedir(dir_ptr);
    return rmdir(path);
}

static void signal_handler(int signo) {
    signal(SIGINT,SIG_IGN);//SIGINT를 무시하게 만듬
    while(loc_sync->prev!=NULL){//진행된 위치이전꺼를 전부 예전 상태로 돌림
        if(loc_sync->same==TRUE){
            char tmp[BUF+10];
            sprintf(tmp,"%s.tmp",loc_sync->dst_path);
            remove(loc_sync->dst_path);
            rename(tmp,loc_sync->dst_path);//tmp파일 화 시켜논 이전 파일을 다시 되돌림
        }
        else{
            remove(loc_sync->dst_path);//동기화 시킨 파일을 삭제
        }
        loc_sync=loc_sync->prev;
    }
    if(tFlag){
        char tmp[BUF];
        sprintf(tmp,"%s/%s",src,"tmp.tar");
        remove(tmp);
    }

    exit(EXIT_SUCCESS);//성공으로 끝냈음을 알림
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{//실행시간 체크 함수
	end_t->tv_sec -= begin_t->tv_sec;//끝난 시각에서 시작시각의 초를 뺌

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}//usec가 시작지점이 큰경우 end의 1초를 usec로 변환

	end_t->tv_usec -= begin_t->tv_usec;//끝난 시각에서 시작시각의 usec를 뺌
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);//걸린 시각을 출력
}
