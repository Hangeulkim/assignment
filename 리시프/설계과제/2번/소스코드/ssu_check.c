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

FILE *fp;//log.txt�� ����ų ������
char chkpath[BUFLEN];//check���丮�� ��θ� ������ �迭
char fpath[BUFLEN];//log.txt�� ��θ� ������ �迭

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

    sprintf(fpath,"%s/%s",dir_name,"log.txt");//log.txt ��� ����
    sprintf(chkpath,"%s/%s",dir_name,"check");//check���丮 ��� ����

    setsid();//���ο� ������ ����
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);//�ñ׳� ������ ����
    maxfd=getdtablesize();//���̺� ����� ������

    for(fd=0;fd<maxfd;fd++)
        close(fd);//�����ִ� fd�� ���� ����

    umask(0);//umask�� 0���� ����
    chdir("/");//�ֻ��� ���丮�� �۾���� �̵�


    if(chdir("/")<0){//���μ����� �۾� ���丮�� �̵�
        fprintf(stderr,"can't open dir %s",dir_name);
        exit(1);
    }
    fd = open("/dev/null",O_RDWR);//ǥ�� ����°� ������ �ٽü���
    dup(0);
    dup(0);


    fp=fopen(fpath,"w+");//����� log.txt���� ����

    lstat(chkpath,&fstat);
    before_root=create_node(fstat,"check",chkpath);//before_root�� �Ǿ� ��� ����
    make_list(chkpath,before_root,2);//check���丮�� 1���� listȭ�Ͽ� before_root�� ����
    before_root->depth=1;


    node *cur=before_root;

    while(1){
        fclose(fp);
        //����� ������ �����ϱ� ���� �ݰ� �ٽÿ���
        fp=fopen(fpath,"a+");
        after_root=create_node(fstat,"check",chkpath);//before_root�� �Ǿ� ��� ����
        make_list(chkpath,after_root,2);//check���丮�� 1���� listȭ�Ͽ� ���ϱ����� after_root�� ����
        after_root->depth=1;//�� ó�� check ���丮�� depth�� 1�� ����

        write_data(before_root,after_root);//���� �����Ϳ� ���� �����͸� ���Ͽ� ������ ���
        free_tree(before_root);//before_root�� �����͸� �� ����Ͽ����Ƿ� ����
        before_root=after_root;//�ٽ� ���ϱ� ���� before_root�� after_root�� �Ҵ�

        sleep(1);
    }

    fprintf(stderr, "ssu_check end\n");
    exit(0);
}

long dir_size(node *root){//���丮 �Ϻ��� ���ϵ��� ũ�⸦ ���ϱ� ���� �Լ�
    node *cur=root;
    char buf[FILELEN];
    strcpy(buf,cur->real_path);
    strcat(buf,"/");//������ ��θ� Ȯ���ϱ� ���� /�� �ٿ���

    long size=0;
    while(cur->next!=NULL){
        cur=cur->next;
        if(strstr(cur->real_path,buf)!=NULL&&!S_ISDIR(cur->data.st_mode)){//���� ������ ��� size�� ������
            size+=cur->data.st_size;
        }
    }
    return size;
}

void size_list(node *root, int depth){//depth�� ���� list�� �����ִ� �Լ�
    node *cur=root;
    long size=0;
    printf("\n");
    while(1){
        if(cur->depth<=depth){
            if(S_ISDIR(cur->data.st_mode)){//���丮 �ϰ�� �Ϻ� ���ϵ��� ����� ���ؾ��ϹǷ� ���� ����
                size=dir_size(cur);
            }
            else{
                size=cur->data.st_size;
            }
            printf("%ld\t\t%s%s\n",size,"./",strstr(cur->real_path,"check"));//���� ������ ���
        }

        if(cur->next!=NULL)
            cur=cur->next;//����ĭ���� �̵�
        else
            break;//�����Ƿ� ����
    }
}

void write_data(node *root1, node *root2){//��������� ���Ͽ� ���� �Լ�
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
        int fflag=false;//ã�Ҵ����� Ȯ���ϴ� �÷���

        while(cur2->next!=NULL){
            cur2=cur2->next;

            if(cur1->data.st_ino==cur2->data.st_ino||!strcmp(cur1->realative_path,cur2->realative_path)){//������ �ִ� �������� Ȯ��
                fflag=true;//ã�����Ƿ� �÷��� true ����
                if(cur1->data.st_mtime!=cur2->data.st_mtime){//������ �ð��� �ٸ��Ƿ� modify �۵�
                    when=localtime(&cur2->data.st_mtime);
                    strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
                    fprintf(fp,"%s [modify_%s]\n",date_str,cur1->realative_path);
                }
                else if(strcmp(cur1->name,cur2->name)){//vi�� ������ inode�� �ٲ�Ƿ� �̸����� üũ
                    temp=time(NULL);
                    when=localtime(&temp);
                    strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
                    fprintf(fp,"%s [modify_%s]\n",date_str,cur1->realative_path);
                }
                break;
            }
        }
        if(!fflag){//���� ���ϸ�Ͽ� �����Ƿ� ������� �ǹ� delete �۵�
            temp=time(NULL);
            when=localtime(&temp);
            strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
            fprintf(fp,"%s [delete_%s]\n",date_str,cur1->realative_path);
        }
    }
    cur2=root2;

    while(cur2->next!=NULL){
        cur2=cur2->next;
        int fflag=false;//ã�Ҵ����� Ȯ���ϴ� �÷���

        cur1=root1;
        while(cur1->next!=NULL){
            cur1=cur1->next;

            if(!strcmp(cur1->realative_path,cur2->realative_path)||cur1->data.st_ino==cur2->data.st_ino){
                fflag=true;//vi�� ������ inode�� �ٲ�Ƿ� ����η� üũ
                break;
            }
        }
        if(!fflag){//���� �������� ��ã�� ���̹Ƿ� ���� ���� ������ �ǹ� create �۵�
            when=localtime(&cur2->data.st_ctime);
            strftime(date_str,BUFLEN,"[%Y:%m:%d %H:%M:%S]",when);
            fprintf(fp,"%s [create_%s]\n",date_str,cur2->realative_path);
        }
    }
}


