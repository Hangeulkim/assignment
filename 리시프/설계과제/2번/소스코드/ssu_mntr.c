#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<dirent.h>
#include<string.h>
#include<signal.h>
#include<stdio_ext.h>
#include"ssu_mntr.h"
#include"ssu_check.h"

char path[BUFLEN];//기본 작업경로 저장
char trash_path[BUFLEN];//trash 경로 저장
char check_path[BUFLEN];//check 경로 저장
char info_path[BUFLEN];//trash/info 경로 저장
char files_path[BUFLEN];//trash/files 경로 저장

char tmp_str[FILELEN];//delete시 경로를 저장할 임시 배열

long info_byte=0;//info의 상한을 알기위해 선언

int ioption=false;
int roption=false;
int doption=false;
int loption=false;

char startmsg[BUFLEN]="20182602>";//시작 메시지를 기록

info *datas;

void ssu_mntr(){
    getcwd(path,BUFLEN);

    sprintf(check_path,"%s/%s",path,"check");
    mkdir(check_path,0777);//check디렉토리 제작


    if(fork()==0){//데몬 프로세스에서 ssu_check실행
        if(fork()==0){
            ssu_check(path);//감시 프로그램 실행
            exit(0);
        }

        else{
            exit(0);
        }

    }
    else{//부모를 종료시키기 위해 다시한번 fork실행 ssu_mntr 실행
        signal(SIGALRM, delete_files);//delete에서의 시간 확인을 위해 사용
        sprintf(trash_path,"%s/%s",path,"trash");//trash 디렉토리 제작
        mkdir(trash_path,0777);
        sprintf(info_path,"%s/%s",trash_path,"info");//trash/info 디렉토리 제작
        mkdir(info_path,0777);
        sprintf(files_path,"%s/%s",trash_path,"files");//trash/filse 디렉토리 제작
        mkdir(files_path,0777);

        init_info(info_path);//현재 가지고있는 info데이터를 가져옴

        while(1){
            char input[BUFLEN];
            char c;
            memset(input,'\0',BUFLEN);//이전값이나 쓰레기값이 있을 수 있으므로 초기화시킴
            write(STDOUT_FILENO,startmsg,BUFLEN);//20182602>를 출력
            int i=0;
            while(read(STDIN_FILENO,&c,1)>0&&c!='\n'){
                input[i++]=c;
            }//\n을 만날때 까지 받아옴
            __fpurge(stdin);
            input[i]='\0';//문자열의 마지막을 설정

            char tmp[BUFLEN];
            strcpy(tmp,input);
            char *ptr=strtok(tmp," ");
            if(input[0]=='\0')
                continue;

            if(!strcmp(ptr,"delete")){
                ioption=false;
                roption=false;
                chk_option_delete(input);
                delete(input);
            }
            else if(!strcmp(ptr,"size")){
                doption=false;
                chk_option_size(input);
                size(input);
            }
            else if(!strcmp(ptr,"recover")){
                loption=false;
                chk_option_recover(input);
                recover(input);
            }
            else if(!strcmp(input,"tree")||!strcmp(ptr,"tree")){
                tree();
            }
            else if(!strcmp(input,"exit")||!strcmp(ptr,"exit")){
                Exit();
                break;
            }
            else{
                help();
            }
        }
    }
}

void delete_files(){//시간이 됐을경우를 위해
    int fflag=true;
    if(roption){//r옵션이 켜져있을 경우
        if(vfork()==0){
            int c;
            do {
                __fpurge(stdin);
                printf("\nDelete [y/n]? ");
                c=getchar();
                getchar();
                if(c=='n'||c=='N'){
                    fflag=false;
                }
                else if(c=='y'||c=='Y'){
                    fflag=true;
                }
            } while(c!='y'&&c!='Y'&&c!='n'&&c!='N');
        }
        __fpurge(stdin);
        write(STDOUT_FILENO,startmsg,BUFLEN);//작업 도중에 진행되므로 다시한번 시작메시지 출력
    }
    if(fflag)
        delete_files_info();//파일 삭제 함수 실행
}

