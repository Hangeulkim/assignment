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

void reset(list *plist,FILE *idfp,FILE *bfp,FILE *borfp);//초기화
void end(list *plist);//프로그램 종료

void addclient(list*);//회원 추가
client *searchClient(list*, char*);//학번에 맞는 회원정보 찾기
void login(list*);//로그인화면
void clientMenu(list*, char*);//회원화면
void searchBookMenu(list*);//도서검색화면
void bookPrint(book*);//도서정보출력
void bookNameSearch(list*);//도서이름검색
void bookPubSearch(list*);//도서출판검색
void bookIsbnSearch(list*);//도서 isbn검색
void bookWritSearch(list*);//도서 저자검색
void bookTotSearch(list*);//도서 전체 검색
char *borBookName(list*, int);//빌린책 도서번호의 도서명 리턴
void borBookList(list*, char*);//빌린책 나열
void fixClient(list*, char*);//회원정보 수정
void delClient(list*, char*);//회원탈퇴

void saveinfo(list *plist);//정보 저장


void addbook(list *plist);//1. 도서 등록
void delbook(list *plist);//2. 도서 삭제
void borbook(list *plist);//3. 도서 대여
void retbook(list *plist);//4. 도서 반납
void memberlistmenu(list *plist);//6. 회원 목록 메뉴 호출
void srchbymemname(list *plist);//1. 이름 검색
void srchbyid(list *plist);//2. 학번 검색
void srchbyall(list *plist);//3. 전체 검색
bool check(void *check);//주어진 포인터가 NULL인지 확인 NULL이 아니면 true
void menuadmin(list *plist);//admin menu호출
void menubooksearch(list *plist);//book search menu호출
void sortidcmp(list *plist);//sort id
void sortisbncmp(list *plist);//sort ISBN
void delisbn(list *plist);//isbn으로 검색 삭제
void delbookname(list *plist);//도서명으로 검색 삭제

bool search_by_book_name(list *plist);								//대출시 책 이름으로 검색하는 함수
bool search_by_ISBN(list *plist);									//대출시 ISBN 으로 검색하는 함수
void insert_borrow_file(list *plist, char *cli_id, int book_num);	//책 정보를 borrow 파일에 저장하는 함수
void want_ret(list *plist,book *bfp, char *cli_id);					//책 반납 여부를 물어보는 함수
void ruduce_borrow_file(list *plist, int tmp_book_no, char *cli_id);//반납할 책을 borrow 파일에서 삭제 하는 함수
bool can_bor(book *pre_ad, int i);									//찾고자 하는 도서가 대출이 가능한지 알려주는 함수
bool can_bor_2(book *pre_ad, int size, int booknum);				//대여 하고자 하는 도서번호의 도서의 대출 가능 여부를 알려주는 함수

char tmp[1000];

int main(){
    FILE *idfp;
    idfp=fopen("client.txt","r");//client 파일을 읽어옴
    if(idfp==NULL)//client파일이 있는지 확인
      idfp=fopen("client.txt","w+");//client 파일이 없으므로 새로만듬
    FILE *bfp;
     bfp=fopen("book.txt","r");//book 파일을 읽어옴
     if(bfp==NULL)//book 파일이 있는지 확인
        bfp=fopen("book.txt","w+");//book 파일이 없으므로 새로만듬
    FILE *borfp;
    borfp=fopen("borrow.txt","r");//borrow 파일을 읽어옴
    if(borfp==NULL)//borrow 파일이 있는지 확인
        borfp=fopen("borrow.txt","w+");//borrow 파일이 없으므로 새로만듬

    list project;//각 파일의 정보를 저장할 header리스트 생성
    reset(&project,idfp,bfp,borfp); //메모리에 client, book, borrow 정보 저장
    fclose(idfp);
    fclose(bfp);
    fclose(borfp);//파일에 정보를 다 불러왔으므로 닫음

    while(1)
    {
        printf(">> 도서관 서비스 <<\n1. 회원 가입\t2. 로그인\t3. 프로그램 종료\n==============================\n");
      int menu=0;
      printf("선택할 기능을 입력하세요 : ");
      scanf("%d",&menu);
      getchar();
      printf("==============================\n");
      switch(menu)
      {
        case 1:
          addclient(&project);//회원가입 함수 호출
          break;
        case 2:
          login(&project);//로그인 함수 호출
          break;
        case 3:
          end(&project);//종료 함수 호출
          return 0;
      }
    }
}

bool check(void *check){ //들어온 포인터가 null인지 확인하고 null이 아니면 true반환하여 다음과정 진행시킴
    if(check==NULL) return false;
    return true;
}

