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
int check(list *plist);//NULL인지 확인
void reset(list *plist,FILE *fps);//초기화
void show(list *plist);//보여주기
void add(list *plist);//값 추가
void search(list *plist);//검색
void modified(list *plist);//수정
void del(list *plist);//삭제
void end(list *plist,FILE *fps);//끝
void searchphone(list *plist);//번호검색
void searchname(list *plist);//이름검색
void searchaddress(list *plist);//주소검색


int main()//메뉴
{
  FILE *fp;
  fp=fopen("스플럭 명단관리.txt","r");
  if(fp==NULL)
    fp=fopen("스플럭 명단관리.txt","w+");


  list hangeul;
  reset(&hangeul,fp);
  printf("        = 메뉴 =\n1. 모든 주소록 정보 출력\n2. 회원 등록\n3. 회원 정보 검색\n4. 회원 정보 수정\n5. 회원 삭제\n6. 프로그램 종료\n==============================\n");
  while(1)
  {
    int menu=0;
    printf("선택할 기능을 입력하세요 : ");
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
        printf("프로그램을 종료합니다.\n");
        end(&hangeul,fp);
        return 0;
      default:
        printf("다른 값을 입력해 주십시오.\n");
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
        while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//오른쪽이크거나 같으면 뒤로가야한다
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
    printf("입력된 정보가 없습니다.\n");
  while(check(plist))
  {
    printf("#%d\n이름 : %s\n주소 : %s\n전화번호 : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
    if(!checknext(plist))
      printf("==============메뉴============\n");
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

  printf("이름 : ");
  scanf("%[^\n]",tmpinput);
  getchar();
  tmpp=(char*)malloc(sizeof(char)*strlen(tmpinput));
  strcpy(tmpp,tmpinput);
  strcpy(newacc->name,tmpp);

  printf("주소 : ");
  scanf("%[^\n]",tmpinput);
  getchar();
  realloc(tmpp,sizeof(char)*strlen(tmpinput));
  strcpy(tmpp,tmpinput);
  strcpy(newacc->address,tmpp);

  printf("전화번호 : ");
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
    while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//오른쪽이크거나 같으면 뒤로가야한다
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
  printf("1. 이름 검색\n2. 주소 검색\n3. 전화번호 검색\n(다른 값을 입력하면 메뉴로 돌아갑니다.)\n선택할 기능을 입력하세요 : ");
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
      printf("메뉴로 돌아갑니다.");
      break;
  }
  plist->cur=plist->head->next;
}

void searchname(list *plist)
{
  char sname[40];
  printf("검색할 이름을 입력해주세요 : ");
  scanf("%[^\n]",sname);
  getchar();
  num=1;
  while(check(plist))
  {
    if(strstr(plist->cur->name,sname)!=NULL)
    {
      printf("#%d\n이름 : %s\n주소 : %s\n전화번호 : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("회원이 존재하지 않습니다.\n");
  printf("==============================\n");
  plist->cur=plist->head->next;
}

void searchaddress(list *plist)
{
  char saddress[400];
  printf("검색할 주소를 입력해주세요 : ");
  scanf("%[^\n]",saddress);
  getchar();
  num=1;
  while(check(plist))
  {
    if(strstr(plist->cur->address,saddress)!=NULL)
    {
      printf("#%d\n이름 : %s\n주소 : %s\n전화번호 : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("회원이 존재하지 않습니다.\n");
  printf("==============================\n");
}

void searchphone(list *plist)
{
  char sphone[40];
  printf("검색할 번호를 입력해주세요 : ");
  scanf("%[^\n]",sphone);
  getchar();
  num=1;
  while(checknext(plist))
  {
    if(strstr(plist->cur->name,sphone)!=NULL)
    {
      printf("#%d\n이름 : %s\n주소 : %s\n전화번호 : %s\n",num,plist->cur->name,plist->cur->address,plist->cur->phonenum);
      printf("==============================\n");
      num++;
    }
    plist->cur=plist->cur->next;
  }
  if(num==1)
    printf("회원이 존재하지 않습니다.\n");
  printf("==============================\n");
}

void modified(list *plist)
{
  char input[40];
  char ans=0;
  printf("수정할 회원의 이름을 입력해주세요 : ");
  scanf("%s",input);
  getchar();
  while(check(plist))
  {
    if(strcmp(plist->cur->name,input)==0)
      break;
    plist->cur=plist->cur->next;
  }
  if(!check(plist))
    printf("수정하시려는 회원이 존재하지 않습니다.\n");
  else
  {
    plist->cur=plist->head->next;
    account *newacc=(account*)malloc(sizeof(account));
    printf("이름 : ");
    scanf("%[^\n]",newacc->name);
    getchar();
    printf("주소 : ");
    scanf("%[^\n]",newacc->address);
    getchar();
    printf("전화번호 : ");
    scanf("%[^\n]",newacc->phonenum);
    getchar();
    printf("\n수정된 정보를 저장 하시겠습니까? (Y입력시 저장 N입력시 취소) : ");
    scanf("%c",&ans);
    getchar();

    while(ans!='Y'&&ans!='N'&&ans!='y'&&ans!='n')
    {
      printf("Y혹은 N을 입력해주세요 : ");
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
        while(checknext(plist)&&strcmp(plist->cur->next->name,newacc->name)<=0)//오른쪽이크거나 같으면 뒤로가야한다
          plist->cur=plist->cur->next;

        newacc->next=plist->cur->next;
        plist->cur->next=newacc;
      }

      plist->cur=plist->head->next;
      printf("정보가 수정되었습니다.\n");
    }
    else
    {
      free(newacc);
      printf("수정이 취소 되었습니다.\n");
    }
  }
  plist->cur=plist->head->next;
  printf("==============================\n");
}

void del(list *plist)
{
  num=0;
  char input[40];
  printf("삭제하실 회원의 이름을 입력해주세요 : ");
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
    printf("\n찾으시는 회원이 존재하지 않습니다.\n");
  else
    printf("\n삭제되었습니다.\n");
  plist->cur=plist->head->next;
  printf("==============================\n");
}

void end(list *plist,FILE *fps)
{
  fclose(fps);
  FILE *fpw=fopen("스플럭 명단관리.txt","w");
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