void delete_files_info(){//파일 삭제 함수 실행
    char d_str[FILELEN];//삭제 시간 저장
    char m_str[FILELEN];//수정 시간 저장
    struct stat fstat;
    time_t now;
    struct tm when;
    char buf[FILELEN];//파일 절대 경로 저장
    char name[FILELEN];//파일명 저장
    char tmp[FILELEN];//files의 저장 위치 저장
    int status;

    strcpy(buf,tmp_str);//이전에 저장해두었던 정보를 buf로 옮겨옴
    time(&now);
    when=*localtime(&now);
    info *cur=datas;//이름이 겹치는게 몇갠지 찾기위함
    int cnt=0;
    strcpy(name,strrchr(buf,'/')+1);//name에 파일의 이름을 저장

    int fflag=false;//넣을 수 있는 이름을 찾았는지 확인
    int jflag=false;//중복 된게 있는지를 보는 플래그

    strcpy(tmp,name);
    while(!fflag){//넣을 수 있는 이름을 찾을때 까지 반복
        fflag=true;
        cur=datas;
        while(cur->next!=NULL){
            cur=cur->next;
            if(!strcmp(strrchr(cur->where,'/')+1,tmp)){
                fflag=false;//이름이 중복되는것이 존재하므로 다시 진행
                sprintf(tmp,"%d_%s",cnt,name);//중복되지 않도록 숫자를 붙여 저장
                jflag=true;
                cnt++;
                break;
            }
        }
    }

    cur->next=(info*)malloc(sizeof(info));
    cur->next->prev=cur;
    cur=cur->next;
    cur->next=NULL;
    strcpy(cur->delete_name,name);//원본의 이름을 저장
    cur->d_time=when;//삭제 시간을 저장

    strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&when);

    lstat(buf,&fstat);//파일의 수정 시각을 저장하기 위해
    cur->m_time=*localtime(&fstat.st_mtime);//수정 시각을 저장


    if(jflag){
        sprintf(name,"%s",tmp);
    }//중복된 파일이 있으므로 파일의 이름 앞에 숫자_를 붙임

    sprintf(tmp,"%s/%s",files_path,name);//원본파일의 이동을 위한 경로를 저장
    strcpy(cur->where,tmp);//원본 파일이 이동할 경로를 복사
    strcpy(cur->real_path,buf);//원본 경로 저장

    rename(cur->real_path,cur->where);//파일을 이동시킴

    sprintf(tmp,"%s/%s",info_path,strrchr(cur->where,'/')+1);


    FILE *tmpfp;
    tmpfp=fopen(tmp,"w+");//삭제 정보를 저장할 파일 생성
    fprintf(tmpfp,"[Trash info]\n%s\n%s\n",cur->delete_name,cur->real_path);//삭제정보를 info에 생성

    fprintf(tmpfp,"D : %s\n",d_str);//삭제 시간 작성

    strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);
    fprintf(tmpfp,"M : %s\n",m_str);//삭제 시간 작성

    fclose(tmpfp);
    lstat(tmp,&fstat);

    info_byte+=fstat.st_size;//2kb가 넘을경우 삭제하기 위해 size를 증가시킴
    sort_info();//오래된 순으로 정렬


    if(info_byte>MAX_BYTE){//2kb를 초과한 경우
        delete_info(datas->next);
    }
}