void reset(list *plist,FILE *idfp,FILE *bfp,FILE *borfp){//파일에 정보들을 메모리에 관리하기위해 정보를 읽어옴
    plist->admin=(client*)malloc(sizeof(client));
    plist->admin->next=NULL;
    plist->chead=(client*)malloc(sizeof(client));
    plist->chead->next=NULL;
    plist->bhead=(book*)malloc(sizeof(book));
    plist->bhead->next=NULL;
    plist->borhead=(borrow*)malloc(sizeof(borrow));
    plist->borhead->next=NULL;//list에 헤더들을 생성
    char tmpcur;

    while(!feof(idfp))//파일이 끝났는지 확인
    {
      if(fread(&tmpcur,1,1,idfp)==0) break;//마지막 공백문자인 경우 제대로 종료되지 않을수있으므로 다시검사
      else fseek(idfp,-1,SEEK_CUR);//검사하면서 다음걸 확인했으므로 포인터를 원래위치로 옮김
      fscanf(idfp,"%[^|]|",tmp);
      if(strcmp(tmp,"00000000")==0){//admin정보인지 확인
          strcpy(plist->admin->id,tmp);

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->pwd=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->pwd,tmp);//admin에 비밀번호 정보 할당

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->name=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->name,tmp);//admin에 이름 정보 할당

          fscanf(idfp,"%[^|]|",tmp);
          plist->admin->address=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->address,tmp);//admin에 주소 정보 할당

          fscanf(idfp,"%[^\n]\n",tmp);
          plist->admin->phonenum=(char*)malloc(strlen(tmp)+1);
          strcpy(plist->admin->phonenum,tmp);//admin에 전화번호 정보 할당
      }
      else{
          client *newcli=(client*)malloc(sizeof(client));//새로운 노드 생성
          newcli->next=plist->chead->next;//newcli가 chead->next를 가르키게함
          plist->chead->next=newcli;//chead->next가 새로운 노드를 가르키게함

          strcpy(newcli->id,tmp);

          fscanf(idfp,"%[^|]|",tmp);
          newcli->pwd=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->pwd,tmp);//newcli에 비밀번호 정보 할당

          fscanf(idfp,"%[^|]|",tmp);
          newcli->name=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->name,tmp);//newcli에 이름 정보 할당

          fscanf(idfp,"%[^|]|",tmp);
          newcli->address=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->address,tmp);//newcli에 주소 정보 할당

          fscanf(idfp,"%[^\n]\n",tmp);
          newcli->phonenum=(char*)malloc(strlen(tmp)+1);
          strcpy(newcli->phonenum,tmp);//newcli에 전화번호 정보 할당
      }
  }

      while(!feof(bfp))//파일이 끝났는지 확인
      {
        if(fread(&tmpcur,1,1,bfp)==0) break;//마지막 공백문자인 경우 제대로 종료되지 않을수있으므로 다시검사
        else fseek(bfp,-1,SEEK_CUR);//검사하면서 다음걸 확인했으므로 포인터를 원래위치로 옮김

        fscanf(bfp,"%[^|]|",tmp);

        book *newbook=(book*)malloc(sizeof(book));//새로운 노드 생성
        newbook->next=plist->bhead->next;//newbook이 bhead->next를 가르키게함
        plist->bhead->next=newbook;//bhead->next가 새로운 노드를 가르키게함

        newbook->booknum=atoi(tmp);//tmp로 읽어온 정보를 atoi로 정수로 바꿔 정보를 할당

        fscanf(bfp,"%[^|]|",tmp);
        newbook->bookname=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->bookname,tmp);//newbook에 책제목 정보 할당

        fscanf(bfp,"%[^|]|",tmp);
        newbook->publicier=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->publicier,tmp);//newbook에 출판사 정보 할당

        fscanf(bfp,"%[^|]|",tmp);
        newbook->writer=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->writer,tmp);//newbook에 글쓴이 정보 할당

        fscanf(bfp,"%[^|]|",newbook->isbn);//newbook에 isbn 정보 할당

        fscanf(bfp,"%[^|]|",tmp);
        newbook->have=(char*)malloc(strlen(tmp)+1);
        strcpy(newbook->have,tmp);//newbook에 소장처 정보 할당

        fscanf(bfp,"%c\n",&tmp[0]);
        newbook->canbor=tmp[0];//newbook에 빌렸는지에 대한 정보 할당
    }

    while(!feof(borfp))//파일이 끝났는지 확인
    {
      if(fread(&tmpcur,1,1,borfp)==0) break;//마지막 공백문자인 경우 제대로 종료되지 않을수있으므로 다시검사
      else fseek(borfp,-1,SEEK_CUR);//검사하면서 다음걸 확인했으므로 포인터를 원래위치로 옮김


      borrow *newbor=(borrow*)malloc(sizeof(borrow));//새로운 노드 생성
      newbor->next=plist->borhead->next;//newbor가 borhead->next를 가르키게함
      plist->borhead->next=newbor;//borhead->next가 새로운 노드를 가르키게함

      fscanf(borfp,"%[^|]|",tmp);
      strcpy(newbor->id,tmp);//newbor에 id정보 할당

      fscanf(borfp,"%d|",&newbor->booknum);//newbor에 booknum정보 할당

      fscanf(borfp,"%ld|%ld\n",&newbor->borday,&newbor->retday);//newbor에 대여 날짜와 반납 날짜 할당
    }
    saveinfo(plist);//저장 함수를 호출하여 정렬시킨후 저장
}

void sortidcmp(list *plist){//id 기준으로 솔팅
    client *curClient = plist->chead;//버블정렬을 하기위해 처음 위치를 지정
    while(check(curClient->next)){//마지막까지 정렬
        client *compare=curClient->next;//다음껏부터 비교하기 위해 선언
        client *tmp=curClient;//id가 큰걸 찾기위해 선언
        while(check(compare->next)){//다음 위치부터 끝까지 검사
            if(strcmp(compare->next->id,tmp->next->id)<0) tmp=compare;//id가 작을경우 정보를 바꾸기 위해 주소 저장
            compare=compare->next;//다음껄로 이동
        }

        if(check(tmp->next)&&tmp!=curClient){//tmp가 null을 가르키거나 움직이지 않았을경우를 제외하고 실행
            char tmpid[9];
            strcpy(tmpid,curClient->next->id);
            strcpy(curClient->next->id,tmp->next->id);
            strcpy(tmp->next->id,tmpid);//id 스왑

            char *tmpch;
            tmpch=tmp->next->pwd;
            tmp->next->pwd=curClient->next->pwd;
            curClient->next->pwd=tmpch;//pwd 스왑

            tmpch=tmp->next->name;
            tmp->next->name=curClient->next->name;
            curClient->next->name=tmpch;//name 스왑

            tmpch=tmp->next->address;
            tmp->next->address=curClient->next->address;
            curClient->next->address=tmpch;//address 스왑

            tmpch=tmp->next->phonenum;
            tmp->next->phonenum=curClient->next->phonenum;
            curClient->next->phonenum=tmpch;//phonenum 스왑
        }
        curClient=curClient->next;//다음껄로 이동
    }
}

