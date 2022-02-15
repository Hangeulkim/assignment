#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include"ssu_crond.h"

char fpath[BUF];//작업 경로를 저장할 배열
char logpath[BUF];//log파일 경로를 저장할 배열
int start[10]={0,0,1,1,0};
int end[10]={59,23,31,12,6};

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

struct timeval begin_t, end_t;//시작 시간과 종료시간을 기록

int main()
{
    int fd,maxfd;
    int i=0,thr_id;
    FILE *fp,*logfp;
    char buf[BUF];
    char tmp[BUF];
    node *b_root,*a_root;
    node *cur;
    time_t t;
    pthread_t p_thread[10001];

    gettimeofday(&begin_t, NULL);//시작시각 저장

    getcwd(fpath,BUF);
    sprintf(logpath,"%s/%s",fpath,"ssu_crontab_log");
    sprintf(fpath,"%s/%s",fpath,"ssu_crontab_file");


    if(fork()!=0){//데몬 프로세스 실행
        exit(0);
    }



    setsid();//새로운 세션을 생성
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);//시그널 연결을 끊음
    maxfd=getdtablesize();//테이블 사이즈를 가져옴

    for(fd=0;fd<maxfd;fd++)
        close(fd);//열려있는 fd를 전부 닫음

    umask(0);//umask를 0으로 설정

    fd = open("/dev/null",O_RDWR);//표준 입출력과 에러를 다시설정
    dup(0);
    dup(0);

    b_root=(node*)malloc(sizeof(node));//시작 루트를 제작
    b_root->next=NULL;
    b_root->prev=NULL;

    while(1){
        fp=fopen(fpath,"r+");//읽기 권한으로 파일을 가져옴
        if(fp==NULL)
            fp=fopen(fpath,"w+");//없을 경우 새로 생성
        a_root=make_list(fp);//안에 있는 데이터를 노드화 시킴
        fclose(fp);

        dif_node(a_root,b_root);//삭제된 명령어를 b_root에서 삭제시키고 추가된 명령어를 b_root에 추가시킴

        cur=b_root;

        i=0;
        while(cur->next!=NULL){//모든 노드를 순회하며 명령어를 실행할 시간이 되었을 경우 쓰레드를 생성하여 그 쓰레드에서 실행하도록 만듦
            cur=cur->next;
            if(cnt_chk(cur)){
                logfp=fopen(logpath,"a+");
                thr_id=pthread_create(&p_thread[i++],NULL,run,(void*)&cur->com);
                t=time(NULL);
                strcpy(tmp,ctime(&t));
                tmp[strlen(tmp)-1]='\0';
                fprintf(logfp,"[%s] run %s\n",tmp,cur->com);
                fclose(logfp);
            }
        }


        del_all(a_root);
        for(int j=0;j<i;j++){
            pthread_join(p_thread[j],NULL);//모든 쓰레드의 작업이 종료될때까지 대기
        }
        sleep(59);
    }
    del_all(b_root);
    gettimeofday(&end_t, NULL);//끝난시각 저장
    ssu_runtime(&begin_t, &end_t);//걸린시간 구하는 함수 실행
    exit(0);
}

node *make_list(FILE *fp)
{
    node *root,*cur;
    char buf[BUF];

    root=(node *)malloc(sizeof(node));
    memset(root,0,sizeof(node));//루트 정보를 초기화
    root->next=NULL;
    root->prev=NULL;
    cur=root;
    while(!feof(fp)){
        memset(buf,'\0',BUF);
        fgets(buf,BUF,fp);
        if(buf[0]=='\0'||buf[0]=='\n')
            break;
        cur->next=add_node(buf);//입력된 명령어를 노드화 시킴
        cur->next->prev=cur;
        cur=cur->next;
    }
    return root;
}

