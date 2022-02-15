#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>


typedef struct Client{
    char id[9];
    char *pwd;
    char *name;
    char *address;
    char *phonenum;
    struct Client *next;
}client;

typedef struct Book{
    int booknum;
    char *bookname;
    char *publicier;
    char *writer;
    char isbn[15];
    char *have;
    char canbor;
    struct Book *next;
}book;

typedef struct Borrow{
    char id[9];
    int booknum;
    time_t borday;
    time_t retday;
    struct Borrow *next;
}borrow;

typedef struct _linkedList
{
    client *admin;
    client *chead;
    book *bhead;
    borrow *borhead;
} list;

void reset(list *plist,FILE *idfp,FILE *bfp,FILE *borfp);//�ʱ�ȭ
void end(list *plist);//���α׷� ����

void addclient(list*);//ȸ�� �߰�
client *searchClient(list*, char*);//�й��� �´� ȸ������ ã��
void login(list*);//�α���ȭ��
void clientMenu(list*, char*);//ȸ��ȭ��
void searchBookMenu(list*);//�����˻�ȭ��
void bookPrint(book*);//�����������
void bookNameSearch(list*);//�����̸��˻�
void bookPubSearch(list*);//�������ǰ˻�
void bookIsbnSearch(list*);//���� isbn�˻�
void bookWritSearch(list*);//���� ���ڰ˻�
void bookTotSearch(list*);//���� ��ü �˻�
char *borBookName(list*, int);//����å ������ȣ�� ������ ����
void borBookList(list*, char*);//����å ����
void fixClient(list*, char*);//ȸ������ ����
void delClient(list*, char*);//ȸ��Ż��

void saveinfo(list *plist);//���� ����


void addbook(list *plist);//1. ���� ���
void delbook(list *plist);//2. ���� ����
void borbook(list *plist);//3. ���� �뿩
void retbook(list *plist);//4. ���� �ݳ�
void memberlistmenu(list *plist);//6. ȸ�� ��� �޴� ȣ��
void srchbymemname(list *plist);//1. �̸� �˻�
void srchbyid(list *plist);//2. �й� �˻�
void srchbyall(list *plist);//3. ��ü �˻�
bool check(void *check);//�־��� �����Ͱ� NULL���� Ȯ�� NULL�� �ƴϸ� true
void menuadmin(list *plist);//admin menuȣ��
void menubooksearch(list *plist);//book search menuȣ��
void sortidcmp(list *plist);//sort id
void sortisbncmp(list *plist);//sort ISBN
void delisbn(list *plist);//isbn���� �˻� ����
void delbookname(list *plist);//���������� �˻� ����

bool search_by_book_name(list *plist);								//����� å �̸����� �˻��ϴ� �Լ�
bool search_by_ISBN(list *plist);									//����� ISBN ���� �˻��ϴ� �Լ�
void insert_borrow_file(list *plist, char *cli_id, int book_num);	//å ������ borrow ���Ͽ� �����ϴ� �Լ�
void want_ret(list *plist,book *bfp, char *cli_id);					//å �ݳ� ���θ� ����� �Լ�
void ruduce_borrow_file(list *plist, int tmp_book_no, char *cli_id);//�ݳ��� å�� borrow ���Ͽ��� ���� �ϴ� �Լ�
bool can_bor(book *pre_ad, int i);									//ã���� �ϴ� ������ ������ �������� �˷��ִ� �Լ�
bool can_bor_2(book *pre_ad, int size, int booknum);				//�뿩 �ϰ��� �ϴ� ������ȣ�� ������ ���� ���� ���θ� �˷��ִ� �Լ�

char tmp[1000];

int main(){
    FILE *idfp;
    idfp=fopen("client.txt","r");//client ������ �о��
    if(idfp==NULL)//client������ �ִ��� Ȯ��
      idfp=fopen("client.txt","w+");//client ������ �����Ƿ� ���θ���
    FILE *bfp;
     bfp=fopen("book.txt","r");//book ������ �о��
     if(bfp==NULL)//book ������ �ִ��� Ȯ��
        bfp=fopen("book.txt","w+");//book ������ �����Ƿ� ���θ���
    FILE *borfp;
    borfp=fopen("borrow.txt","r");//borrow ������ �о��
    if(borfp==NULL)//borrow ������ �ִ��� Ȯ��
        borfp=fopen("borrow.txt","w+");//borrow ������ �����Ƿ� ���θ���

    list project;//�� ������ ������ ������ header����Ʈ ����
    reset(&project,idfp,bfp,borfp); //�޸𸮿� client, book, borrow ���� ����
    fclose(idfp);
    fclose(bfp);
    fclose(borfp);//���Ͽ� ������ �� �ҷ������Ƿ� ����

    while(1)
    {
        printf(">> ������ ���� <<\n1. ȸ�� ����\t2. �α���\t3. ���α׷� ����\n==============================\n");
      int menu=0;
      printf("������ ����� �Է��ϼ��� : ");
      scanf("%d",&menu);
      getchar();
      printf("==============================\n");
      switch(menu)
      {
        case 1:
          addclient(&project);//ȸ������ �Լ� ȣ��
          break;
        case 2:
          login(&project);//�α��� �Լ� ȣ��
          break;
        case 3:
          end(&project);//���� �Լ� ȣ��
          return 0;
      }
    }
}

bool check(void *check){ //���� �����Ͱ� null���� Ȯ���ϰ� null�� �ƴϸ� true��ȯ�Ͽ� �������� �����Ŵ
    if(check==NULL) return false;
    return true;
}