void sortisbncmp(list *plist){//isbn기준으로 솔팅
    book *curBook = plist->bhead;//버블정렬을 하기위해 처음 위치를 지정
    while(check(curBook->next)){//마지막까지 정렬
        book *compare=curBook->next;//다음껏부터 비교하기 위해 선언
        book *tmp=curBook;//isbn가 큰걸 찾기위해 선언
        while(check(compare->next)){//다음 위치부터 끝까지 검사
            if(strcmp(compare->next->isbn,tmp->next->isbn)<0) tmp=compare;//isbn이 작을경우 정보를 바꾸기 위해 주소 저장
            compare=compare->next;//다음껄로 이동
        }

        if(check(tmp->next)&&tmp!=curBook){//tmp가 null을 가르키거나 움직이지 않았을경우를 제외하고 실행
            char tmpisbn[15];
            strcpy(tmpisbn,curBook->next->isbn);
            strcpy(curBook->next->isbn,tmp->next->isbn);
            strcpy(tmp->next->isbn,tmpisbn);//isbn 스왑

            char tmpbor=curBook->next->canbor;
            curBook->next->canbor=tmp->next->canbor;
            tmp->next->canbor=tmpbor;//canbor 스왑

            int tmpbknum=curBook->next->booknum;
            curBook->next->booknum=tmp->next->booknum;
            tmp->next->booknum=tmpbknum;//booknum 스왑

            char *tmpch;
            tmpch=tmp->next->bookname;
            tmp->next->bookname=curBook->next->bookname;
            curBook->next->bookname=tmpch;//bookname 스왑

            tmpch=tmp->next->publicier;
            tmp->next->publicier=curBook->next->publicier;
            curBook->next->publicier=tmpch;//publicier 스왑

            tmpch=tmp->next->writer;
            tmp->next->writer=curBook->next->writer;
            curBook->next->writer=tmpch;//writer 스왑

            tmpch=tmp->next->have;
            tmp->next->have=curBook->next->have;
            curBook->next->have=tmpch;//have 스왑
        }
        curBook=curBook->next;//다음껄로 이동
    }
}

void saveinfo(list *plist){//파일 저장 함수
    sortidcmp(plist);
    sortisbncmp(plist);
    FILE *idfp=fopen("client.txt","w");//client 파일을 수정하기위해 w로 열어 초기화


    client *ctmp=plist->admin;
    fprintf(idfp,"00000000|%s|%s|%s|%s\n",ctmp->pwd,ctmp->name,ctmp->address,ctmp->phonenum);//admin 정보 저장

    ctmp=plist->chead;
    while(check(ctmp->next))//메모리 상에서 관리중인 client정보 모두 저장
    {
      fprintf(idfp,"%s|%s|%s|%s|%s\n",ctmp->next->id,ctmp->next->pwd,ctmp->next->name,ctmp->next->address,ctmp->next->phonenum);
      ctmp=ctmp->next;
    }
    fclose(idfp);//정보 저장이 끝났으니 닫아줌

    FILE *bfp=fopen("book.txt","w");//book 파일을 수정하기위해 w로 열어 초기화
    book *btmp=plist->bhead;
    while(check(btmp->next))//메모리 상에서 관리중인 book정보 모두 저장
    {
      fprintf(bfp,"%d|%s|%s|%s|%s|%s|%c\n",btmp->next->booknum,btmp->next->bookname,btmp->next->publicier,btmp->next->writer,btmp->next->isbn,btmp->next->have,btmp->next->canbor);
      btmp=btmp->next;
    }
    fclose(bfp);//정보 저장이 끝났으니 닫아줌

    FILE *borfp=fopen("borrow.txt","w");//borrow 파일을 수정하기위해 w로 열어 초기화
    borrow *bortmp=plist->borhead;
    while(check(bortmp->next))//메모리 상에서 관리중인 borrow정보 모두 저장
    {
      fprintf(idfp,"%s|%d|%ld|%ld\n",bortmp->next->id,bortmp->next->booknum,bortmp->next->borday,bortmp->next->retday);
      bortmp=bortmp->next;
    }
    fclose(borfp);//정보 저장이 끝났으니 닫아줌
}

void menuadmin(list *plist){//관리자 메뉴 호출
    while(1)
    {
	    printf("\n>> 관리자 메뉴 <<\n");
	    printf("1. 도서 등록\t\t2. 도서 삭제\n");
	    printf("3. 도서 대여\t\t4. 도서 반납\n");
	    printf("5. 도서 검색\t\t6. 회원 목록\n");
	    printf("7. 로그아웃\t\t8. 프로그램 종료\n");
      int admenu=0;
      printf("선택할 기능을 입력하세요 : ");
      scanf("%d",&admenu);
      getchar();
      printf("==============================\n");
      switch(admenu)
      {
        case 1:
          addbook(plist);//도서 등록 함수 호출
          break;
        case 2:
          delbook(plist);//도서 삭제 함수 호출
          break;
        case 3:
          borbook(plist);//도서 대여 함수 호출
          break;
         case 4:
            retbook(plist);//도서 반납 함수 호출
            break;
        case 5:
            searchBookMenu(plist);//도서 검색 메뉴 함수 호출
            break;
        case 6:
            memberlistmenu(plist);//회원 목록 함수 호출
            break;
        case 7:
            return;//로그아웃
        case 8:
            end(plist);//프로그램 종료 함수 호출
            break;
      }
    }
}

void addbook(list *plist){//도서 등록 함수
    book *newbook=(book*)malloc(sizeof(book));//새로운 도서정보 입력받을 노드 생성
    newbook->next=plist->bhead->next;//새로운 노드 뒤에 헤드 다음 노드를 연결한 후 헤드 다음껄 새로운 노드로 변경
    plist->bhead->next=newbook;

    printf(">> 도서 등록 <<\n도서명: ");//등록할 도서명 입력
    gets(tmp);
    newbook->bookname=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->bookname,tmp);

    printf("출판사: ");//등록할 출판사명
    gets(tmp);
    newbook->publicier=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->publicier,tmp);

    printf("저자명: ");//등록할 저자명
    gets(tmp);
    newbook->writer=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->writer,tmp);

    printf("ISBN: ");//등록할 isbn
    gets(newbook->isbn);

    printf("소장처: ");//등록할 소장처
    gets(tmp);
    newbook->have=(char*)malloc(strlen(tmp)+1);
    strcpy(newbook->have,tmp);

    int last=0;
    book *btmp=plist->bhead;//마지막 booknum을 찾음
    while(check(btmp->next)){
        if(last<btmp->next->booknum){
            last=btmp->next->booknum;
        }
        btmp=btmp->next;
    }

    newbook->canbor='Y';//canbor을 y로 설정
    newbook->booknum=last+1;//마지막 booknum보다 1크게 저장

    printf("\n\n자동입력 사항\n\n");//자동입력 사항 출력
    printf("대여가능 여부 : %c\n",newbook->canbor);
    printf("도서번호: %d\n",newbook->booknum);


    saveinfo(plist);//정보가 변경되었으니 저장 함수 출력
}

