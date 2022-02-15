#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct account_info
{
  char name[40];
  char address[400];
  char phonenum[40];
  struct account_info *next;
} account;

typedef struct _linkedList
{
  account *head;
  account *cur;
  int numofdata;
} list;

int num;

int checknext(list *plist);
int check(list *plist);//NULL���� Ȯ��
void reset(list *plist,FILE *fps);//�ʱ�ȭ
void show(list *plist);//�����ֱ�
void add(list *plist);//�� �߰�
void search(list *plist);//�˻�
void modified(list *plist);//����
void del(list *plist);//����
void end(list *plist,FILE *fps);//��
void searchphone(list *plist);//��ȣ�˻�
void searchname(list *plist);//�̸��˻�
void searchaddress(list *plist);//�ּҰ˻�


int main()//�޴�
{
  FILE *fp;
  fp=fopen("���÷� ��ܰ���.txt","r");
  if(fp==NULL)
    fp=fopen("���÷� ��ܰ���.txt","w+");


  list hangeul;
  reset(&hangeul,fp);
  printf("        = �޴� =\n1. ��� �ּҷ� ���� ���\n2. ȸ�� ���\n3. ȸ�� ���� �˻�\n4. ȸ�� ���� ����\n5. ȸ�� ����\n6. ���α׷� ����\n==============================\n");
  while(1)
  {
    int menu=0;
    printf("������ ����� �Է��ϼ��� : ");
    scanf("%d",&menu);
    getchar();
    printf("==============================\n");
    switch(menu)
    {
      case 1:
        show(&hangeul);
        break;
      case 2:
        add(&hangeul);
        break;
      case 3:
        search(&hangeul);
        break;
      case 4:
        modified(&hangeul);
        break;
      case 5:
        del(&hangeul);
        break;
      case 6:
        printf("���α׷��� �����մϴ�.\n");
        end(&hangeul,fp);
        return 0;
      default:
        printf("�ٸ� ���� �Է��� �ֽʽÿ�.\n");
        break;
    }
  }
}

int checknext(list *plist)
{
  if(plist->cur->next==NULL)
    return 0;
  else
    return 1;
}

int check(list *plist)
{
  if(plist->cur==NULL)
    return 0;
  else
    return 1;
}

void reset(list *plist,FILE *fps)
{
  plist->head=(account*)malloc(sizeof(account));
  plist->head->next=NULL;
  plist->cur=NULL;
  plist->numofdata=0;
  char tmp;
  while(!feof(fps))
  {
    if(fread(&tmp,1,1,fps)==0) break;
    else fseek(fps,-1,SEEK_CUR);
    account *newacc=(account*)malloc(sizeof(account));
    fscanf(fps,"%[^\n]\n%[^\n]\n%[^\n]\n\n",newacc->name,newacc->address,newacc->phonenum);
    if(!check(plist))
    {
      newacc->next=plist->head->next;
      plist->head->next=newacc;
      plist->cur=plist->head->next;
    }
    else
    {
      if(strcmp(plist->head->next->name,newacc->name)>0)
      {
        plist->head->next=newacc;
        plist->cur=newacc->next;
      }
      else
      {
        while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//��������ũ�ų� ������ �ڷΰ����Ѵ�
          plist->cur=plist->cur->next;

        newacc->next=plist->cur->next;
        plist->cur->next=newacc;

      }
    }
    (plist->numofdata)++;
    plist->cur=plist->head->next;

  }
}