void reset(list *plist,FILE *idfp,FILE *bfp,FILE *borfp){//���Ͽ� �������� �޸𸮿� �����ϱ����� ������ �о��
    plist->admin=(client*)malloc(sizeof(client));
    plist->admin->next=NULL;
    plist->chead=(client*)malloc(sizeof(client));
    plist->chead->next=NULL;
    plist->bhead=(book*)malloc(sizeof(book));
    plist->bhead->next=NULL;
    plist->borhead=(borrow*)malloc(sizeof(borrow));
    plist->borhead->next=NULL;//list�� ������� ����
    char tmpcur;

    while(!feof(idfp))//������ �������� Ȯ��
    {
      if(fread(&tmpcur,1,1,idfp)==0) break;//������ ���鹮���� ��� ����� ������� �����������Ƿ� �ٽð˻�
      else fseek(idfp,-1,SEEK_CUR);//�˻��ϸ鼭 ������ Ȯ�������Ƿ� �����͸� ������ġ�� �ű�
      fscanf(idfp,"%[^|]|",tmp);
      if(strcmp(tmp,"00000000")==0){//admin�������� Ȯ��
          strcpy(plist->admin->id,tmp);

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->pwd=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->pwd,tmp);//admin�� ��й�ȣ ���� �Ҵ�

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->name=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->name,tmp);//admin�� �̸� ���� �Ҵ�

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->address=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->address,tmp);//admin�� �ּ� ���� �Ҵ�

          fscanf(idfp,"%[^\n]\n",tmp);
          plist->admin->phonenum=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->phonenum,tmp);//admin�� ��ȭ��ȣ ���� �Ҵ�
      }
      else{
          client *newcli=(client*)malloc(sizeof(client));//���ο� ��� ����
          newcli->next=plist->chead->next;//newcli�� chead->next�� ����Ű����
          plist->chead->next=newcli;//chead->next�� ���ο� ��带 ����Ű����

          strcpy(newcli->id,tmp);

          fscanf(idfp,"%[^|]|",tmp);
          newcli->pwd=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->pwd,tmp);//newcli�� ��й�ȣ ���� �Ҵ�

          fscanf(idfp,"%[^|]|",tmp);
          newcli->name=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->name,tmp);//newcli�� �̸� ���� �Ҵ�

          fscanf(idfp,"%[^|]|",tmp);
          newcli->address=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->address,tmp);//newcli�� �ּ� ���� �Ҵ�

          fscanf(idfp,"%[^\n]\n",tmp);
          newcli->phonenum=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->phonenum,tmp);//newcli�� ��ȭ��ȣ ���� �Ҵ�
      }
  }

      while(!feof(bfp))//������ �������� Ȯ��
      {
        if(fread(&tmpcur,1,1,bfp)==0) break;//������ ���鹮���� ��� ����� ������� �����������Ƿ� �ٽð˻�
        else fseek(bfp,-1,SEEK_CUR);//�˻��ϸ鼭 ������ Ȯ�������Ƿ� �����͸� ������ġ�� �ű�

        fscanf(bfp,"%[^|]|",tmp);

        book *newbook=(book*)malloc(sizeof(book));//���ο� ��� ����
        newbook->next=plist->bhead->next;//newbook�� bhead->next�� ����Ű����
        plist->bhead->next=newbook;//bhead->next�� ���ο� ��带 ����Ű����

        newbook->booknum=atoi(tmp);//tmp�� �о�� ������ atoi�� ������ �ٲ� ������ �Ҵ�

        fscanf(bfp,"%[^|]|",tmp);
        newbook->bookname=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->bookname,tmp);//newbook�� å���� ���� �Ҵ�

        fscanf(bfp,"%[^|]|",tmp);
        newbook->publicier=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->publicier,tmp);//newbook�� ���ǻ� ���� �Ҵ�

        fscanf(bfp,"%[^|]|",tmp);
        newbook->writer=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->writer,tmp);//newbook�� �۾��� ���� �Ҵ�

        fscanf(bfp,"%[^|]|",newbook->isbn);//newbook�� isbn ���� �Ҵ�

        fscanf(bfp,"%[^|]|",tmp);
        newbook->have=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->have,tmp);//newbook�� ����ó ���� �Ҵ�

        fscanf(bfp,"%c\n",&tmp[0]);
        newbook->canbor=tmp[0];//newbook�� ���ȴ����� ���� ���� �Ҵ�
    }

    while(!feof(borfp))//������ �������� Ȯ��
    {
      if(fread(&tmpcur,1,1,borfp)==0) break;//������ ���鹮���� ��� ����� ������� �����������Ƿ� �ٽð˻�
      else fseek(borfp,-1,SEEK_CUR);//�˻��ϸ鼭 ������ Ȯ�������Ƿ� �����͸� ������ġ�� �ű�


      borrow *newbor=(borrow*)malloc(sizeof(borrow));//���ο� ��� ����
      newbor->next=plist->borhead->next;//newbor�� borhead->next�� ����Ű����
      plist->borhead->next=newbor;//borhead->next�� ���ο� ��带 ����Ű����

      fscanf(borfp,"%[^|]|",tmp);
      strcpy(newbor->id,tmp);//newbor�� id���� �Ҵ�

      fscanf(borfp,"%d|",&newbor->booknum);//newbor�� booknum���� �Ҵ�

      fscanf(borfp,"%ld|%ld\n",&newbor->borday,&newbor->retday);//newbor�� �뿩 ��¥�� �ݳ� ��¥ �Ҵ�
    }
    saveinfo(plist);//���� �Լ��� ȣ���Ͽ� ���Ľ�Ų�� ����
}

void sortidcmp(list *plist){//id �������� ����
    client *curClient = plist->chead;//���������� �ϱ����� ó�� ��ġ�� ����
    while(check(curClient->next)){//���������� ����
        client *compare=curClient->next;//���������� ���ϱ� ���� ����
        client *tmp=curClient;//id�� ū�� ã������ ����
        while(check(compare->next)){//���� ��ġ���� ������ �˻�
            if(strcmp(compare->next->id,tmp->next->id)<0) tmp=compare;//id�� ������� ������ �ٲٱ� ���� �ּ� ����
            compare=compare->next;//�������� �̵�
        }

        if(check(tmp->next)&&tmp!=curClient){//tmp�� null�� ����Ű�ų� �������� �ʾ�����츦 �����ϰ� ����
            char tmpid[9];
            strcpy(tmpid,curClient->next->id);
            strcpy(curClient->next->id,tmp->next->id);
            strcpy(tmp->next->id,tmpid);//id ����

            char *tmpch;
            tmpch=tmp->next->pwd;
            tmp->next->pwd=curClient->next->pwd;
            curClient->next->pwd=tmpch;//pwd ����

            tmpch=tmp->next->name;
            tmp->next->name=curClient->next->name;
            curClient->next->name=tmpch;//name ����

            tmpch=tmp->next->address;
            tmp->next->address=curClient->next->address;
            curClient->next->address=tmpch;//address ����

            tmpch=tmp->next->phonenum;
            tmp->next->phonenum=curClient->next->phonenum;
            curClient->next->phonenum=tmpch;//phonenum ����
        }
        curClient=curClient->next;//�������� �̵�
    }
}

void sortisbncmp(list *plist){//isbn�������� ����
    book *curBook = plist->bhead;//���������� �ϱ����� ó�� ��ġ�� ����
    while(check(curBook->next)){//���������� ����
        book *compare=curBook->next;//���������� ���ϱ� ���� ����
        book *tmp=curBook;//isbn�� ū�� ã������ ����
        while(check(compare->next)){//���� ��ġ���� ������ �˻�
            if(strcmp(compare->next->isbn,tmp->next->isbn)<0) tmp=compare;//isbn�� ������� ������ �ٲٱ� ���� �ּ� ����
            compare=compare->next;//�������� �̵�
        }

        if(check(tmp->next)&&tmp!=curBook){//tmp�� null�� ����Ű�ų� �������� �ʾ�����츦 �����ϰ� ����
            char tmpisbn[15];
            strcpy(tmpisbn,curBook->next->isbn);
            strcpy(curBook->next->isbn,tmp->next->isbn);
            strcpy(tmp->next->isbn,tmpisbn);//isbn ����

            char tmpbor=curBook->next->canbor;
            curBook->next->canbor=tmp->next->canbor;
            tmp->next->canbor=tmpbor;//canbor ����

            int tmpbknum=curBook->next->booknum;
            curBook->next->booknum=tmp->next->booknum;
            tmp->next->booknum=tmpbknum;//booknum ����

            char *tmpch;
            tmpch=tmp->next->bookname;
            tmp->next->bookname=curBook->next->bookname;
            curBook->next->bookname=tmpch;//bookname ����

            tmpch=tmp->next->publicier;
            tmp->next->publicier=curBook->next->publicier;
            curBook->next->publicier=tmpch;//publicier ����

            tmpch=tmp->next->writer;
            tmp->next->writer=curBook->next->writer;
            curBook->next->writer=tmpch;//writer ����

            tmpch=tmp->next->have;
            tmp->next->have=curBook->next->have;
            curBook->next->have=tmpch;//have ����
        }
        curBook=curBook->next;//�������� �̵�
    }
}