void delbook(list *plist){//도서 삭제 함수 출력
    printf(">> 도서 삭제 <<\n1. 도서명 검색\t\t2. ISBN 검색\n==============================\n");
      int dmenu=0;
      printf("검색 번호를 입력하세요: ");
      scanf("%d",&dmenu);
      getchar();
      switch(dmenu)
      {
        case 1:
          delbookname(plist);//이름으로 삭제
          break;
        case 2:
          delisbn(plist);//isbn으로 삭제
          break;
      }

    saveinfo(plist);//정보 저장 실행
}

void delbookname(list *plist){//이름으로 책 삭제
    printf("도서명을 입력하세요: ");
    gets(tmp);
    printf("\n\n>> 검색 결과 <<\n");;
    printf("도서번호: ");
    book *btmp=plist->bhead;
    int cnt=0;
    while(check(btmp->next)){//이름과 일치하는 모든 책 검색
        if(strcmp(btmp->next->bookname,tmp)==0&&cnt==0){
            printf("%d(삭제 가능 여부 : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        else if(strcmp(btmp->next->bookname,tmp)==0){
            printf(", %d(삭제 가능 여부 : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        btmp=btmp->next;
    }

    if(cnt==0){//검색된 도서가 없을 경우 함수 종료
        printf("도서가 존재하지 않습니다.\n");
        return;
    }

    btmp=plist->bhead;

    while(check(btmp->next)){//책 정보를 출력하기 위해 이동시킴
        if(strcmp(btmp->next->bookname,tmp)==0) break;
        btmp=btmp->next;
    }

    printf("\n도서명: %s\n",btmp->next->bookname);//책 정보 출력
    printf("출판사: %s\n",btmp->next->publicier);
    printf("저자명: %s\n",btmp->next->writer);
    printf("ISBN: %s\n",btmp->next->isbn);
    printf("소장처: %s\n\n",btmp->next->have);

    printf("삭제할 도서의 번호를 입력하세요: ");//검색된 정보중 삭제할 booknum입력받음
    int delnum=0;
    scanf("%d",&delnum);
    getchar();

    btmp=plist->bhead;
    while(check(btmp->next)){
        if(btmp->next->booknum==delnum){
            if(btmp->next->canbor=='N'){//canbor이 n 일경우 삭제할 수 없음
                printf("이 도서는 삭제할 수 없습니다.\n");
                return;
            }
            else{//canbor이 y인 경우 노드를 삭제함
                printf("삭제 되었습니다.\n");
                book *tmpdel=btmp->next;//삭제할 노드를 free하기 위해 임시 저장
                btmp->next=btmp->next->next;
                free(tmpdel->bookname);//삭제할 노드의 정보 free
                tmpdel->bookname=NULL;
                free(tmpdel->publicier);
                tmpdel->publicier=NULL;
                free(tmpdel->writer);
                tmpdel->writer=NULL;
                free(tmpdel->have);
                tmpdel->have=NULL;
                free(tmpdel);
                tmpdel=NULL;
                saveinfo(plist);//정보 저장 함수 호출
                return;
            }
        }
        btmp=btmp->next;
    }

    printf("\n찾으시는 도서가 없습니다.\n");//삭제되지 않았음으로 검색된 도서가 없는것 따라서 안내문 출력후 함수 종료
}

void delisbn(list *plist){//isbn으로 삭제
    printf("ISBN을 입력하세요: ");
    char tmpisbn[15];
    gets(tmpisbn);
    printf("\n\n>> 검색 결과 <<\n");;
    printf("도서번호: ");
    book *btmp=plist->bhead;
    int cnt=0;
    while(check(btmp->next)){//isbn이 일치하는 모든 책 검색
        if(strcmp(btmp->next->isbn,tmpisbn)==0&&cnt==0){
            printf("%d(삭제 가능 여부 : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        else if(strcmp(btmp->next->isbn,tmp)==0){
            printf(", %d(삭제 가능 여부 : %c)",btmp->next->booknum,btmp->next->canbor);
            cnt++;
        }
        btmp=btmp->next;
    }

    if(cnt==0){//검색된 도서가 없을 경우 함수 종료
        printf("도서가 존재하지 않습니다.\n");
        return;
    }

    btmp=plist->bhead;

    while(check(btmp->next)){//책 정보를 출력하기 위해 이동시킴
        if(strcmp(btmp->next->isbn,tmpisbn)==0) break;
        btmp=btmp->next;
    }

    printf("\n도서명: %s\n",btmp->next->bookname);//책 정보 출력
    printf("출판사: %s\n",btmp->next->publicier);
    printf("저자명: %s\n",btmp->next->writer);
    printf("ISBN: %s\n",btmp->next->isbn);
    printf("소장처: %s\n\n",btmp->next->have);

    printf("삭제할 도서의 번호를 입력하세요: ");//검색된 정보중 삭제할 booknum입력받음
    int delnum=0;
    scanf("%d",&delnum);
    getchar();

    btmp=plist->bhead;
    while(check(btmp->next)){
        if(btmp->next->booknum==delnum){
            if(btmp->next->canbor=='N'){//canbor이 n 일경우 삭제할 수 없음
                printf("이 도서는 삭제할 수 없습니다.\n");
                return;
            }
            else{//canbor이 y인 경우 노드를 삭제함
                printf("삭제 되었습니다.\n");
                book *tmpdel=btmp->next;//삭제할 노드를 free하기 위해 임시 저장
                btmp->next=btmp->next->next;
                free(tmpdel->bookname);//삭제할 노드의 정보 free
                tmpdel->bookname=NULL;
                free(tmpdel->publicier);
                tmpdel->publicier=NULL;
                free(tmpdel->writer);
                tmpdel->writer=NULL;
                free(tmpdel->have);
                tmpdel->have=NULL;
                free(tmpdel);
                tmpdel=NULL;
                saveinfo(plist);//정보 저장 함수 호출
                return;
            }
        }
        btmp=btmp->next;
    }
    printf("\n찾으시는 도서가 없습니다.\n");//삭제되지 않았음으로 검색된 도서가 없는것 따라서 안내문 출력후 함수 종료
}

void addclient(list *libInfo)//회원 가입 함수
{
	client *newClient;//새로운 회원을 입력받을 노드 생성
	newClient=(client*)malloc(sizeof(client));

	printf("\n\n\n>>회원 가입<<\n");
	printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n\n");

    int cflag=0;//동일한 id가 존재하는지 확인하는 flag
    while(1) {
        cflag=0;
        printf("학번: ");
    	gets(tmp);
        client *ctmp=libInfo->chead;
        while(check(ctmp->next)){//id가 동일한 게 존재하는지 확인
            if(strcmp(ctmp->next->id,tmp)==0){//동일한게 존재하므로 반복문 flag변화 후 반복문 종료
                cflag++;
                break;
            }
            ctmp=ctmp->next;
        }
        if(cflag!=0){//id를 다시 입력시킴
            printf("다시 입력해 주세요.\n");
        }
        else break;
    }
	strcpy(newClient->id, tmp);

	printf("비밀번호: ");//pwd입력
	gets(tmp);
	newClient->pwd=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->pwd, tmp);

	printf("이름: ");//name입력
	gets(tmp);
	newClient->name=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->name, tmp);

	printf("주소: ");//addresss입력
	gets(tmp);
	newClient->address=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->address, tmp);

	printf("전화번호: ");//phonenum입력
	gets(tmp);
	newClient->phonenum=(char*)malloc(strlen(tmp)+1);
	strcpy(newClient->phonenum, tmp);

	newClient->next=libInfo->chead->next;//새로운 노드의 next를 헤드의 다음 노드로 연결
	libInfo->chead->next=newClient;//헤드의 next를 새로운 노드로 설정
	saveinfo(libInfo);//정보 저장 함수 호출

	printf("\n\n회원가입이 되셨습니다.\n\n\n");



}

client *searchClient(list *libinfo, char* ClientID)//clientid와 일치하는 client 반환
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

void login(list *libinfo)//로그인 함수
{
    char idcmp[9];
    char pwcmp[100];
    while(1){
        printf(">>로그인<<\n");
        printf("학번: ");
        gets(idcmp);
        printf("비밀번호: ");
        gets(pwcmp);
        if(strcmp(idcmp,"admin")==0){//admin일 경우 admin pwd확인 및 관리자메뉴 호출
            strcpy(idcmp,"00000000");
            if(strcmp(pwcmp,libinfo->admin->pwd)==0){
                menuadmin(libinfo);
                break;
            }
            else{//다시 id 입력
                printf("\n비밀번호가 맞지 않습니다.\n");
                continue;
            }
        }
        client *loginClient = searchClient(libinfo, idcmp);
        if(loginClient==NULL){//검색된 회원이 없을경우
            printf("가입되지 않은 회원입니다.\n");//안내문 출력후 다시 입력
        }
        else{//검색된 회원이 있을경우
                if(strcmp(loginClient->pwd, pwcmp)==0){//입력된 pwd가 맞는지 확인후 맞으면 회원메뉴 호출
                    clientMenu(libinfo, idcmp);
                    break;
                }
                else{
                    printf("\n비밀번호가 맞지 않습니다.\n");//비밀번호가 틀렸음으로 다시 반복
                }
        }
    }
}

void clientMenu(list *libinfo, char *clientID)//회원 메뉴 호출
{
    while(1){
        printf("\n>>회원 메뉴<<\n");
        printf("1. 도서 검색\t\t2. 내 대여 목록\n");
        printf("3. 개인정보 수정\t4. 회원 탈퇴\n");
        printf("5. 로그아웃\t\t6. 프로그램 종료\n");
        printf("\n번호를 선택하세요: ");
        int menu=0;
        scanf("%d",&menu);
        getchar();
        switch(menu)
        {
            case 1:
                searchBookMenu(libinfo);//책 검색 메뉴 함수 호출
                break;
            case 2:
                borBookList(libinfo, clientID);//책 대여 목록 함수 호출
                break;
            case 3:
                fixClient(libinfo, clientID);//회원 수정 함수 호출
                break;
            case 4:
                delClient(libinfo, clientID);//회원 삭제 함수 호출
                break;
            case 5:
                return;//로그아웃
            case 6:
                end(libinfo);//프로그램 종료 함수 호출
                return;
        }
    }
}

void searchBookMenu(list *libinfo)//책 검색 메뉴 함수
{
    while(1){
        printf("\n>>도서 검색<<\n");
        printf("1. 도서명 검색\t2. 출판사 검색\n");
        printf("3. ISBN 검색\t4. 저자명 검색\n");
        printf("5. 전체 검색\t6. 이전 메뉴\n");
        printf("\n번호를 선택하세요: ");
        int menu;
        scanf("%d",&menu);
        getchar();
        switch(menu)
        {
            case 1:
                bookNameSearch(libinfo);//도서명으로 검색
                break;
            case 2:
                bookPubSearch(libinfo);//출판사로 검색
                break;
            case 3:
                bookIsbnSearch(libinfo);//isbn으로 검색
                break;
            case 4:
                bookWritSearch(libinfo);//저자명으로 검색
                break;
            case 5:
                bookTotSearch(libinfo);//전체 출력
                break;
            case 6:
                return;//이전 메뉴
        }
    }
}

void bookPrint(book *curBook)//책 정보 출력
{
    printf("도서명: %s\n", curBook->bookname);
    printf("출판사: %s\n", curBook->publicier);
    printf("저자명: %s\n", curBook->writer);
    printf("ISBN: %s\n", curBook->isbn);
    printf("소장처: %s\n", curBook->have);
    printf("대여 가능 여부: %c", curBook->canbor);
}

void bookNameSearch(list *libinfo)//도서명으로 검색
{
    printf("\n도서명을 입력하세요: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//입력받은 도서명과 일치하는 도서 검색후 출력
        if(strcmp(curBook->next->bookname, tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=전체 책 권 수 cant=빌릴수 없는 책 권 수
    curBook=libinfo->bhead;
    while(check(curBook->next)){//대여 정보 출력
        if(strcmp(curBook->next->bookname, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint의 canbor정보 삭제
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0일 경우 책이 존재하지 않으므로 출력 안함
}

void bookPubSearch(list *libinfo)//출판사로 검색
{
    printf("\n출판사를 입력하세요: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//입력받은 출판사 일치하는 도서 검색후 출력
        if(strcmp(curBook->next->publicier, tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=전체 책 권 수 cant=빌릴수 없는 책 권 수
    curBook=libinfo->bhead;
    while(check(curBook->next)){//대여 정보 출력
        if(strcmp(curBook->next->publicier, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint의 canbor정보 삭제
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0일 경우 책이 존재하지 않으므로 출력 안함
}

void bookIsbnSearch(list *libinfo)//isbn으로 검색
{
    printf("\nISBN을 입력하세요: ");
    char a[15];
    scanf("%s", a);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//입력받은 isbn과 일치하는 도서 검색후 출력
        if(strcmp(curBook->next->isbn,a)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=전체 책 권 수 cant=빌릴수 없는 책 권 수
    curBook=libinfo->bhead;
    while(check(curBook->next)){//대여 정보 출력
        if(strcmp(curBook->next->isbn, a)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint의 canbor정보 삭제
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0일 경우 책이 존재하지 않으므로 출력 안함
}

void bookWritSearch(list *libinfo)//저자명으로 검색
{
    printf("\n저자명을 입력하세요: ");
    gets(tmp);
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//입력받은 저자명 일치하는 도서 검색후 출력
        if(strcmp(curBook->next->writer,tmp)==0){
            bookPrint(curBook->next);
            break;
        }
        curBook=curBook->next;
    }
    int tot=0,cant=0;//tot=전체 책 권 수 cant=빌릴수 없는 책 권 수
    curBook=libinfo->bhead;
    while(check(curBook->next)){//대여 정보 출력
        if(strcmp(curBook->next->writer, tmp)==0){
            if(curBook->next->canbor=='N') cant++;
            tot++;
        }
        curBook=curBook->next;
    }
    printf("\b");//bookprint의 canbor정보 삭제
    if(tot!=0) printf("(%d/%d)\n",cant,tot);//tot=0일 경우 책이 존재하지 않으므로 출력 안함
}

void bookTotSearch(list *libinfo)//책 정보 전체 출력
{
    book *curBook=libinfo->bhead;
    printf("\n");
    while(check(curBook->next)){//끝까지 출력
        bookPrint(curBook->next);
        printf("\n");
        curBook=curBook->next;
    }
}

char *borBookName(list *libinfo, int bNum)//빌린 책 이름
{
    book *curBook=libinfo->bhead;
    while(check(curBook->next)){//booknum으로 일치하는 booknum검색 후 일치하면 반환
        if(curBook->next->booknum==bNum){
            return curBook->next->bookname;
        }
        else    curBook=curBook->next;
    }
    return NULL;
}

void borBookList(list *libinfo, char *ClientID)//빌린 책 리스트
{
    printf("\n>>내 대여 목록<<\n");
    borrow *curBor=libinfo->borhead;
    while(check(curBor->next)){
        if(strcmp(curBor->next->id, ClientID)==0){//현재 회원 id와 일치하는지 검색
            printf("\n도서 번호: %d\n", curBor->next->booknum);//빌린 도서 번호
            printf("도서명: %s\n", borBookName(libinfo, curBor->next->booknum));//빌린 도서명
            const char *day[] = {"일","월","화","수","목","금","토"};
            struct tm* bor_time = localtime(&(curBor->next->borday));//빌린 날짜 출력
            printf("대여일자: %d년 %d월 %d일 %s요일\n",bor_time->tm_year+1900, bor_time->tm_mon+1, bor_time->tm_mday, day[bor_time->tm_wday]);
            struct tm* ret_time = localtime(&(curBor->next->retday));//반납 날짜 출력
            printf("반납일자: %d년 %d월 %d일 %s요일\n",ret_time->tm_year+1900, ret_time->tm_mon+1, ret_time->tm_mday, day[ret_time->tm_wday]);
        }
        curBor=curBor->next;
    }
}


void fixClient(list *libinfo, char *ClientID)//회원 정보 수정
{
    client *curClient = searchClient(libinfo, ClientID);//현재 회원 정보
    printf("학번: %s\n", curClient->id);//id 출력
    printf("이름: %s\n", curClient->name);//name 출력

    printf("비밀번호: ");//pwd 수정
    gets(tmp);
    curClient->pwd = (char*)realloc(curClient->pwd,sizeof(char)*strlen(tmp));
    strcpy(curClient->pwd, tmp);

    printf("주소: ");//address 수정
    gets(tmp);
    curClient->address = (char*)realloc(curClient->address,sizeof(char)*strlen(tmp));
    strcpy(curClient->address, tmp);

    printf("전화번호: ");//phonenum 수정
    gets(tmp);
    curClient->phonenum = (char*)realloc(curClient->phonenum,sizeof(char)*strlen(tmp));
    strcpy(curClient->phonenum, tmp);

    saveinfo(libinfo);//정보 저장 호출

    printf("\n\n회원정보가 수정되었습니다.\n\n\n");//수정 되었다는 안내문 출력
}

void delClient(list *libinfo, char *ClientID)//회원 삭제 함수
{
    client *curClient=libinfo->chead;
    client *preClient=NULL;
    borrow *bortmp=libinfo->borhead;
    while(check(curClient->next)){
        if(strcmp(curClient->next->id, ClientID)==0){
            while(check(bortmp->next)){
                if(strcmp(bortmp->next->id,curClient->next->id)==0){//빌린 도서가 있으므로 삭제 불가
                    printf("대여목록이 있으므로 삭제하실 수 없습니다.\n");
                    return;//함수 종료시킴
                }
                bortmp=bortmp->next;
            }
            preClient=curClient->next;//삭제할 노드의 위치 저장
            curClient->next=curClient->next->next;//노드 삭제
            free(preClient->pwd);//노드 정보들 삭제
            preClient->pwd=NULL;
            free(preClient->name);
            preClient->name=NULL;
            free(preClient->address);
            preClient->address=NULL;
            free(preClient->phonenum);
            preClient->phonenum=NULL;
            free(preClient);
            preClient=NULL;
            saveinfo(libinfo);//정보 저장 호출
            main();//삭제 됐을시 메인 메뉴로 이동
            return;
        }
        curClient=curClient->next;
    }
}
void borbook(list *plist) { // 도서 대여 UI를 출력해 주고 책 이름과 ISBN으로 책을 검색하는 함수로 넘어가게 해줌
	bool loop_end = false;
	while(!loop_end){
		printf(">> 도서 대여 <<\n");
		printf("1. 도서명 검색\t2. ISBN 검색\n");
	    int borbook_menu=0;
	    printf("검색 번호를 입력하세요: ");
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


bool can_bor(book *pre_ad, int i){			//도서 명으로 검색 해서 여러권 있는 책중 한권이라도 도서 가능일 경우 true 를 리턴해줌
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

bool can_bor_2(book *pre_ad, int size, int booknum){//내가 빌리려고 하는 도서번호의 책이 대출 가능이면 true 를 리턴해줌
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

bool search_by_book_name(list *plist) {//책 이름으로 도서 검색을 해줌 또한 해당 도서의 정보를 출력하고 학번을 입력 받아 borrow 파일에 필요한 정보를 보내줌
	int size;
	int i = 0;
	printf("도서명을 입력하세요: ");
    gets(tmp);
    printf("\n\n>> 검색 결과 <<\n");

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

	btmp = plist->bhead; // btmp 를 처음 위치로

    i = 0;
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->bookname,tmp);
        if(ptr != NULL){
		    printf("도서번호: %07d(대여 가능 여부 %c)",btmp->next->booknum, btmp->next->canbor);
	        pre_ad[i].next = btmp->next;
	        i++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }

    if (size == 0){
        printf("도서가 존재하지 않습니다.\n");
        return false;
    }

	printf("\n");
    printf("도서명: %s\n",pre_ad[0].next->bookname);
    printf("출판사: %s\n",pre_ad[0].next->publicier);
    printf("저자명: %s\n",pre_ad[0].next->writer);
    printf("ISBN: %s\n",pre_ad[0].next->isbn);
    printf("소장처: %s\n\n",pre_ad[0].next->have);

    if (!can_bor(pre_ad, size)) {
        printf("이 도서는 대여할 수 없습니다.\n");
        return false;
    }


    printf("\n학번을 입력하세요: ");
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
        printf("학번이 존재하지 않습니다.\n");
        return false;
    }

    printf("\n도서번호를 입력하세요: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

	if (!can_bor_2(pre_ad, size, tmp_book_no)) {
        printf("이 도서는 대여할 수 없습니다.\n");
        return false;
	}

    printf("\n이 도서를 대여합니까?: ");
    gets(tmp);


	if (!strcmp("Y", tmp) == 0) {
	    printf("도서 대여를 취소하였습니다.\n");
		return false;
	}

	btmp = plist->bhead; // btmp 를 처음 위치로,...
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

bool search_by_ISBN(list *plist) {//ISBN으로 도서 검색을 해줌 또한 해당 도서의 정보를 출력하고 학번을 입력 받아 borrow 파일에 필요한 정보를 보내줌
	int size;
	int i = 0;
    printf("ISBN을 입력하세요: ");
    gets(tmp);
    printf("\n\n>> 검색 결과 <<\n");

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

	btmp = plist->bhead; // btmp 를 처음 위치로

    i = 0;
    while(check(btmp->next)){
    	ptr = strstr(btmp->next->isbn,tmp);
        if(ptr != NULL){
		    printf("도서번호: %07d(대여 가능 여부 %c)",btmp->next->booknum, btmp->next->canbor);
	        pre_ad[i].next = btmp->next;
	        i++;
	        btmp=btmp->next;
        } else {
	        btmp=btmp->next;
        }
    }

    if (size == 0){
        printf("도서가 존재하지 않습니다.\n");
        return false;
    }

	printf("\n");
    printf("도서명: %s\n",pre_ad[0].next->bookname);
    printf("출판사: %s\n",pre_ad[0].next->publicier);
    printf("저자명: %s\n",pre_ad[0].next->writer);
    printf("ISBN: %s\n",pre_ad[0].next->isbn);
    printf("소장처: %s\n\n",pre_ad[0].next->have);

    if (!can_bor(pre_ad, i)) {
        printf("이 도서는 대여할 수 없습니다.\n");
        return false;
    }


    printf("\n학번을 입력하세요: ");
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
        printf("학번이 존재하지 않습니다.\n");
        return false;
    }

    printf("\n도서번호를 입력하세요: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

	if (!can_bor_2(pre_ad, size, tmp_book_no)) {
        printf("이 도서는 대여할 수 없습니다.\n");
        return false;
	}


    printf("\n이 도서를 대여합니까?: ");
    gets(tmp);

	if (!strcmp("Y", tmp) == 0) {
	    printf("도서 대여를 취소하였습니다.\n");
		return false;
	}

	btmp = plist->bhead; // btmp 를 처음 위치로
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


void insert_borrow_file(list *plist, char *cli_id, int book_num){//대여 하고자 하는 책 정보를 borrow 파일에 저장하는 역할
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
    printf("도서가 대여 되었습니다.\n");
}

void retbook(list *plist) { //학번을 입력 받으면 그 학생이 대여한 책 목록을 출력하고 반납일자, 대출일자를 출력해줌 해당 정보들을 want_ret로 보내줌
	book *bfp = plist->bhead->next;
	borrow *borfp = plist->borhead->next;

	struct tm* bor_time;
	struct tm* ret_time;
	const char *day[] = {"일", "월", "화", "수", "목", "금", "토"};

		printf("학번을 입력하세요: ");
		gets(tmp);

		while(check(borfp->next)){
//			printf("%s\n",borfp->id);

			if(strcmp(borfp->id, tmp) == 0){
				printf("도서번호:%d\n", borfp->booknum);
				while(check(bfp->next)){
					if(borfp->booknum == bfp->booknum){
						printf("도서명:%s\n", bfp->bookname);
						break;
					}
					bfp = bfp->next;
				}
				bor_time = localtime(&(borfp->borday));
				printf("대여일자: %d년 %d월 %d일 %s요일\n",
						bor_time->tm_year+1900, bor_time->tm_mon+1, bor_time->tm_mday, day[bor_time->tm_wday]
						);
				ret_time = localtime(&(borfp->retday));
				printf("반납일자: %d년 %d월 %d일 %s요일\n\n",
						ret_time->tm_year+1900, ret_time->tm_mon+1, ret_time->tm_mday, day[ret_time->tm_wday]
						);
			}

			borfp = borfp->next;
		}
		want_ret(plist, bfp, tmp);
		saveinfo(plist);
}

void want_ret(list *plist,book *bfp, char *cli_id){// 반납 하고자 하는 도서번호를 입력받고 해당도서의 반납 여부를 물어봄. Y를 입력 받으면 ruduce_borrow_file 함수 실행후 대여 가능 여부 Y
	printf("\n도서번호를 입력하세요: ");
    gets(tmp);

    int tmp_book_no = atoi(tmp);

    printf("\n이 도서를 반납처리 할까요?: ");
    gets(tmp);

	if (strcmp("Y", tmp) == 0) {
		bfp->canbor = 'Y';
		ruduce_borrow_file(plist, tmp_book_no, cli_id);

	}

}

void ruduce_borrow_file(list *plist, int tmp_book_no, char *cli_id){//반납 하고자 하는 도서를 borrow 파일에서 지우는 역할
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

void memberlistmenu(list *plist) { //6. 회원 목록 메뉴 호출
	printf("\n>> 회원 목록 <<\n1. 이름 검색\t2. 학번 검색\n3. 전체 검색\t4. 이전 메뉴\n");
    int listmenu=0;
    printf("선택할 기능을 입력하세요 : ");
    scanf("%d",&listmenu);
    getchar();
    printf("==============================\n");
    switch (listmenu) {
        case 1:
            srchbymemname(plist);//이름으로 회원 검색
            break;
        case 2:
            srchbyid(plist);//학번으로 회원 검색
            break;
        case 3:
            srchbyall(plist);//회원 정보 전체 출력
            break;
        case 4:
            return;//이전 메뉴
    }
}

void srchbymemname(list *plist){//이름으로 회원 검색
    printf("회원 이름 : ");
    gets(tmp);
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        if(strcmp(ctmp->next->name,tmp)==0){//입력한 회원 이름과 일치하는 회원 정보 출력
            printf("\n==============================\n");
            printf("\n학번 : %s",ctmp->next->id);
            printf("\n이름 : %s",ctmp->next->name);
            printf("\n주소 : %s",ctmp->next->address);
            printf("\n전화번호 : %s\n",ctmp->next->phonenum);
            printf("==============================\n");
        }
        ctmp=ctmp->next;
    }
}

void srchbyid(list *plist){//학번으로 회원 검색
    printf("학번 : ");
    gets(tmp);
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        if(strcmp(ctmp->next->id,tmp)==0){//입력한 회원 id와 일치하는 회원 정보 출력
            printf("\n==============================\n");
            printf("\n학번 : %s",ctmp->next->id);
            printf("\n이름 : %s",ctmp->next->name);
            printf("\n주소 : %s",ctmp->next->address);
            printf("\n전화번호 : %s\n",ctmp->next->phonenum);
            printf("==============================\n");
        }
        ctmp=ctmp->next;
    }
}

void srchbyall(list *plist){//모든 회원 정보 출력
    printf("\n");
    client *ctmp=plist->chead;
    while(check(ctmp->next)){
        printf("==============================\n");
        printf("\n학번 : %s",ctmp->next->id);
        printf("\n이름 : %s",ctmp->next->name);
        printf("\n주소 : %s",ctmp->next->address);
        printf("\n전화번호 : %s\n",ctmp->next->phonenum);
        printf("==============================\n");
        ctmp=ctmp->next;
    }
    printf("==============================\n");
}

void end(list *plist){//프로그램 종료 함수
    client *ctmp=plist->admin;//admin정보 메모리에서 삭제
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

    while(check(plist->chead->next)){//client정보 메모리에서 삭제
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
    free(plist->chead);//client head 삭제
    plist->chead=NULL;

    while(check(plist->bhead->next)){//book정보 메모리에서 삭제
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
    free(plist->bhead);//book head 삭제
    plist->bhead=NULL;

    while(check(plist->borhead->next)){//borrow정보 메모리에서 삭제
        borrow *bortmp=plist->borhead->next;
        plist->borhead->next=plist->borhead->next->next;
        free(bortmp);
        bortmp=NULL;
    }
    free(plist->borhead);//borrow head 삭제
    plist->borhead=NULL;

    exit(0);
}

//김찬진 : 최종 버그 수정 및 설명 및 addbook,delbook,reset,main,end,saveinfo,check,menuadmin,sortidcmp,sortisbncmp,delisbn,delbookname과 전체 틀 제작
//안성빈 : client.txt, book.txt, borrow.txt 파일 정보 저장
//이수형 : 버그체크 및 borbook,retbook,memberlistmenu,srchbymemname,srchbyid,srchbyall,menubooksearch,search_by_ISBN,search_by_book_name,insert_borrow_file,want_ret,ruduce_borrow_file,can_bor,can_bor_2 제작
//임가균 : addclient,searchClient,login,clientMenu,searchBookMenu,bookPrint,bookNameSearch,bookPubSearch,bookIsbnSearch,bookWritSearch,bookTotSearch,borBookName,borBookList,fixClient,delClient 제작