void init_info(char *info_path){
    int files_num;
    struct dirent **items;//데이터를 저장할 포인터
    FILE *fp;
    char buf[BUFLEN];
    time_t rawtime;
    int i;
    int year,month,day,hour,minute,sec;
    info *cur;

    time(&rawtime);

    datas=(info*)malloc(sizeof(info));//front 노드 제작
    datas->next=NULL;
    datas->prev=NULL;

    files_num=scandir(info_path,&items,NULL,alphasort);//info의 데이터 파일들을 가져옴
    cur=datas;

    for(i=0;i<files_num;i++){
        struct stat input;//파일 상태를 저장하기 위한 struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.과 ..은 패스
        }

        sprintf(ppath,"%s/%s",info_path,items[i]->d_name);
        lstat(ppath,&input);//작업 파일의 정보를 불러옴
        cur->next=(info*)malloc(sizeof(info));
        cur->next->prev=cur;
        cur=cur->next;//한칸씩 옮겨가며 제작
        cur->next=NULL;


        info_byte+=input.st_size;//info의 데이터 상한제한을 위해 진행

        fp=fopen(ppath,"r+");//info파일의 정보를 datas에 저장하기 위해 파일을 엶
        fscanf(fp,"%[^\n]\n",buf);
        fscanf(fp,"%[^\n]\n",cur->delete_name);
        fscanf(fp,"%[^\n]\n",cur->real_path);
        fscanf(fp,"D : %d-%d-%d %d:%d:%d\n",&year,&month,&day,&hour,&minute,&sec);//파일에서 값을 읽어와서 데이터를 d_time에 저장

        sprintf(cur->where,"%s/%s",files_path,items[i]->d_name);//파일이 위치한 정보 저장

        cur->d_time=*localtime(&rawtime);
        cur->d_time.tm_year=year-1900;
        cur->d_time.tm_mon=month-1;
        cur->d_time.tm_mday=day;
        cur->d_time.tm_hour=hour;
        cur->d_time.tm_min=minute;
        cur->d_time.tm_sec=sec;

        fscanf(fp,"M : %d-%d-%d %d:%d:%d\n",&year,&month,&day,&hour,&minute,&sec);//파일에서 값을 읽어와서 데이터를 m_time에 저장

        cur->m_time=*localtime(&rawtime);
        cur->m_time.tm_year=year-1900;
        cur->m_time.tm_mon=month-1;
        cur->m_time.tm_mday=day;
        cur->m_time.tm_hour=hour;
        cur->m_time.tm_min=minute;
        cur->m_time.tm_sec=sec;

        fclose(fp);
        free(items[i]);
    }
    free(items);
    sort_info();
}

void chk_option_delete(char *input){//i r 옵션 체크
    char tmp[BUFLEN];
    strcpy(tmp,input);

    char *ptr=strtok(tmp," ");
    while(ptr!=NULL){
        if(!strcmp(ptr,"-i")){
            ioption=true;
        }
        else if(!strcmp(ptr,"-r")){
            roption=true;
        }
        ptr=strtok(NULL," ");
    }
}

void chk_option_size(char *input){//d 옵션체크
    char tmp[BUFLEN];
    strcpy(tmp,input);

    char *ptr=strtok(tmp," ");
    while(ptr!=NULL){
        if(!strcmp(ptr,"-d")){
            doption=true;
        }
        ptr=strtok(NULL," ");
    }
}

void chk_option_recover(char *input){//l 옵션체크
    char tmp[BUFLEN];
    strcpy(tmp,input);

    char *ptr=strtok(tmp," ");
    while(ptr!=NULL){
        if(!strcmp(ptr,"-l")){
            loption=true;
        }
        ptr=strtok(NULL," ");
    }
}