int cnt_chk(node *cur)
{
    node *rnode=cur;
    struct tm time_tm;
    time_t now;

    now=time(NULL);
    time_tm=*localtime(&now);//현재 시간의 구조를 가져옴

    if(rnode->data.min.time[time_tm.tm_min]==TRUE){//분의 조건 체크
        if(rnode->data.hour.time[time_tm.tm_hour]==TRUE){//시간의 조건 체크
            if(rnode->data.day.time[time_tm.tm_mday]==TRUE){//일의 조건 체크
                if(rnode->data.mon.time[time_tm.tm_mon+1]==TRUE){//월의 조건 체크
                    if(rnode->data.seven.time[time_tm.tm_wday]==TRUE){//요일의 조건 체크
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

void *run(void *arg){
    char com[BUF];
    sprintf(com,"%s\n",(char*)arg);//명령어를 옮김
    system(com);//명령어 실행
    return (void *)0;
}

node *add_node(char *str){
    node *cur;
    char buf[BUF];
    char digit[BUF];
    char *ptr;
    int cnt=0,range,a,tmp,sFlag;

    cur=(node*)malloc(sizeof(node));//데이터를 넣을 노드를 생성
    cur->next=NULL;
    strcpy(cur->all,str);
    strcpy(buf,str);
    cur->data.start=time(NULL);

    ptr=strtok(buf," ");//분 데이터 입력
    a=-1;
    range=-1;
    tmp=-1;
    sFlag=FALSE;
    for(int i=0;i<strlen(ptr);i++){
        if(ptr[i]=='*'){//*가 있는 경우
            a=start[cnt];
            range=end[cnt];
        }
        else if(ptr[i]>='0'&&ptr[i]<='9'){//숫자가 있는 경우
            a=atoi(&ptr[i]);
            range=a;
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",a);
            i+=strlen(digit)-1;
        }
        else if(ptr[i]=='-'){//-가 있는 경우
            if(a==-1)
                break;
            range=atoi(&ptr[i+1]);
            if(range<a){
                int tmp_num=a;
                a=range;
                range=tmp_num;
            }
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",range);
            i+=strlen(digit);
        }
        else if(ptr[i]==','){//,가 있는 경우
            if(sFlag){
                a=-1;
                tmp=-1;
                range=-1;
                sFlag=FALSE;
                continue;
            }

            for(int x=a;x<=range;x++){//시작점 부터 범위까지 자동으로 입력
                cur->data.min.time[x]=TRUE;
            }
        }
        else if(ptr[i]=='/'){// /가 있는 경우
            tmp=atoi(&ptr[i+1]);
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",tmp);//주기를 받아옴
            i+=strlen(digit);
            sFlag=TRUE;
            int tmp_cnt=tmp;
            for(int x=a;x<=range;x++){//범위 내에서의 카운트를 작동시켜 0이 될경우 TRUE로 값을 변경시킴
                tmp_cnt--;
                if(tmp_cnt<=0){
                    cur->data.min.time[x]=TRUE;
                    tmp_cnt=tmp;
                }
            }
        }
    }
    for(int x=a;!sFlag&&x<=range;x++){//시작점 부터 범위까지 자동으로 입력
        cur->data.min.time[x]=TRUE;
    }

    ptr=strtok(NULL," ");
    cnt++;
    a=-1;
    range=-1;
    tmp=-1;
    sFlag=FALSE;
    for(int i=0;i<strlen(ptr);i++){
        if(ptr[i]=='*'){//*가 있는 경우
            a=start[cnt];
            range=end[cnt];
        }
        else if(ptr[i]>='0'&&ptr[i]<='9'){//숫자가 있는 경우
            a=atoi(&ptr[i]);
            range=a;
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",a);
            i+=strlen(digit)-1;
        }
        else if(ptr[i]=='-'){//-가 있는 경우
            if(a==-1)
                break;
            range=atoi(&ptr[i+1]);
            if(range<a){
                int tmp_num=a;
                a=range;
                range=tmp_num;
            }
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",range);
            i+=strlen(digit);
        }
        else if(ptr[i]==','){//,가 있는 경우
            if(sFlag){
                a=-1;
                tmp=-1;
                range=-1;
                sFlag=FALSE;
                continue;
            }

            for(int x=a;x<=range;x++){//시작점 부터 범위까지 자동으로 입력
                cur->data.hour.time[x]=TRUE;
            }
        }
        else if(ptr[i]=='/'){// /가 있는 경우
            tmp=atoi(&ptr[i+1]);
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",tmp);
            i+=strlen(digit);
            sFlag=TRUE;
            int tmp_cnt=tmp;
            for(int x=a;x<=range;x++){
                tmp_cnt--;
                if(tmp_cnt<=0){
                    cur->data.hour.time[x]=TRUE;
                    tmp_cnt=tmp;
                }
            }
        }
    }
    for(int x=a;!sFlag&&x<=range;x++){//시작점 부터 범위까지 자동으로 입력
        cur->data.hour.time[x]=TRUE;
    }


    ptr=strtok(NULL," ");
    cnt++;
    a=-1;
    range=-1;
    tmp=-1;
    sFlag=FALSE;
    for(int i=0;i<strlen(ptr);i++){
        if(ptr[i]=='*'){//*가 있는 경우
            a=start[cnt];
            range=end[cnt];
        }
        else if(ptr[i]>='0'&&ptr[i]<='9'){//숫자가 있는 경우
            a=atoi(&ptr[i]);
            range=a;
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",a);
            i+=strlen(digit)-1;
        }
        else if(ptr[i]=='-'){//-가 있는 경우
            if(a==-1)
                break;
            range=atoi(&ptr[i+1]);
            if(range<a){
                int tmp_num=a;
                a=range;
                range=tmp_num;
            }
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",range);
            i+=strlen(digit);
        }
        else if(ptr[i]==','){//,가 있는 경우
            if(sFlag){
                a=-1;
                tmp=-1;
                range=-1;
                sFlag=FALSE;
                continue;
            }

            for(int x=a;x<=range;x++){//시작점 부터 범위까지 자동으로 입력
                cur->data.day.time[x]=TRUE;
            }
        }
        else if(ptr[i]=='/'){// /가 있는 경우
            tmp=atoi(&ptr[i+1]);
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",tmp);
            i+=strlen(digit);
            sFlag=TRUE;
            int tmp_cnt=tmp;
            for(int x=a;x<=range;x++){
                tmp_cnt--;
                if(tmp_cnt<=0){
                    cur->data.day.time[x]=TRUE;
                    tmp_cnt=tmp;
                }
            }
        }
    }
    for(int x=a;!sFlag&&x<=range;x++){//시작점 부터 범위까지 자동으로 입력
        cur->data.day.time[x]=TRUE;
    }


    ptr=strtok(NULL," ");
    cnt++;
    a=-1;
    range=-1;
    tmp=-1;
    sFlag=FALSE;
    for(int i=0;i<strlen(ptr);i++){
        if(ptr[i]=='*'){//*가 있는 경우
            a=start[cnt];
            range=end[cnt];
        }
        else if(ptr[i]>='0'&&ptr[i]<='9'){//숫자가 있는 경우
            a=atoi(&ptr[i]);
            range=a;
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",a);
            i+=strlen(digit)-1;
        }
        else if(ptr[i]=='-'){//-가 있는 경우
            if(a==-1)
                break;
            range=atoi(&ptr[i+1]);
            if(range<a){
                int tmp_num=a;
                a=range;
                range=tmp_num;
            }
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",range);
            i+=strlen(digit);
        }
        else if(ptr[i]==','){//,가 있는 경우
            if(sFlag){
                a=-1;
                tmp=-1;
                range=-1;
                sFlag=FALSE;
                continue;
            }

            for(int x=a;x<=range;x++){//시작점 부터 범위까지 자동으로 입력
                cur->data.mon.time[x]=TRUE;
            }
        }
        else if(ptr[i]=='/'){// /가 있는 경우
            tmp=atoi(&ptr[i+1]);
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",tmp);
            i+=strlen(digit);
            sFlag=TRUE;
            int tmp_cnt=tmp;
            for(int x=a;x<=range;x++){
                tmp_cnt--;
                if(tmp_cnt<=0){
                    cur->data.mon.time[x]=TRUE;
                    tmp_cnt=tmp;
                }
            }
        }
    }
    for(int x=a;!sFlag&&x<=range;x++){//시작점 부터 범위까지 자동으로 입력
        cur->data.mon.time[x]=TRUE;
    }


    ptr=strtok(NULL," ");
    cnt++;
    a=-1;
    range=-1;
    tmp=-1;
    sFlag=FALSE;
    for(int i=0;i<strlen(ptr);i++){
        if(ptr[i]=='*'){//*가 있는 경우
            a=start[cnt];
            range=end[cnt];
        }
        else if(ptr[i]>='0'&&ptr[i]<='9'){//숫자가 있는 경우
            a=atoi(&ptr[i]);
            range=a;
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",a);
            i+=strlen(digit)-1;
        }
        else if(ptr[i]=='-'){//-가 있는 경우
            if(a==-1)
                break;
            range=atoi(&ptr[i+1]);
            if(range<a){
                int tmp_num=a;
                a=range;
                range=tmp_num;
            }
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",range);
            i+=strlen(digit);
        }
        else if(ptr[i]==','){//,가 있는 경우
            if(sFlag){
                a=-1;
                tmp=-1;
                range=-1;
                sFlag=FALSE;
                continue;
            }

            for(int x=a;x<=range;x++){//시작점 부터 범위까지 자동으로 입력
                cur->data.seven.time[x]=TRUE;
            }
        }
        else if(ptr[i]=='/'){// /가 있는 경우
            tmp=atoi(&ptr[i+1]);
            memset(digit,'\0',BUF);
            sprintf(digit,"%d",tmp);
            i+=strlen(digit);
            sFlag=TRUE;
            int tmp_cnt=tmp;
            for(int x=a;x<=range;x++){
                tmp_cnt--;
                if(tmp_cnt<=0){
                    cur->data.seven.time[x]=TRUE;
                    tmp_cnt=tmp;
                }
            }
        }
    }
    for(int x=a;!sFlag&&x<=range;x++){//시작점 부터 범위까지 자동으로 입력
        cur->data.seven.time[x]=TRUE;
    }


    ptr=strtok(NULL,"\n");//명령어 부분을 잘라냄
    strcpy(cur->com,ptr);
    return cur;
}

node *del_node(node *cur){
    node *tmp;
    node *return_node;

    return_node=cur->prev;
    tmp=cur;//tmp를 이용하여 cur의 다음노드와 이전노드의 연결을 바꿔줌
    if(tmp->next!=NULL){
        tmp->next->prev=tmp->prev;
    }
    tmp->prev->next=tmp->next;
    tmp->next=NULL;
    tmp->prev=NULL;

    free(tmp);//tmp를 해제
    return return_node;//tmp->prev의 노드를 리턴
}

void dif_node(node *a_root, node *b_root){
    node *a_cur,*b_cur;

    a_cur=a_root;
    b_cur=b_root;
    while(b_cur->next!=NULL){
        b_cur=b_cur->next;
        a_cur=a_root;
        int fFlag=FALSE;
        while(a_cur->next!=NULL){
            a_cur=a_cur->next;
            if(!strcmp(a_cur->all,b_cur->all)){//같은 명령어 일경우 이미 있었던 것이므로 무시
                fFlag=TRUE;
                break;
            }
        }
        if(!fFlag){
            b_cur=del_node(b_cur);//이후의 명령어 목록에 존재하지 않으므로 삭제
        }
    }

    a_cur=a_root;
    b_cur=b_root;
    while(a_cur->next!=NULL){
        a_cur=a_cur->next;
        b_cur=b_root;
        int fFlag=FALSE;
        while(b_cur->next!=NULL){
            b_cur=b_cur->next;
            if(!strcmp(a_cur->all,b_cur->all)){//이후의 리스트에 있는 경우 이전엔 없던 것이므로 추가시킴
                fFlag=TRUE;
                break;
            }
        }
        if(!fFlag){
            node *tmp;//노드를 root다음에 추가
            tmp=add_node(a_cur->all);
            tmp->next=b_root->next;
            tmp->prev=b_root;
            if(b_root->next!=NULL){
                b_root->next->prev=tmp;
            }
            b_root->next=tmp;
        }
    }
}

void del_all(node *root){
    node *del;
    node *cur=root;

    while(cur->next!=NULL){
        del=cur;
        cur=cur->next;
        del->next=NULL;
        del->prev=NULL;
        free(del);//del의 이전과 이후 노드의 연결을 끊은 다음 이동시킴
    }
    cur->next=NULL;
    cur->prev=NULL;
    free(cur);//마지막으로 남은 노드의 연결을 해제
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