node *create_node(struct stat input, char *path,char *r_path){
    //��� �����Լ�
    node *new;
    char *ptr;
    new=(node*)malloc(sizeof(node));//���ο� ��带 ����

    new->next=NULL;
    new->prev=NULL;
    new->child=NULL;
    new->parent=NULL;

    new->data=input;

    ptr=strstr(r_path,"/check");
    strcat(new->realative_path,ptr+strlen("/check"));//check�� ��� ��� ����
    strcpy(new->real_path,r_path);//���� ��� ����
    strcpy(new->name,path);//���� �̸��� ����

    return new;
}

void print_tree(node *root){//tree�� �����ִ� �Լ�
    node *cur;
    cur = root;

    printf("\n");
    while(cur->parent!=NULL||cur->next!=NULL){//�θ� ���� ���� ��嵵 ���°�� ����
        while(cur->parent!=NULL&&cur->next==NULL){//child�� ��� ��带 �˻����� ��� parent�� �̵�
            cur=cur->parent;
        }
        if(cur->next==NULL) break;
        cur=cur->next;

        if(cur->parent!=NULL&&cur!=cur->parent->child->next){
            for(int i=1;i<cur->depth;i++){
                printf("\t");
            }
        }
        printf("->%s\t",cur->name);//����� �̸� ���
        if(cur->child!=NULL&&cur->child->next!=NULL){
            cur=cur->child;//�ڽ��� ������� �ڽĳ��� �̵�
        }
        else{
            printf("\n\n");
        }
    }
}

node *make_list(char *path, node *root, int depth){//1���� ����Ʈ�� ����� �Լ�
    node *cur;
    int files_num;
    int i;
    struct dirent **items;//�����͸� ������ ������
    char tmp[BUFLEN];//�θ��� �̸��� �����ϱ� ���� �ӽ� �迭

    files_num=scandir(path,&items,NULL,alphasort);//���� ������ ������ ������

    cur=root;//cur�� ��Ʈ�� �̵�
    for(i=0;i<files_num;i++){
        struct stat input;//���� ���¸� �����ϱ� ���� struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.�� ..�� �н�
        }


        sprintf(ppath,"%s/%s",path,items[i]->d_name);
        lstat(ppath,&input);//�۾� ������ ������ �ҷ���
        cur->next=create_node(input,items[i]->d_name,ppath);
        cur->next->prev=cur;
        cur=cur->next;//��ĭ�� �Űܰ��� üũ
        cur->depth=depth+1;

        //���丮 �ϰ�� ��� ȣ��
        if(S_ISDIR(input.st_mode)){
            cur=make_list(ppath,cur,depth+1);//�ڽİ� �θ� ����
        }
        free(items[i]);
    }
    free(items);
    return cur;
}

node *make_tree(char *path,int depth)
{//���丮�� Ʈ���� ����� �Լ�
    node *root;
    node *cur;
    int files_num;
    int i;
    struct dirent **items;//�����͸� ������ ������
    char tmp[BUFLEN];//�θ��� �̸��� �����ϱ� ���� �ӽ� �迭

    root=(node*)malloc(sizeof(node));

    root->parent=NULL;
    root->next=NULL;
    root->prev=NULL;
    root->child=NULL;
    root->depth=depth;

    files_num=scandir(path,&items,NULL,alphasort);//���� ������ ������ ������

    cur=root;//cur�� ��Ʈ�� �̵�
    for(i=0;i<files_num;i++){
        struct stat input;//���� ���¸� �����ϱ� ���� struct
        char ppath[BUFLEN];

        if(!strcmp(items[i]->d_name,".")||!strcmp(items[i]->d_name,
        "..")){
            free(items[i]);
            continue;//.�� ..�� �н�
        }


        sprintf(ppath,"%s/%s",path,items[i]->d_name);
        lstat(ppath,&input);//�۾� ������ ������ �ҷ���
        cur->next=create_node(input,items[i]->d_name,ppath);
        cur->next->prev=cur;
        cur=cur->next;//��ĭ�� �Űܰ��� üũ
        cur->depth=depth+1;

        //���丮 �ϰ�� ��� ȣ��
        if(S_ISDIR(input.st_mode)){
            cur->child=make_tree(ppath,depth+1);//�ڽİ� �θ� ����
            cur->child->parent=cur;
            node *tmp;//�ڽ� ���θ� �θ�� ���� �ϱ� ���� ���
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

void free_tree(node *root)//���� ������ �� �Ʒ��� �ڽĺ��� �Ҵ� ����
{//Ʈ�� ���� �Լ�
    node* cur=root;
    node *tmp;
    while(cur->next!=NULL)//cur�� ���� �������� �̵���Ŵ
        cur=cur->next;

    while(cur->prev!=NULL){
        if(cur->child!=NULL){
            free_tree(cur->child);
            cur->child=NULL;
        }
        tmp=cur;
        cur=cur->prev;

        tmp->parent=NULL;//��� ���� ����
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