void recover(char *input){
    int cnt=0;
    info *cur;
    char buf[FILELEN];
    char m_str[BUFLEN];
    char d_str[BUFLEN];
    char *ptr;
    //info *datas;//info데이터들을 저장할 리스트

    if(loption){//l옵션 실행
        cur=datas;

        while(cur->next!=NULL){
            cur=cur->next;
            cnt++;

            strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->d_time);

            strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);

            printf("%d_%s D : %s M : %s\n",cnt,cur->delete_name,d_str,m_str);//파일 데이터 출력
        }
        printf("\n");
    }

    ptr=strtok(input," ");
    ptr=strtok(NULL," ");//인자의 2번째 값인 파일 이름 가져옴

    cur=datas;
    cnt=0;
    while(cur->next!=NULL){
        cur=cur->next;
        if(!strcmp(ptr,cur->delete_name)){
            cnt++;
        }
    }//파일 몇 개 있는지 탐색

    if(cnt==0){
        printf("There is no '%s' in the 'trash' directory\n",ptr);
        return;
    }//파일이 없을때의 예외처리

    cur=datas;
    int count=1;
    if(cnt!=1){//파일이 여러개가 있을 경우
        while(cur->next!=NULL){//recover과정 진행
            cur=cur->next;
            if(!strcmp(ptr,cur->delete_name)){
                    printf("%d_%s ",count++,cur->delete_name);//파일 데이터 출력

                    strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->d_time);
                    printf("D : %s ",d_str);//파일 데이터 출력

                    strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);
                    printf("M : %s\n",m_str);//파일 데이터 출력
            }
        }
        do{
            printf("Choose : ");
            scanf("%d",&cnt);//몇번인지 선택
            getchar();
        }while(cnt<=0&&cnt>count);//올바른 번호를 입력할때 까지 반복
    }

    cur=datas;
    count=0;
    int fflag=false;
    while(cur->next!=NULL){
        cur=cur->next;
        if(!strcmp(ptr,cur->delete_name)){
            count++;
            if(count==cnt){
                if(!strcmp(cur->real_path,"")){
                    printf("This file '%s' has not recovery path\n",cur->delete_name);
                    return ;
                }//복구 경로가 없을때의 예외 처리
                strcpy(buf,cur->real_path);//복구 경로를 buf에 저장
                char rec_path[FILELEN];
                ptr=strtok(buf,"/");
                strcpy(rec_path,"/");

                while(1){//하위폴더가 삭제되었을지 모르니 하위폴더를 생성
                    strcat(rec_path,ptr);
                    strcat(rec_path,"/");
                    ptr=strtok(NULL,"/");
                    if(!strcmp(cur->delete_name,ptr))
                        break;
                    mkdir(rec_path,0777);//디렉토리를 하나씩 제작해 나감
                }
                char there_path[BUFLEN];
                strcpy(there_path,cur->real_path);
                int num=1;//중복 될시 붙일 숫자
                while(access(there_path,00)!=-1){
                    sprintf(there_path,"%s%d_%s",rec_path,num++,cur->delete_name);
                }//파일이 존재하는지 확인 있을시 숫자를 하나씩 붙여가며 제작
                rename(cur->where,there_path);

                delete_info(cur);//복구한 파일이므로 가지고 있는 정보 삭제
                cur=datas;
                break;
            }
        }
    }
}

void delete(char *input){
        int year=0,month=0,day=0,hour=0,min=0;
        time_t now;
        time_t delete_time;
        struct tm when;
        char buf[FILELEN];//파일 절대 경로 저장
        char name[FILELEN];//파일명 저장
        char tmp[FILELEN];//files의 저장 위치 저장


        sscanf(input,"delete %s %d-%d-%d %d:%d",tmp,&year,&month,&day,&hour,&min);

        if(strstr(tmp,"./")==NULL||strstr(tmp,path)==NULL){
            if(strstr(tmp,"check")==NULL){
                sprintf(buf,"%s/%s",check_path,tmp);
            }
            else{
                sprintf(buf,"%s/%s",path,strstr(tmp,"check"));
            }
        }//파일의 절대경로를 buf에 저장


        if(access(buf,00)==-1){
            fprintf(stderr,"%s doesn't exit!!\n",tmp);
            return ;
        }

        if(ioption){
            remove(buf);
            return;
        }//입력값이 없거나 i옵션이 true인경우 즉시 삭제

        time(&now);
        when=*localtime(&now);
        when.tm_year=year-1900;
        when.tm_mon=month-1;
        when.tm_mday=day;
        when.tm_hour=hour;
        when.tm_min=min;//삭제시간을 struct *tm의 형태에 저장
        when.tm_sec=0;

        delete_time=mktime(&when);//삭제시간을 편하게 비교하기위해 time_t로 변경

        if(delete_time<now){
            alarm(1);
        }
        else{
            alarm(delete_time-now);
        }
        strcpy(tmp_str,buf);
}

void sort_info(){//info 데이터 정렬
    info *cur;
    cur=datas;

    while(cur->next!=NULL){
        cur=cur->next;
        info *comp=cur;
        info *old_info=cur;
        while(comp->next!=NULL){
            comp=comp->next;
            if(mktime(&old_info->d_time)>mktime(&comp->d_time)){//삭제 시간이 오래된걸 cur위치로 이동
                old_info=comp;
            }
        }
        if(old_info!=cur){//삭제 시간이 오래된게 있을경우 데이터를 변경
            info temp;
            memcpy(&temp,cur,sizeof(info));

            memcpy(cur->where,old_info->where,FILELEN);
            memcpy(cur->delete_name,old_info->delete_name,FILELEN);
            memcpy(cur->real_path,old_info->real_path,FILELEN);
            memcpy(&cur->m_time,&old_info->m_time,sizeof(struct tm));
            memcpy(&cur->d_time,&old_info->d_time,sizeof(struct tm));

            memcpy(old_info->where,&temp.where,FILELEN);
            memcpy(old_info->delete_name,&temp.delete_name,FILELEN);
            memcpy(old_info->real_path,&temp.real_path,FILELEN);
            memcpy(&old_info->m_time,&temp.m_time,sizeof(struct tm));
            memcpy(&old_info->d_time,&temp.d_time,sizeof(struct tm));//데이터값을 바꿔줌
        }
    }
}