void saveinfo(list *plist){//���� ���� �Լ�
    sortidcmp(plist);
    sortisbncmp(plist);
    FILE *idfp=fopen("client.txt","w");//client ������ �����ϱ����� w�� ���� �ʱ�ȭ


    client *ctmp=plist->admin;
    fprintf(idfp,"00000000|%s|%s|%s|%s\n",ctmp->pwd,ctmp->name,ctmp->address,ctmp->phonenum);//admin ���� ����

    ctmp=plist->chead;
    while(check(ctmp->next))//�޸� �󿡼� �������� client���� ��� ����
    {
      fprintf(idfp,"%s|%s|%s|%s|%s\n",ctmp->next->id,ctmp->next->pwd,ctmp->next->name,ctmp->next->address,ctmp->next->phonenum);
      ctmp=ctmp->next;
    }
    fclose(idfp);//���� ������ �������� �ݾ���

    FILE *bfp=fopen("book.txt","w");//book ������ �����ϱ����� w�� ���� �ʱ�ȭ
    book *btmp=plist->bhead;
    while(check(btmp->next))//�޸� �󿡼� �������� book���� ��� ����
    {
      fprintf(bfp,"%d|%s|%s|%s|%s|%s|%c\n",btmp->next->booknum,btmp->next->bookname,btmp->next->publicier,btmp->next->writer,btmp->next->isbn,btmp->next->have,btmp->next->canbor);
      btmp=btmp->next;
    }
    fclose(bfp);//���� ������ �������� �ݾ���

    FILE *borfp=fopen("borrow.txt","w");//borrow ������ �����ϱ����� w�� ���� �ʱ�ȭ
    borrow *bortmp=plist->borhead;
    while(check(bortmp->next))//�޸� �󿡼� �������� borrow���� ��� ����
    {
      fprintf(idfp,"%s|%d|%ld|%ld\n",bortmp->next->id,bortmp->next->booknum,bortmp->next->borday,bortmp->next->retday);
      bortmp=bortmp->next;
    }
    fclose(borfp);//���� ������ �������� �ݾ���
}

void menuadmin(list *plist){//������ �޴� ȣ��
    while(1)
    {
	    printf("\n>> ������ �޴� <<\n");
	    printf("1. ���� ���\t\t2. ���� ����\n");
	    printf("3. ���� �뿩\t\t4. ���� �ݳ�\n");
	    printf("5. ���� �˻�\t\t6. ȸ�� ���\n");
	    printf("7. �α׾ƿ�\t\t8. ���α׷� ����\n");
      int admenu=0;
      printf("������ ����� �Է��ϼ��� : ");
      scanf("%d",&admenu);
      getchar();
      printf("==============================\n");
      switch(admenu)
      {
        case 1:
          addbook(plist);//���� ��� �Լ� ȣ��
          break;
        case 2:
          delbook(plist);//���� ���� �Լ� ȣ��
          break;
        case 3:
          borbook(plist);//���� �뿩 �Լ� ȣ��
          break;
         case 4:
            retbook(plist);//���� �ݳ� �Լ� ȣ��
            break;
        case 5:
            searchBookMenu(plist);//���� �˻� �޴� �Լ� ȣ��
            break;
        case 6:
            memberlistmenu(plist);//ȸ�� ��� �Լ� ȣ��
            break;
        case 7:
            return;//�α׾ƿ�
        case 8:
            end(plist);//���α׷� ���� �Լ� ȣ��
            break;
      }
    }
}

void addbook(list *plist){//���� ��� �Լ�
    book *newbook=(book*)malloc(sizeof(book));//���ο� �������� �Է¹��� ��� ����
    newbook->next=plist->bhead->next;//���ο� ��� �ڿ� ��� ���� ��带 ������ �� ��� ������ ���ο� ���� ����
    plist->bhead->next=newbook;

    printf(">> ���� ��� <<\n������: ");//����� ������ �Է�
    gets(tmp);
    newbook->bookname=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->bookname,tmp);

    printf("���ǻ�: ");//����� ���ǻ��
    gets(tmp);
    newbook->publicier=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->publicier,tmp);

    printf("���ڸ�: ");//����� ���ڸ�
    gets(tmp);
    newbook->writer=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->writer,tmp);

    printf("ISBN: ");//����� isbn
    gets(newbook->isbn);

    printf("����ó: ");//����� ����ó
    gets(tmp);
    newbook->have=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->have,tmp);

    int last=0;
    book *btmp=plist->bhead;//������ booknum�� ã��
    while(check(btmp->next)){
        if(last<btmp->next->booknum){
            last=btmp->next->booknum;
        }
        btmp=btmp->next;
    }

    newbook->canbor='Y';//canbor�� y�� ����
    newbook->booknum=last+1;//������ booknum���� 1ũ�� ����

    printf("\n\n�ڵ��Է� ����\n\n");//�ڵ��Է� ���� ���
    printf("�뿩���� ���� : %c\n",newbook->canbor);
    printf("������ȣ: %d\n",newbook->booknum);


    saveinfo(plist);//������ ����Ǿ����� ���� �Լ� ���
}

void delbook(list *plist){//���� ���� �Լ� ���
    printf(">> ���� ���� <<\n1. ������ �˻�\t\t2. ISBN �˻�\n==============================\n");
      int dmenu=0;
      printf("�˻� ��ȣ�� �Է��ϼ���: ");
      scanf("%d",&dmenu);
      getchar();
      switch(dmenu)
      {
        case 1:
          delbookname(plist);//�̸����� ����
          break;
        case 2:
          delisbn(plist);//isbn���� ����
          break;
      }

    saveinfo(plist);//���� ���� ����
}