void show(list *plist)
{
  num=1;
  if(plist->head->next==NULL)
    printf("�Էµ� ������ �����ϴ�.\n");
  while(check(plist))
  {
    printf("#%d\n�̸� : %s\n�ּ� : %s\n��ȭ��ȣ : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
    if(!checknext(plist))
      printf("==============�޴�============\n");
    else
      printf("==============================\n");
    plist->cur=plist->cur->next;
    num++;
  }
  plist->cur=plist->head->next;
}

void add(list *plist)
{
  account *newacc=(account*)malloc(sizeof(account));
  char tmpinput[400];
  char *tmpp;

  printf("�̸� : ");
  scanf("%[^\n]",tmpinput);
  getchar();
  tmpp=(char*)malloc(sizeof(char)*strlen(tmpinput));
  strcpy(tmpp,tmpinput);
  strcpy(newacc->name,tmpp);

  printf("�ּ� : ");
  scanf("%[^\n]",tmpinput);
  getchar();
  realloc(tmpp,sizeof(char)*strlen(tmpinput));
  strcpy(tmpp,tmpinput);
  strcpy(newacc->address,tmpp);

  printf("��ȭ��ȣ : ");
  scanf("%[^\n]",tmpinput);
  getchar();
  realloc(tmpp,sizeof(char)*strlen(tmpinput));
  strcpy(tmpp,tmpinput);
  strcpy(newacc->phonenum,tmpp);
  free(tmpp);

  if(!check(plist))
  {
    newacc->next=plist->head->next;
    plist->head->next=newacc;
    plist->cur=plist->head->next;
  }
  else if(strcmp(plist->head->next->name,newacc->name)>0)
  {
    newacc->next=plist->head->next;
    plist->head->next=newacc;
  }
  else
  {
    while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//��������ũ�ų� ������ �ڷΰ����Ѵ�
      plist->cur=plist->cur->next;

    newacc->next=plist->cur->next;
    plist->cur->next=newacc;
  }
  (plist->numofdata)++;
  plist->cur=plist->head->next;
  printf("==============================\n");
}

void search(list *plist)
{
  int input;
  printf("1. �̸� �˻�\n2. �ּ� �˻�\n3. ��ȭ��ȣ �˻�\n(�ٸ� ���� �Է��ϸ� �޴��� ���ư��ϴ�.)\n������ ����� �Է��ϼ��� : ");
  scanf("%d",&input);
  getchar();
  printf("==============================\n");
  plist->cur=plist->head->next;
  switch(input)
  {
    case 1:
      searchname(plist);
      break;
    case 2:
      searchaddress(plist);
      break;
    case 3:
      searchphone(plist);
      break;
    default:
      printf("�޴��� ���ư��ϴ�.");
      break;
  }
  plist->cur=plist->head->next;
}

void searchname(list *plist)
{
  char sname[40];
  printf("�˻��� �̸��� �Է����ּ��� : ");
  scanf("%[^\n]",sname);
  getchar();
  num=1;
  while(check(plist))
  {
    if(strstr(plist->cur->name,sname)!=NULL)
    {
      printf("#%d\n�̸� : %s\n�ּ� : %s\n��ȭ��ȣ : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("ȸ���� �������� �ʽ��ϴ�.\n");
  printf("==============================\n");
  plist->cur=plist->head->next;
}

void searchaddress(list *plist)
{
  char saddress[400];
  printf("�˻��� �ּҸ� �Է����ּ��� : ");
  scanf("%[^\n]",saddress);
  getchar();
  num=1;
  while(check(plist))
  {
    if(strstr(plist->cur->address,saddress)!=NULL)
    {
      printf("#%d\n�̸� : %s\n�ּ� : %s\n��ȭ��ȣ : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("ȸ���� �������� �ʽ��ϴ�.\n");
  printf("==============================\n");
}

void searchphone(list *plist)
{
  char sphone[40];
  printf("�˻��� ��ȣ�� �Է����ּ��� : ");
  scanf("%[^\n]",sphone);
  getchar();
  num=1;
  while(checknext(plist))
  {
    if(strstr(plist->cur->name,sphone)!=NULL)
    {
      printf("#%d\n�̸� : %s\n�ּ� : %s\n��ȭ��ȣ : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("ȸ���� �������� �ʽ��ϴ�.\n");
  printf("==============================\n");
}

void modified(list *plist)
{
  char input[40];
  char ans=0;
  printf("������ ȸ���� �̸��� �Է����ּ��� : ");
  scanf("%s",input);
  getchar();
  while(check(plist))
  {
    if(strcmp(plist->cur->name,input)==0)
      break;
    plist->cur=plist->cur->next;
  }
  if(!check(plist))
    printf("�����Ͻ÷��� ȸ���� �������� �ʽ��ϴ�.\n");
  else
  {
    plist->cur=plist->head->next;
    account *newacc=(account*)malloc(sizeof(account));
    printf("�̸� : ");
    scanf("%[^\n]",newacc->name);
    getchar();
    printf("�ּ� : ");
    scanf("%[^\n]",newacc->address);
    getchar();
    printf("��ȭ��ȣ : ");
    scanf("%[^\n]",newacc->phonenum);
    getchar();
    printf("\n������ ������ ���� �Ͻðڽ��ϱ�? (Y�Է½� ���� N�Է½� ���) : ");
    scanf("%c",&ans);
    getchar();

    while(ans!='Y'&&ans!='N'&&ans!='y'&&ans!='n')
    {
      printf("YȤ�� N�� �Է����ּ��� : ");
      scanf("%c",&ans);
      getchar();
    }
    if(ans=='Y'||ans=='y')
    {
      plist->cur=plist->head->next;
      if(strcmp(plist->head->next->name,input)==0)
      {
        account *temp=plist->cur;
        plist->head->next=plist->head->next->next;
        plist->cur=plist->head->next;

        free(temp);
      }
      while(checknext(plist))
      {
        if(strcmp(plist->cur->next->name,input)==0)
        {
          account *temp=plist->cur->next;
          plist->cur->next=plist->cur->next->next;

          free(temp);
          break;
        }
        plist->cur=plist->cur->next;
      }

      if(strcmp(plist->head->next->name,newacc->name)>0)
      {
        newacc->next=plist->head->next;
        plist->head->next=newacc;
      }
      else
      {
        while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//��������ũ�ų� ������ �ڷΰ����Ѵ�
          plist->cur=plist->cur->next;

        newacc->next=plist->cur->next;
        plist->cur->next=newacc;
      }

      plist->cur=plist->head->next;
      printf("������ �����Ǿ����ϴ�.\n");
    }
    else
    {
      free(newacc);
      printf("������ ��� �Ǿ����ϴ�.\n");
    }
  }
  plist->cur=plist->head->next;
  printf("==============================\n");
}

void del(list *plist)
{
  num=0;
  char input[40];
  printf("�����Ͻ� ȸ���� �̸��� �Է����ּ��� : ");
  scanf("%[^\n]",input);
  getchar();

  if(strcmp(plist->head->next->name,input)==0)
  {
    account *temp=plist->cur;
    plist->head->next=plist->head->next->next;
    plist->cur=plist->head->next;

    free(temp);
    num++;
  }
  else
    while(checknext(plist))
    {
      if(strcmp(plist->cur->next->name,input)==0)
      {
        account *temp=plist->cur->next;
        plist->cur->next=plist->cur->next->next;
        num++;
        free(temp);
        break;
      }
      plist->cur=plist->cur->next;
    }
  if(num==0)
    printf("\nã���ô� ȸ���� �������� �ʽ��ϴ�.\n");
  else
    printf("\n�����Ǿ����ϴ�.\n");
  plist->cur=plist->head->next;
  printf("==============================\n");
}

void end(list *plist,FILE *fps)
{
  fclose(fps);
  FILE *fpw=fopen("���÷� ��ܰ���.txt","w");
  plist->cur=plist->head->next;
  while(check(plist))
  {
    fprintf(fpw,"%s\n%s\n%s\n\n",plist->cur->name,plist->cur->address,plist->cur->phonenum);
    account *temp=plist->cur;
    plist->cur=plist->cur->next;
    free(temp);
  }
  free(plist->head);
  fclose(fpw);
}