void delete_info(info *root){//가장 오래된 데이터 삭제
    info *cur=root;
    struct stat fstat;

    char tmp[FILELEN];
    if(cur->next!=NULL){
        cur->next->prev=cur->prev;
    }
    cur->prev->next=cur->next;
    cur->next=NULL;
    cur->prev=NULL;

    strcpy(tmp,cur->where);
    remove(tmp);//주어진 파일의 files 삭제
    sprintf(tmp,"%s/%s",info_path,strrchr(tmp,'/')+1);
    lstat(tmp,&fstat);
    info_byte-=fstat.st_size;//info파일의 데이터만큼 줄임
    remove(tmp);//info 파일 데이터 삭제
}

void size(char *input){//파일들의 size와 경로를 출력해주는 함수
    int depth=1;
    char *ptr;
    char buf[BUFLEN];
    char tmp[BUFLEN];
    node *root;
    struct stat fstat;

    strcpy(tmp,input);
    ptr=strtok(tmp," ");
    ptr=strtok(NULL," ");
    strcpy(tmp,ptr);//size뒤의 경로를 복사하기 위함

    if(strstr(tmp,"./")==NULL||strstr(tmp,path)==NULL){
        if(strstr(tmp,"check")==NULL){
            sprintf(buf,"./%s/%s",check_path,tmp);
        }
        else{
            sprintf(buf,"./%s",strstr(tmp,"check"));
        }
    }//경로를 상대경로화 시킴

    lstat(buf,&fstat);//초기 check값을 넣기 위함
    root=create_node(fstat,buf,buf);//root의 맨앞 노드 생성
    root->depth=1;
    make_list(buf,root,root->depth);

    if(doption){//number를 depth로 설정
        ptr=strrchr(input,' ');
        depth=atoi(ptr+1);
    }
    size_list(root,depth);//size 출력
    free_tree(root);
}

void tree(){//디렉토리를 트리화 시켜 보여주는 함수
    node *root;
    struct stat fstat;

    root=(node*)malloc(sizeof(node));
    root->next=NULL;
    root->prev=NULL;
    root->child=NULL;
    root->parent=NULL;

    lstat(check_path,&fstat);
    root->next=create_node(fstat,"check",check_path);//root의 맨앞 노드 생성

    root->next->child=make_tree(check_path,1);//check 경로로 tree제작
    root->next->child->parent=root->next;
    node *tmp;//자식 전부를 부모와 연결 하기 위해 사용
    tmp=root->next->child;
    while(tmp->next!=NULL){
        tmp=tmp->next;
        tmp->parent=root->next;
    }

    print_tree(root);//tree 출력
    free_tree(root);//만든 tree해제
}

void Exit(){//종료 함수
    delete_infos();
}

void delete_infos(){//info데이터를 삭제
    info *cur=datas;
    while(cur->next!=NULL){
        cur=cur->next;
        memset(cur->prev,0,sizeof(info));//cur->prev의 데이터를 비움
        free(cur->prev);
    }
    memset(cur,0,sizeof(info));//마지막 위치의 노드 데이터를 삭제
    free(cur);
}

void help(){//도움말 출력
    printf("DELETE [FILENAME] [END_TIME] [OPTION] when time is END_TIME FILENAME will delete\n");
    printf("                               -i :   delete Immediately\n");
    printf("                               -r :   ask before delete\n");
    printf("SIZE   [FILENAME] [OPTION]            print File path, File size\n");
    printf("                      -d number   :   print small than number of depth\n");
    printf("RECOVER [FILENAME] [OPTION]           recover in trash folder\n");
    printf("                      -l          :   print trash files order by delete date\n");
    printf("TREE                                  print check directory like tree\n");
    printf("EXIT                                  program end\n");
    printf("HELP                                  print program usage\n");
}