void delbookname(list *plist){//�̸����� å ����
    printf("�������� �Է��ϼ���: ");
    gets(tmp);
    printf("\n\n>> �˻� ��� <<\n");;
    printf("������ȣ: ");
    book *btmp=plist->bhead;
    int cnt=0;
    while(check(btmp->next)){//�̸��� ��ġ�ϴ� ��� å �˻�
        if(strcmp(btmp->next->bookname,tmp)==0&&cnt==0){
            printf("%d(���� ���� ���� : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        else if(strcmp(btmp->next->bookname,tmp)==0){
            printf(", %d(���� ���� ���� : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        btmp=btmp->next;
    }

    if(cnt==0){//�˻��� ������ ���� ��� �Լ� ����
        printf("������ �������� �ʽ��ϴ�.\n");
        return;
    }

    btmp=plist->bhead;

    while(check(btmp->next)){//å ������ ����ϱ� ���� �̵���Ŵ
        if(strcmp(btmp->next->bookname,tmp)==0) break;
        btmp=btmp->next;
    }

    printf("\n������: %s\n",btmp->next->bookname);//å ���� ���
    printf("���ǻ�: %s\n",btmp->next->publicier);
    printf("���ڸ�: %s\n",btmp->next->writer);
    printf("ISBN: %s\n",btmp->next->isbn);
    printf("����ó: %s\n\n",btmp->next->have);

    printf("������ ������ ��ȣ�� �Է��ϼ���: ");//�˻��� ������ ������ booknum�Է¹���
    int delnum=0;
    scanf("%d",&delnum);
    getchar();

    btmp=plist->bhead;
    while(check(btmp->next)){
        if(btmp->next->booknum==delnum){
            if(btmp->next->canbor=='N'){//canbor�� n �ϰ�� ������ �� ����
                printf("�� ������ ������ �� �����ϴ�.\n");
                return;
            }
            else{//canbor�� y�� ��� ��带 ������
                printf("���� �Ǿ����ϴ�.\n");
                book *tmpdel=btmp->next;//������ ��带 free�ϱ� ���� �ӽ� ����
                btmp->next=btmp->next->next;
                free(tmpdel->bookname);//������ ����� ���� free
                tmpdel->bookname=NULL;
                free(tmpdel->publicier);
                tmpdel->publicier=NULL;
                free(tmpdel->writer);
                tmpdel->writer=NULL;
                free(tmpdel->have);
                tmpdel->have=NULL;
                free(tmpdel);
                tmpdel=NULL;
                saveinfo(plist);//���� ���� �Լ� ȣ��
                return;
            }
        }
        btmp=btmp->next;
    }

    printf("\nã���ô� ������ �����ϴ�.\n");//�������� �ʾ������� �˻��� ������ ���°� ���� �ȳ��� ����� �Լ� ����
}

void delisbn(list *plist){//isbn���� ����
    printf("ISBN�� �Է��ϼ���: ");
    char tmpisbn[15];
    gets(tmpisbn);
    printf("\n\n>> �˻� ��� <<\n");;
    printf("������ȣ: ");
    book *btmp=plist->bhead;
    int cnt=0;
    while(check(btmp->next)){//isbn�� ��ġ�ϴ� ��� å �˻�
        if(strcmp(btmp->next->isbn,tmpisbn)==0&&cnt==0){
            printf("%d(���� ���� ���� : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        else if(strcmp(btmp->next->isbn,tmp)==0){
            printf(", %d(���� ���� ���� : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        btmp=btmp->next;
    }

    if(cnt==0){//�˻��� ������ ���� ��� �Լ� ����
        printf("������ �������� �ʽ��ϴ�.\n");
        return;
    }

    btmp=plist->bhead;

    while(check(btmp->next)){//å ������ ����ϱ� ���� �̵���Ŵ
        if(strcmp(btmp->next->isbn,tmpisbn)==0) break;
        btmp=btmp->next;
    }

    printf("\n������: %s\n",btmp->next->bookname);//å ���� ���
    printf("���ǻ�: %s\n",btmp->next->publicier);
    printf("���ڸ�: %s\n",btmp->next->writer);
    printf("ISBN: %s\n",btmp->next->isbn);
    printf("����ó: %s\n\n",btmp->next->have);

    printf("������ ������ ��ȣ�� �Է��ϼ���: ");//�˻��� ������ ������ booknum�Է¹���
    int delnum=0;
    scanf("%d",&delnum);
    getchar();

    btmp=plist->bhead;
    while(check(btmp->next)){
        if(btmp->next->booknum==delnum){
            if(btmp->next->canbor=='N'){//canbor�� n �ϰ�� ������ �� ����
                printf("�� ������ ������ �� �����ϴ�.\n");
                return;
            }
            else{//canbor�� y�� ��� ��带 ������
                printf("���� �Ǿ����ϴ�.\n");
                book *tmpdel=btmp->next;//������ ��带 free�ϱ� ���� �ӽ� ����
                btmp->next=btmp->next->next;
                free(tmpdel->bookname);//������ ����� ���� free
                tmpdel->bookname=NULL;
                free(tmpdel->publicier);
                tmpdel->publicier=NULL;
                free(tmpdel->writer);
                tmpdel->writer=NULL;
                free(tmpdel->have);
                tmpdel->have=NULL;
                free(tmpdel);
                tmpdel=NULL;
                saveinfo(plist);//���� ���� �Լ� ȣ��
                return;
            }
        }
        btmp=btmp->next;
    }
    printf("\nã���ô� ������ �����ϴ�.\n");//�������� �ʾ������� �˻��� ������ ���°� ���� �ȳ��� ����� �Լ� ����
}

void addclient(list *libInfo)//ȸ�� ���� �Լ�
{
	client *newClient;//���ο� ȸ���� �Է¹��� ��� ����
	newClient=(client*)malloc(sizeof(client));

	printf("\n\n\n>>ȸ�� ����<<\n");
	printf("�й�, ��й�ȣ, �̸�, �ּ�, ��ȭ��ȣ�� �Է��ϼ���.\n\n");

    int cflag=0;//������ id�� �����ϴ��� Ȯ���ϴ� flag
    while(1) {
        cflag=0;
        printf("�й�: ");
    	gets(tmp);
        client *ctmp=libInfo->chead;
        while(check(ctmp->next)){//id�� ������ �� �����ϴ��� Ȯ��
            if(strcmp(ctmp->next->id,tmp)==0){//�����Ѱ� �����ϹǷ� �ݺ��� flag��ȭ �� �ݺ��� ����
                cflag++;
                break;
            }
            ctmp=ctmp->next;
        }
        if(cflag!=0){//id�� �ٽ� �Է½�Ŵ
            printf("�ٽ� �Է��� �ּ���.\n");
        }
        else break;
    }
	strcpy(newClient->id, tmp);

	printf("��й�ȣ: ");//pwd�Է�
	gets(tmp);
	newClient->pwd=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->pwd, tmp);

	printf("�̸�: ");//name�Է�
	gets(tmp);
	newClient->name=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->name, tmp);

	printf("�ּ�: ");//addresss�Է�
	gets(tmp);
	newClient->address=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->address, tmp);

	printf("��ȭ��ȣ: ");//phonenum�Է�
	gets(tmp);
	newClient->phonenum=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->phonenum, tmp);

	newClient->next=libInfo->chead->next;//���ο� ����� next�� ����� ���� ���� ����
	libInfo->chead->next=newClient;//����� next�� ���ο� ���� ����
	saveinfo(libInfo);//���� ���� �Լ� ȣ��

	printf("\n\nȸ�������� �Ǽ̽��ϴ�.\n\n\n");



}

client *searchClient(list *libinfo, char* ClientID)//clientid�� ��ġ�ϴ� client ��ȯ
{
    client *curClient=libinfo->chead;
    while(curClient->next!=NULL){
        if(strcmp(curClient->next->id, ClientID)==0){
            return curClient->next;
        }
        curClient=curClient->next;
    }
    return NULL;
}

void login(list *libinfo)//�α��� �Լ�
{
    char idcmp[9];
    char pwcmp[100];
    while(1){
        printf(">>�α���<<\n");
        printf("�й�: ");
        gets(idcmp);
        printf("��й�ȣ: ");
        gets(pwcmp);
        if(strcmp(idcmp,"admin")==0){//admin�� ��� admin pwdȮ�� �� �����ڸ޴� ȣ��
            strcpy(idcmp,"00000000");
            if(strcmp(pwcmp,libinfo->admin->pwd)==0){
                menuadmin(libinfo);
                break;
            }
            else{//�ٽ� id �Է�
                printf("\n��й�ȣ�� ���� �ʽ��ϴ�.\n");
                continue;
            }
        }
        client *loginClient = searchClient(libinfo, idcmp);
        if(loginClient==NULL){//�˻��� ȸ���� �������
            printf("���Ե��� ���� ȸ���Դϴ�.\n");//�ȳ��� ����� �ٽ� �Է�
        }
        else{//�˻��� ȸ���� �������
                if(strcmp(loginClient->pwd, pwcmp)==0){//�Էµ� pwd�� �´��� Ȯ���� ������ ȸ���޴� ȣ��
                    clientMenu(libinfo, idcmp);
                    break;
                }
                else{
                    printf("\n��й�ȣ�� ���� �ʽ��ϴ�.\n");//��й�ȣ�� Ʋ�������� �ٽ� �ݺ�
                }
        }
    }
}

void clientMenu(list *libinfo, char *clientID)//ȸ�� �޴� ȣ��
{
    while(1){
        printf("\n>>ȸ�� �޴�<<\n");
        printf("1. ���� �˻�\t\t2. �� �뿩 ���\n");
        printf("3. �������� ����\t4. ȸ�� Ż��\n");
        printf("5. �α׾ƿ�\t\t6. ���α׷� ����\n");
        printf("\n��ȣ�� �����ϼ���: ");
        int menu=0;
        scanf("%d",&menu);
        getchar();
        switch(menu)
        {
            case 1:
                searchBookMenu(libinfo);//å �˻� �޴� �Լ� ȣ��
                break;
            case 2:
                borBookList(libinfo, clientID);//å �뿩 ��� �Լ� ȣ��
                break;
            case 3:
                fixClient(libinfo, clientID);//ȸ�� ���� �Լ� ȣ��
                break;
            case 4:
                delClient(libinfo, clientID);//ȸ�� ���� �Լ� ȣ��
                break;
            case 5:
                return;//�α׾ƿ�
            case 6:
                end(libinfo);//���α׷� ���� �Լ� ȣ��
                return;
        }
    }
}

void searchBookMenu(list *libinfo)//å �˻� �޴� �Լ�
{
    while(1){
        printf("\n>>���� �˻�<<\n");
        printf("1. ������ �˻�\t2. ���ǻ� �˻�\n");
        printf("3. ISBN �˻�\t4. ���ڸ� �˻�\n");
        printf("5. ��ü �˻�\t6. ���� �޴�\n");
        printf("\n��ȣ�� �����ϼ���: ");
        int menu;
        scanf("%d",&menu);
        getchar();
        switch(menu)
        {
            case 1:
                bookNameSearch(libinfo);//���������� �˻�
                break;
            case 2:
                bookPubSearch(libinfo);//���ǻ�� �˻�
                break;
            case 3:
                bookIsbnSearch(libinfo);//isbn���� �˻�
                break;
            case 4:
                bookWritSearch(libinfo);//���ڸ����� �˻�
                break;
            case 5:
                bookTotSearch(libinfo);//��ü ���
                break;
            case 6:
                return;//���� �޴�
        }
    }
}

void bookPrint(book *curBook)//å ���� ���
{
    printf("������: %s\n", curBook->bookname);
    printf("���ǻ�: %s\n", curBook->publicier);
    printf("���ڸ�: %s\n", curBook->writer);
    printf("ISBN: %s\n", curBook->isbn);
    printf("����ó: %s\n", curBook->have);
    printf("�뿩 ���� ����: %c", curBook->canbor);
}

void bookNameSearch(list *libinfo)//���������� �˻�
{
    printf("\n�������� �Է��ϼ���: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//�Է¹��� ������� ��ġ�ϴ� ���� �˻��� ���
        if(strcmp(curBook->next->bookname, tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=��ü å �� �� cant=������ ���� å �� ��
    curBook=libinfo->bhead;
    while(check(curBook->next)){//�뿩 ���� ���
        if(strcmp(curBook->next->bookname, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint�� canbor���� ����
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0�� ��� å�� �������� �����Ƿ� ��� ����
}

void bookPubSearch(list *libinfo)//���ǻ�� �˻�
{
    printf("\n���ǻ縦 �Է��ϼ���: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//�Է¹��� ���ǻ� ��ġ�ϴ� ���� �˻��� ���
        if(strcmp(curBook->next->publicier, tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=��ü å �� �� cant=������ ���� å �� ��
    curBook=libinfo->bhead;
    while(check(curBook->next)){//�뿩 ���� ���
        if(strcmp(curBook->next->publicier, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint�� canbor���� ����
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0�� ��� å�� �������� �����Ƿ� ��� ����
}

void bookIsbnSearch(list *libinfo)//isbn���� �˻�
{
    printf("\nISBN�� �Է��ϼ���: ");
    char a[15];
    scanf("%s", a);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//�Է¹��� isbn�� ��ġ�ϴ� ���� �˻��� ���
        if(strcmp(curBook->next->isbn,a)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=��ü å �� �� cant=������ ���� å �� ��
    curBook=libinfo->bhead;
    while(check(curBook->next)){//�뿩 ���� ���
        if(strcmp(curBook->next->isbn, a)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint�� canbor���� ����
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0�� ��� å�� �������� �����Ƿ� ��� ����
}

void bookWritSearch(list *libinfo)//���ڸ����� �˻�
{
    printf("\n���ڸ��� �Է��ϼ���: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//�Է¹��� ���ڸ� ��ġ�ϴ� ���� �˻��� ���
        if(strcmp(curBook->next->writer,tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=��ü å �� �� cant=������ ���� å �� ��
    curBook=libinfo->bhead;
    while(check(curBook->next)){//�뿩 ���� ���
        if(strcmp(curBook->next->writer, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint�� canbor���� ����
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0�� ��� å�� �������� �����Ƿ� ��� ����
}

void bookTotSearch(list *libinfo)//å ���� ��ü ���
{
    book *curBook=libinfo->bhead;
    printf("\n");
    while(check(curBook->next)){//������ ���
        bookPrint(curBook->next);
        printf("\n");
        curBook=curBook->next;
    }
}

char *borBookName(list *libinfo, int bNum)//���� å �̸�
{
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//booknum���� ��ġ�ϴ� booknum�˻� �� ��ġ�ϸ� ��ȯ
        if(curBook->next->booknum==bNum){
            return curBook->next->bookname;
        }
        else    curBook=curBook->next;
    }
    return NULL;
}

void borBookList(list *libinfo, char *ClientID)//���� å ����Ʈ
{
    printf("\n>>�� �뿩 ���<<\n");
    borrow *curBor=libinfo->borhead;
    while(check(curBor->next)){
        if(strcmp(curBor->next->id, ClientID)==0){//���� ȸ�� id�� ��ġ�ϴ��� �˻�
            printf("\n���� ��ȣ: %d\n", curBor->next->booknum);//���� ���� ��ȣ
            printf("������: %s\n", borBookName(libinfo, curBor->next->booknum));//���� ������
            const char *day[] = {"��","��","ȭ","��","��","��","��"};
            struct tm* bor_time = localtime(&(curBor->next->borday));//���� ��¥ ���
            printf("�뿩����: %d�� %d�� %d�� %s����\n",bor_time->tm_year+1900, bor_time->tm_mon+1, bor_time->tm_mday, day[bor_time->tm_wday]);
            struct tm* ret_time = localtime(&(curBor->next->retday));//�ݳ� ��¥ ���
            printf("�ݳ�����: %d�� %d�� %d�� %s����\n",ret_time->tm_year+1900, ret_time->tm_mon+1, ret_time->tm_mday, day[ret_time->tm_wday]);
        }
        curBor=curBor->next;
    }
}


void fixClient(list *libinfo, char *ClientID)//ȸ�� ���� ����
{
    client *curClient = searchClient(libinfo, ClientID);//���� ȸ�� ����
    printf("�й�: %s\n", curClient->id);//id ���
    printf("�̸�: %s\n", curClient->name);//name ���

    printf("��й�ȣ: ");//pwd ����
    gets(tmp);
    curClient->pwd = (char*)realloc(curClient->pwd,sizeof(char)*strlen(tmp));
    strcpy(curClient->pwd, tmp);

    printf("�ּ�: ");//address ����
    gets(tmp);
    curClient->address = (char*)realloc(curClient->address,sizeof(char)*strlen(tmp));
    strcpy(curClient->address, tmp);

    printf("��ȭ��ȣ: ");//phonenum ����
    gets(tmp);
    curClient->phonenum = (char*)realloc(curClient->phonenum,sizeof(char)*strlen(tmp));
    strcpy(curClient->phonenum, tmp);

    saveinfo(libinfo);//���� ���� ȣ��

    printf("\n\nȸ�������� �����Ǿ����ϴ�.\n\n\n");//���� �Ǿ��ٴ� �ȳ��� ���
}

void delClient(list *libinfo, char *ClientID)//ȸ�� ���� �Լ�
{
    client *curClient=libinfo->chead;
    client *preClient=NULL;
    borrow *bortmp=libinfo->borhead;
    while(check(curClient->next)){
        if(strcmp(curClient->next->id, ClientID)==0){
            while(check(bortmp->next)){
                if(strcmp(bortmp->next->id,curClient->next->id)==0){//���� ������ �����Ƿ� ���� �Ұ�
                    printf("�뿩����� �����Ƿ� �����Ͻ� �� �����ϴ�.\n");
                    return;//�Լ� �����Ŵ
                }
                bortmp=bortmp->next;
            }
            preClient=curClient->next;//������ ����� ��ġ ����
            curClient->next=curClient->next->next;//��� ����
            free(preClient->pwd);//��� ������ ����
            preClient->pwd=NULL;
            free(preClient->name);
            preClient->name=NULL;
            free(preClient->address);
            preClient->address=NULL;
            free(preClient->phonenum);
            preClient->phonenum=NULL;
            free(preClient);
            preClient=NULL;
            saveinfo(libinfo);//���� ���� ȣ��
            main();//���� ������ ���� �޴��� �̵�
            return;
        }
        curClient=curClient->next;
    }
}
void borbook(list *plist) { // ���� �뿩 UI�� ����� �ְ� å �̸��� ISBN���� å�� �˻��ϴ� �Լ��� �Ѿ�� ����
	bool loop_end = false;
	while(!loop_end){
		printf(">> ���� �뿩 <<\n");
		printf("1. ������ �˻�\t2. ISBN �˻�\n");
	    int borbook_menu=0;
	    printf("�˻� ��ȣ�� �Է��ϼ���: ");
	    scanf("%d",&borbook_menu);
	    getchar();
	    printf("==============================\n");
	    switch(borbook_menu){
			case 1:
				loop_end = search_by_book_name(plist);
				break;
			case 2:
				loop_end = search_by_ISBN(plist);
				break;
		}
	}
	saveinfo(plist);

}


bool can_bor(book *pre_ad, int i){			//���� ������ �˻� �ؼ� ������ �ִ� å�� �ѱ��̶� ���� ������ ��� true �� ��������
	int k;
	bool result = false;
	for(k = 0; k < i; k++){
		if(pre_ad[k].next->canbor == 'Y'){
			result = true;
			break;
		}
	}
	return result;
}

bool can_bor_2(book *pre_ad, int size, int booknum){//���� �������� �ϴ� ������ȣ�� å�� ���� �����̸� true �� ��������
	int k;
	bool result = false;
	for(k = 0; k < size; k++){
		if(pre_ad[k].next->booknum == booknum){
			if (pre_ad[k].next->canbor =='Y') {
				result = true;
				break;
			}
		}
	}
	return result;
}

bool search_by_book_name(list *plist) {//å �̸����� ���� �˻��� ���� ���� �ش� ������ ������ ����ϰ� �й��� �Է� �޾� borrow ���Ͽ� �ʿ��� ������ ������
	int size;
	int i = 0;
	printf("�������� �Է��ϼ���: ");
    gets(tmp);
    printf("\n\n>> �˻� ��� <<\n");

    book *btmp = plist->bhead;
	char *ptr;

	size = 0;
	while(check(btmp->next)){
    	ptr = strstr(btmp->next->bookname,tmp);
        if(ptr != NULL){
			size++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }
	book *pre_ad = (book *)malloc(sizeof(book) * size);

	btmp = plist->bhead; // btmp �� ó�� ��ġ��

    i = 0;
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->bookname,tmp);
        if(ptr != NULL){
		    printf("������ȣ: %07d(�뿩 ���� ���� %c)",btmp->next->booknum, btmp->next->canbor);
	        pre_ad[i].next = btmp->next;
	        i++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }

    if (size == 0){
        printf("������ �������� �ʽ��ϴ�.\n");
        return false;
    }

	printf("\n");
    printf("������: %s\n",pre_ad[0].next->bookname);
    printf("���ǻ�: %s\n",pre_ad[0].next->publicier);
    printf("���ڸ�: %s\n",pre_ad[0].next->writer);
    printf("ISBN: %s\n",pre_ad[0].next->isbn);
    printf("����ó: %s\n\n",pre_ad[0].next->have);

    if (!can_bor(pre_ad, size)) {
        printf("�� ������ �뿩�� �� �����ϴ�.\n");
        return false;
    }


    printf("\n�й��� �Է��ϼ���: ");
    gets(tmp);

	client * ctmp = plist->chead;

    while(check(ctmp->next)){
        if(strcmp(ctmp->next->id, tmp) == 0) {
        	break;
        } else {
	        ctmp = ctmp->next;
        }
    }

    if(ctmp->next == NULL){
        printf("�й��� �������� �ʽ��ϴ�.\n");
        return false;
    }

    printf("\n������ȣ�� �Է��ϼ���: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

	if (!can_bor_2(pre_ad, size, tmp_book_no)) {
        printf("�� ������ �뿩�� �� �����ϴ�.\n");
        return false;
	}

    printf("\n�� ������ �뿩�մϱ�?: ");
    gets(tmp);


	if (!strcmp("Y", tmp) == 0) {
	    printf("���� �뿩�� ����Ͽ����ϴ�.\n");
		return false;
	}

	btmp = plist->bhead; // btmp �� ó�� ��ġ��,...
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->bookname,tmp);
        if(btmp->next->booknum == tmp_book_no){
        	break;
        } else {
	        btmp=btmp->next;
        }
    }

	btmp->next->canbor = 'N';

	insert_borrow_file(plist, ctmp->next->id, tmp_book_no);

	free(pre_ad);
	return true;
}

bool search_by_ISBN(list *plist) {//ISBN���� ���� �˻��� ���� ���� �ش� ������ ������ ����ϰ� �й��� �Է� �޾� borrow ���Ͽ� �ʿ��� ������ ������
	int size;
	int i = 0;
    printf("ISBN�� �Է��ϼ���: ");
    gets(tmp);
    printf("\n\n>> �˻� ��� <<\n");

    book *btmp = plist->bhead;
	char *ptr;

	size = 0;
	while(check(btmp->next)){
    	ptr = strstr(btmp->next->isbn,tmp);
        if(ptr != NULL){
			size++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }
	book *pre_ad = (book *)malloc(sizeof(book) * size);

	btmp = plist->bhead; // btmp �� ó�� ��ġ��

    i = 0;
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->isbn,tmp);
        if(ptr != NULL){
		    printf("������ȣ: %07d(�뿩 ���� ���� %c)",btmp->next->booknum, btmp->next->canbor);
	        pre_ad[i].next = btmp->next;
	        i++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }

    if (size == 0){
        printf("������ �������� �ʽ��ϴ�.\n");
        return false;
    }

	printf("\n");
    printf("������: %s\n",pre_ad[0].next->bookname);
    printf("���ǻ�: %s\n",pre_ad[0].next->publicier);
    printf("���ڸ�: %s\n",pre_ad[0].next->writer);
    printf("ISBN: %s\n",pre_ad[0].next->isbn);
    printf("����ó: %s\n\n",pre_ad[0].next->have);

    if (!can_bor(pre_ad, i)) {
        printf("�� ������ �뿩�� �� �����ϴ�.\n");
        return false;
    }


    printf("\n�й��� �Է��ϼ���: ");
    gets(tmp);

	client * ctmp = plist->chead;

    while(check(ctmp->next)){
        if(strcmp(ctmp->next->id, tmp) == 0) {
        	break;
        } else {
	        ctmp = ctmp->next;
        }
    }

    if(ctmp->next == NULL){
        printf("�й��� �������� �ʽ��ϴ�.\n");
        return false;
    }

    printf("\n������ȣ�� �Է��ϼ���: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

	if (!can_bor_2(pre_ad, size, tmp_book_no)) {
        printf("�� ������ �뿩�� �� �����ϴ�.\n");
        return false;
	}


    printf("\n�� ������ �뿩�մϱ�?: ");
    gets(tmp);

	if (!strcmp("Y", tmp) == 0) {
	    printf("���� �뿩�� ����Ͽ����ϴ�.\n");
		return false;
	}

	btmp = plist->bhead; // btmp �� ó�� ��ġ��
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->bookname,tmp);
        if(btmp->next->booknum == tmp_book_no){
        	break;
        } else {
	        btmp=btmp->next;
        }
    }

	btmp->next->canbor = 'N';

	insert_borrow_file(plist, ctmp->next->id, tmp_book_no);

	free(pre_ad);
	return true;
}


void insert_borrow_file(list *plist, char *cli_id, int book_num){//�뿩 �ϰ��� �ϴ� å ������ borrow ���Ͽ� �����ϴ� ����
	borrow *newbor=(borrow*)malloc(sizeof(borrow));
	newbor->next=plist->borhead->next;
	plist->borhead->next=newbor;

	time_t test;
	time_t now;
	time_t rday;
	time(&now);

	strcpy(newbor->id, cli_id);
    newbor->booknum = book_num;

    newbor->borday = now;
	test = newbor->borday + (30*24*60*60);

    if((test / (60*60*24) + 4)%7 == 0){
    	rday = newbor->borday + (31*24*60*60);
    	newbor->retday = rday;
	} else {
    	rday = newbor->borday + (30*24*60*60);
    	newbor->retday = rday;
	}
    printf("������ �뿩 �Ǿ����ϴ�.\n");
}

void retbook(list *plist) { //�й��� �Է� ������ �� �л��� �뿩�� å ����� ����ϰ� �ݳ�����, �������ڸ� ������� �ش� �������� want_ret�� ������
	book *bfp = plist->bhead->next;
	borrow *borfp = plist->borhead->next;

	struct tm* bor_time;
	struct tm* ret_time;
	const char *day[] = {"��", "��", "ȭ", "��", "��", "��", "��"};

		printf("�й��� �Է��ϼ���: ");
		gets(tmp);

		while(check(borfp->next)){
//			printf("%s\n",borfp->id);

			if(strcmp(borfp->id, tmp) == 0){
				printf("������ȣ:%d\n", borfp->booknum);
				while(check(bfp->next)){
					if(borfp->booknum == bfp->booknum){
						printf("������:%s\n", bfp->bookname);
						break;
					}
					bfp = bfp->next;
				}
				bor_time = localtime(&(borfp->borday));
				printf("�뿩����: %d�� %d�� %d�� %s����\n",
						bor_time->tm_year+1900, bor_time->tm_mon+1, bor_time->tm_mday, day[bor_time->tm_wday]
						);
				ret_time = localtime(&(borfp->retday));
				printf("�ݳ�����: %d�� %d�� %d�� %s����\n\n",
						ret_time->tm_year+1900, ret_time->tm_mon+1, ret_time->tm_mday, day[ret_time->tm_wday]
						);
			}

			borfp = borfp->next;
		}
		want_ret(plist, bfp, tmp);
		saveinfo(plist);
}

void want_ret(list *plist,book *bfp, char *cli_id){// �ݳ� �ϰ��� �ϴ� ������ȣ�� �Է¹ް� �ش絵���� �ݳ� ���θ� ���. Y�� �Է� ������ ruduce_borrow_file �Լ� ������ �뿩 ���� ���� Y
	printf("\n������ȣ�� �Է��ϼ���: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

    printf("\n�� ������ �ݳ�ó�� �ұ��?: ");
    gets(tmp);

	if (strcmp("Y", tmp) == 0) {
		bfp->canbor = 'Y';
		ruduce_borrow_file(plist, tmp_book_no, cli_id);

	}

}

void ruduce_borrow_file(list *plist, int tmp_book_no, char *cli_id){//�ݳ� �ϰ��� �ϴ� ������ borrow ���Ͽ��� ����� ����
	borrow *borfp = plist->borhead->next;

	borrow *front_del = plist->borhead;
	while(check(borfp->next)){
		borrow *want_del = borfp;
		if(want_del->booknum == tmp_book_no){
			front_del->next = want_del->next;
			free(want_del);
            want_del=NULL;
            saveinfo(plist);
			break;
		} else{
			front_del = want_del;
			borfp = borfp->next;
		}
	}

}

void memberlistmenu(list *plist) { //6. ȸ�� ��� �޴� ȣ��
	printf("\n>> ȸ�� ��� <<\n1. �̸� �˻�\t2. �й� �˻�\n3. ��ü �˻�\t4. ���� �޴�\n");
    int listmenu=0;
    printf("������ ����� �Է��ϼ��� : ");
    scanf("%d",&listmenu);
    getchar();
    printf("==============================\n");
    switch (listmenu) {
        case 1:
            srchbymemname(plist);//�̸����� ȸ�� �˻�
            break;
        case 2:
            srchbyid(plist);//�й����� ȸ�� �˻�
            break;
        case 3:
            srchbyall(plist);//ȸ�� ���� ��ü ���
            break;
        case 4:
            return;//���� �޴�
    }
}

void srchbymemname(list *plist){//�̸����� ȸ�� �˻�
    printf("ȸ�� �̸� : ");
    gets(tmp);
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        if(strcmp(ctmp->next->name,tmp)==0){//�Է��� ȸ�� �̸��� ��ġ�ϴ� ȸ�� ���� ���
            printf("\n==============================\n");
            printf("\n�й� : %s",ctmp->next->id);
            printf("\n�̸� : %s",ctmp->next->name);
            printf("\n�ּ� : %s",ctmp->next->address);
            printf("\n��ȭ��ȣ : %s\n",ctmp->next->phonenum);
            printf("==============================\n");
        }
        ctmp=ctmp->next;
    }
}

void srchbyid(list *plist){//�й����� ȸ�� �˻�
    printf("�й� : ");
    gets(tmp);
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        if(strcmp(ctmp->next->id,tmp)==0){//�Է��� ȸ�� id�� ��ġ�ϴ� ȸ�� ���� ���
            printf("\n==============================\n");
            printf("\n�й� : %s",ctmp->next->id);
            printf("\n�̸� : %s",ctmp->next->name);
            printf("\n�ּ� : %s",ctmp->next->address);
            printf("\n��ȭ��ȣ : %s\n",ctmp->next->phonenum);
            printf("==============================\n");
        }
        ctmp=ctmp->next;
    }
}

void srchbyall(list *plist){//��� ȸ�� ���� ���
    printf("\n");
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        printf("==============================\n");
        printf("\n�й� : %s",ctmp->next->id);
        printf("\n�̸� : %s",ctmp->next->name);
        printf("\n�ּ� : %s",ctmp->next->address);
        printf("\n��ȭ��ȣ : %s\n",ctmp->next->phonenum);
        printf("==============================\n");
        ctmp=ctmp->next;
    }
    printf("==============================\n");
}

void end(list *plist){//���α׷� ���� �Լ�
    client *ctmp=plist->admin;//admin���� �޸𸮿��� ����
    free(ctmp->pwd);
    ctmp->pwd=NULL;
    free(ctmp->name);
    ctmp->name=NULL;
    free(ctmp->address);
    ctmp->address=NULL;
    free(ctmp->phonenum);
    ctmp->phonenum=NULL;
    free(ctmp);
    ctmp=NULL;

    while(check(plist->chead->next)){//client���� �޸𸮿��� ����
        free(plist->chead->next->pwd);
        plist->chead->next->pwd=NULL;
        free(plist->chead->next->name);
        plist->chead->next->name=NULL;
        free(plist->chead->next->address);
        plist->chead->next->address=NULL;
        free(plist->chead->next->phonenum);
        plist->chead->next->phonenum=NULL;
        client *ctmp=plist->chead->next;
        plist->chead->next=plist->chead->next->next;
        free(ctmp);
        ctmp=NULL;
    }
    free(plist->chead);//client head ����
    plist->chead=NULL;

    while(check(plist->bhead->next)){//book���� �޸𸮿��� ����
        free(plist->bhead->next->bookname);
        plist->bhead->next->bookname=NULL;
        free(plist->bhead->next->publicier);
        plist->bhead->next->publicier=NULL;
        free(plist->bhead->next->writer);
        plist->bhead->next->writer=NULL;
        free(plist->bhead->next->have);
        plist->bhead->next->have=NULL;
        book *btmp=plist->bhead->next;
        plist->bhead->next=plist->bhead->next->next;
        free(btmp);
        btmp=NULL;
    }
    free(plist->bhead);//book head ����
    plist->bhead=NULL;

    while(check(plist->borhead->next)){//borrow���� �޸𸮿��� ����
        borrow *bortmp=plist->borhead->next;
        plist->borhead->next=plist->borhead->next->next;
        free(bortmp);
        bortmp=NULL;
    }
    free(plist->borhead);//borrow head ����
    plist->borhead=NULL;

    exit(0);
}

//������ : ���� ���� ���� �� ���� �� addbook,delbook,reset,main,end,saveinfo,check,menuadmin,sortidcmp,sortisbncmp,delisbn,delbookname�� ��ü Ʋ ����
//�ȼ��� : client.txt, book.txt, borrow.txt ���� ���� ����
//�̼��� : ����üũ �� borbook,retbook,memberlistmenu,srchbymemname,srchbyid,srchbyall,menubooksearch,search_by_ISBN,search_by_book_name,insert_borrow_file,want_ret,ruduce_borrow_file,can_bor,can_bor_2 ����
//�Ӱ��� : addclient,searchClient,login,clientMenu,searchBookMenu,bookPrint,bookNameSearch,bookPubSearch,bookIsbnSearch,bookWritSearch,bookTotSearch,borBookName,borBookList,fixClient,delClient ����
