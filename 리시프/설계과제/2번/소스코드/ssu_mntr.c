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

char path[BUFLEN];//�⺻ �۾���� ����
char trash_path[BUFLEN];//trash ��� ����
char check_path[BUFLEN];//check ��� ����
char info_path[BUFLEN];//trash/info ��� ����
char files_path[BUFLEN];//trash/files ��� ����

char tmp_str[FILELEN];//delete�� ��θ� ������ �ӽ� �迭

long info_byte=0;//info�� ������ �˱����� ����

int ioption=false;
int roption=false;
int doption=false;
int loption=false;

char startmsg[BUFLEN]="20182602>";//���� �޽����� ���

info *datas;

void ssu_mntr(){
    getcwd(path,BUFLEN);

    sprintf(check_path,"%s/%s",path,"check");
    mkdir(check_path,0777);//check���丮 ����


    if(fork()==0){//���� ���μ������� ssu_check����
        if(fork()==0){
            ssu_check(path);//���� ���α׷� ����
            exit(0);
        }

        else{
            exit(0);
        }

    }
    else{//�θ� �����Ű�� ���� �ٽ��ѹ� fork���� ssu_mntr ����
        signal(SIGALRM, delete_files);//delete������ �ð� Ȯ���� ���� ���
        sprintf(trash_path,"%s/%s",path,"trash");//trash ���丮 ����
        mkdir(trash_path,0777);
        sprintf(info_path,"%s/%s",trash_path,"info");//trash/info ���丮 ����
        mkdir(info_path,0777);
        sprintf(files_path,"%s/%s",trash_path,"files");//trash/filse ���丮 ����
        mkdir(files_path,0777);

        init_info(info_path);//���� �������ִ� info�����͸� ������

        while(1){
            char input[BUFLEN];
            char c;
            memset(input,'\0',BUFLEN);//�������̳� �����Ⱚ�� ���� �� �����Ƿ� �ʱ�ȭ��Ŵ
            write(STDOUT_FILENO,startmsg,BUFLEN);//20182602>�� ���
            int i=0;
            while(read(STDIN_FILENO,&c,1)>0&&c!='\n'){
                input[i++]=c;
            }//\n�� ������ ���� �޾ƿ�
            __fpurge(stdin);
            input[i]='\0';//���ڿ��� �������� ����

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

void delete_files(){//�ð��� ������츦 ����
    int fflag=true;
    if(roption){//r�ɼ��� �������� ���
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
        write(STDOUT_FILENO,startmsg,BUFLEN);//�۾� ���߿� ����ǹǷ� �ٽ��ѹ� ���۸޽��� ���
    }
    if(fflag)
        delete_files_info();//���� ���� �Լ� ����
}

void delete_files_info(){//���� ���� �Լ� ����
    char d_str[FILELEN];//���� �ð� ����
    char m_str[FILELEN];//���� �ð� ����
    struct stat fstat;
    time_t now;
    struct tm when;
    char buf[FILELEN];//���� ���� ��� ����
    char name[FILELEN];//���ϸ� ����
    char tmp[FILELEN];//files�� ���� ��ġ ����
    int status;

    strcpy(buf,tmp_str);//������ �����صξ��� ������ buf�� �Űܿ�
    time(&now);
    when=*localtime(&now);
    info *cur=datas;//�̸��� ��ġ�°� ��� ã������
    int cnt=0;
    strcpy(name,strrchr(buf,'/')+1);//name�� ������ �̸��� ����

    int fflag=false;//���� �� �ִ� �̸��� ã�Ҵ��� Ȯ��
    int jflag=false;//�ߺ� �Ȱ� �ִ����� ���� �÷���

    strcpy(tmp,name);
    while(!fflag){//���� �� �ִ� �̸��� ã���� ���� �ݺ�
        fflag=true;
        cur=datas;
        while(cur->next!=NULL){
            cur=cur->next;
            if(!strcmp(strrchr(cur->where,'/')+1,tmp)){
                fflag=false;//�̸��� �ߺ��Ǵ°��� �����ϹǷ� �ٽ� ����
                sprintf(tmp,"%d_%s",cnt,name);//�ߺ����� �ʵ��� ���ڸ� �ٿ� ����
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
    strcpy(cur->delete_name,name);//������ �̸��� ����
    cur->d_time=when;//���� �ð��� ����

    strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&when);

    lstat(buf,&fstat);//������ ���� �ð��� �����ϱ� ����
    cur->m_time=*localtime(&fstat.st_mtime);//���� �ð��� ����


    if(jflag){
        sprintf(name,"%s",tmp);
    }//�ߺ��� ������ �����Ƿ� ������ �̸� �տ� ����_�� ����

    sprintf(tmp,"%s/%s",files_path,name);//���������� �̵��� ���� ��θ� ����
    strcpy(cur->where,tmp);//���� ������ �̵��� ��θ� ����
    strcpy(cur->real_path,buf);//���� ��� ����

    rename(cur->real_path,cur->where);//������ �̵���Ŵ

    sprintf(tmp,"%s/%s",info_path,strrchr(cur->where,'/')+1);


    FILE *tmpfp;
    tmpfp=fopen(tmp,"w+");//���� ������ ������ ���� ����
    fprintf(tmpfp,"[Trash info]\n%s\n%s\n",cur->delete_name,cur->real_path);//���������� info�� ����

    fprintf(tmpfp,"D : %s\n",d_str);//���� �ð� �ۼ�

    strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);
    fprintf(tmpfp,"M : %s\n",m_str);//���� �ð� �ۼ�

    fclose(tmpfp);
    lstat(tmp,&fstat);

    info_byte+=fstat.st_size;//2kb�� ������� �����ϱ� ���� size�� ������Ŵ
    sort_info();//������ ������ ����


    if(info_byte>MAX_BYTE){//2kb�� �ʰ��� ���
        delete_info(datas->next);
    }
}

void init_info(char *info_path){
    int files_num;
    struct dirent **items;//�����͸� ������ ������
    FILE *fp;
    char buf[BUFLEN];
    time_t rawtime;
    int i;
    int year,month,day,hour,minute,sec;
    info *cur;

    time(&rawtime);

    datas=(info*)malloc(sizeof(info));//front ��� ����
    datas->next=NULL;
    datas->prev=NULL;

    files_num=scandir(info_path,&items,NULL,alphasort);//info�� ������ ���ϵ��� ������
    cur=datas;

    for(i=0;i<files_num;i++){
        struct stat input;//���� ���¸� �����ϱ� ���� struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.�� ..�� �н�
        }

        sprintf(ppath,"%s/%s",info_path,items[i]->d_name);
        lstat(ppath,&input);//�۾� ������ ������ �ҷ���
        cur->next=(info*)malloc(sizeof(info));
        cur->next->prev=cur;
        cur=cur->next;//��ĭ�� �Űܰ��� ����
        cur->next=NULL;


        info_byte+=input.st_size;//info�� ������ ���������� ���� ����

        fp=fopen(ppath,"r+");//info������ ������ datas�� �����ϱ� ���� ������ ��
        fscanf(fp,"%[^\n]\n",buf);
        fscanf(fp,"%[^\n]\n",cur->delete_name);
        fscanf(fp,"%[^\n]\n",cur->real_path);
        fscanf(fp,"D : %d-%d-%d %d:%d:%d\n",&year,&month,&day,&hour,&minute,&sec);//���Ͽ��� ���� �о�ͼ� �����͸� d_time�� ����

        sprintf(cur->where,"%s/%s",files_path,items[i]->d_name);//������ ��ġ�� ���� ����

        cur->d_time=*localtime(&rawtime);
        cur->d_time.tm_year=year-1900;
        cur->d_time.tm_mon=month-1;
        cur->d_time.tm_mday=day;
        cur->d_time.tm_hour=hour;
        cur->d_time.tm_min=minute;
        cur->d_time.tm_sec=sec;

        fscanf(fp,"M : %d-%d-%d %d:%d:%d\n",&year,&month,&day,&hour,&minute,&sec);//���Ͽ��� ���� �о�ͼ� �����͸� m_time�� ����

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

void chk_option_delete(char *input){//i r �ɼ� üũ
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

void chk_option_size(char *input){//d �ɼ�üũ
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

void chk_option_recover(char *input){//l �ɼ�üũ
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
    //info *datas;//info�����͵��� ������ ����Ʈ

    if(loption){//l�ɼ� ����
        cur=datas;

        while(cur->next!=NULL){
            cur=cur->next;
            cnt++;

            strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->d_time);

            strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);

            printf("%d_%s D : %s M : %s\n",cnt,cur->delete_name,d_str,m_str);//���� ������ ���
        }
        printf("\n");
    }

    ptr=strtok(input," ");
    ptr=strtok(NULL," ");//������ 2��° ���� ���� �̸� ������

    cur=datas;
    cnt=0;
    while(cur->next!=NULL){
        cur=cur->next;
        if(!strcmp(ptr,cur->delete_name)){
            cnt++;
        }
    }//���� �� �� �ִ��� Ž��

    if(cnt==0){
        printf("There is no '%s' in the 'trash' directory\n",ptr);
        return;
    }//������ �������� ����ó��

    cur=datas;
    int count=1;
    if(cnt!=1){//������ �������� ���� ���
        while(cur->next!=NULL){//recover���� ����
            cur=cur->next;
            if(!strcmp(ptr,cur->delete_name)){
                    printf("%d_%s ",count++,cur->delete_name);//���� ������ ���

                    strftime(d_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->d_time);
                    printf("D : %s ",d_str);//���� ������ ���

                    strftime(m_str,BUFLEN,"%Y-%m-%d %H:%M:%S",&cur->m_time);
                    printf("M : %s\n",m_str);//���� ������ ���
            }
        }
        do{
            printf("Choose : ");
            scanf("%d",&cnt);//������� ����
            getchar();
        }while(cnt<=0&&cnt>count);//�ùٸ� ��ȣ�� �Է��Ҷ� ���� �ݺ�
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
                }//���� ��ΰ� �������� ���� ó��
                strcpy(buf,cur->real_path);//���� ��θ� buf�� ����
                char rec_path[FILELEN];
                ptr=strtok(buf,"/");
                strcpy(rec_path,"/");

                while(1){//���������� �����Ǿ����� �𸣴� ���������� ����
                    strcat(rec_path,ptr);
                    strcat(rec_path,"/");
                    ptr=strtok(NULL,"/");
                    if(!strcmp(cur->delete_name,ptr))
                        break;
                    mkdir(rec_path,0777);//���丮�� �ϳ��� ������ ����
                }
                char there_path[BUFLEN];
                strcpy(there_path,cur->real_path);
                int num=1;//�ߺ� �ɽ� ���� ����
                while(access(there_path,00)!=-1){
                    sprintf(there_path,"%s%d_%s",rec_path,num++,cur->delete_name);
                }//������ �����ϴ��� Ȯ�� ������ ���ڸ� �ϳ��� �ٿ����� ����
                rename(cur->where,there_path);

                delete_info(cur);//������ �����̹Ƿ� ������ �ִ� ���� ����
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
        char buf[FILELEN];//���� ���� ��� ����
        char name[FILELEN];//���ϸ� ����
        char tmp[FILELEN];//files�� ���� ��ġ ����


        sscanf(input,"delete %s %d-%d-%d %d:%d",tmp,&year,&month,&day,&hour,&min);

        if(strstr(tmp,"./")==NULL||strstr(tmp,path)==NULL){
            if(strstr(tmp,"check")==NULL){
                sprintf(buf,"%s/%s",check_path,tmp);
            }
            else{
                sprintf(buf,"%s/%s",path,strstr(tmp,"check"));
            }
        }//������ �����θ� buf�� ����


        if(access(buf,00)==-1){
            fprintf(stderr,"%s doesn't exit!!\n",tmp);
            return ;
        }

        if(ioption){
            remove(buf);
            return;
        }//�Է°��� ���ų� i�ɼ��� true�ΰ�� ��� ����

        time(&now);
        when=*localtime(&now);
        when.tm_year=year-1900;
        when.tm_mon=month-1;
        when.tm_mday=day;
        when.tm_hour=hour;
        when.tm_min=min;//�����ð��� struct *tm�� ���¿� ����
        when.tm_sec=0;

        delete_time=mktime(&when);//�����ð��� ���ϰ� ���ϱ����� time_t�� ����

        if(delete_time<now){
            alarm(1);
        }
        else{
            alarm(delete_time-now);
        }
        strcpy(tmp_str,buf);
}

void sort_info(){//info ������ ����
    info *cur;
    cur=datas;

    while(cur->next!=NULL){
        cur=cur->next;
        info *comp=cur;
        info *old_info=cur;
        while(comp->next!=NULL){
            comp=comp->next;
            if(mktime(&old_info->d_time)>mktime(&comp->d_time)){//���� �ð��� �����Ȱ� cur��ġ�� �̵�
                old_info=comp;
            }
        }
        if(old_info!=cur){//���� �ð��� �����Ȱ� ������� �����͸� ����
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
            memcpy(&old_info->d_time,&temp.d_time,sizeof(struct tm));//�����Ͱ��� �ٲ���
        }
    }
}

void delete_info(info *root){//���� ������ ������ ����
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
    remove(tmp);//�־��� ������ files ����
    sprintf(tmp,"%s/%s",info_path,strrchr(tmp,'/')+1);
    lstat(tmp,&fstat);
    info_byte-=fstat.st_size;//info������ �����͸�ŭ ����
    remove(tmp);//info ���� ������ ����
}

void size(char *input){//���ϵ��� size�� ��θ� ������ִ� �Լ�
    int depth=1;
    char *ptr;
    char buf[BUFLEN];
    char tmp[BUFLEN];
    node *root;
    struct stat fstat;

    strcpy(tmp,input);
    ptr=strtok(tmp," ");
    ptr=strtok(NULL," ");
    strcpy(tmp,ptr);//size���� ��θ� �����ϱ� ����

    if(strstr(tmp,"./")==NULL||strstr(tmp,path)==NULL){
        if(strstr(tmp,"check")==NULL){
            sprintf(buf,"./%s/%s",check_path,tmp);
        }
        else{
            sprintf(buf,"./%s",strstr(tmp,"check"));
        }
    }//��θ� �����ȭ ��Ŵ

    lstat(buf,&fstat);//�ʱ� check���� �ֱ� ����
    root=create_node(fstat,buf,buf);//root�� �Ǿ� ��� ����
    root->depth=1;
    make_list(buf,root,root->depth);

    if(doption){//number�� depth�� ����
        ptr=strrchr(input,' ');
        depth=atoi(ptr+1);
    }
    size_list(root,depth);//size ���
    free_tree(root);
}

void tree(){//���丮�� Ʈ��ȭ ���� �����ִ� �Լ�
    node *root;
    struct stat fstat;

    root=(node*)malloc(sizeof(node));
    root->next=NULL;
    root->prev=NULL;
    root->child=NULL;
    root->parent=NULL;

    lstat(check_path,&fstat);
    root->next=create_node(fstat,"check",check_path);//root�� �Ǿ� ��� ����

    root->next->child=make_tree(check_path,1);//check ��η� tree����
    root->next->child->parent=root->next;
    node *tmp;//�ڽ� ���θ� �θ�� ���� �ϱ� ���� ���
    tmp=root->next->child;
    while(tmp->next!=NULL){
        tmp=tmp->next;
        tmp->parent=root->next;
    }

    print_tree(root);//tree ���
    free_tree(root);//���� tree����
}

void Exit(){//���� �Լ�
    delete_infos();
}

void delete_infos(){//info�����͸� ����
    info *cur=datas;
    while(cur->next!=NULL){
        cur=cur->next;
        memset(cur->prev,0,sizeof(info));//cur->prev�� �����͸� ���
        free(cur->prev);
    }
    memset(cur,0,sizeof(info));//������ ��ġ�� ��� �����͸� ����
    free(cur);
}

void help(){//���� ���
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
