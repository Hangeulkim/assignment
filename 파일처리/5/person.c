#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "person.h"

#define FALSE 0
#define TRUE 1
//필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 모든 I/O는 위의 두 함수를 먼저 호출해야 합니다. 즉 페이지 단위로 읽거나 써야 합니다.
int RECORDS_IN_PAGE=PAGE_SIZE/RECORD_SIZE;
int all_page=1,all_record=PAGE_SIZE/RECORD_SIZE,page_num=-1,record_num=-1;

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	fread((void *)pagebuf, PAGE_SIZE, 1, fp);
}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 위치에 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	fwrite((void *)pagebuf, PAGE_SIZE, 1, fp);
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고, pack() 함수를 사용하여
// 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다. 그런 후 이 레코드를 저장할 페이지를 readPage()를 통해 프로그램 상에
// 읽어 온 후 pagebuf에 recordbuf에 저장되어 있는 레코드를 저장한다. 그 다음 writePage() 호출하여 pagebuf를 해당 페이지 번호에
// 저장한다.
//
void pack(char *recordbuf, const Person *p)
{
	int j=0;
	memset(recordbuf,(char)0xff,RECORD_SIZE);
	strcpy(recordbuf,p->sn);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->name);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->age);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->addr);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->phone);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->email);
	strcat(recordbuf,"#");
}

//
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다. 이 함수가 언제 호출되는지는
// 위에서 설명한 pack()의 시나리오를 참조하면 된다.
//
void unpack(const char *recordbuf, Person *p)
{
	int i=0;
	int j=0;
	for(j=0;recordbuf[i]!='#';j++,i++){
		p->sn[j]=recordbuf[i];
	}
	p->sn[j]='\0';
	for(j=0,i++;recordbuf[i]!='#';j++,i++){
		p->name[j]=recordbuf[i];
	}
	p->name[j]='\0';
	for(j=0,i++;recordbuf[i]!='#';j++,i++){
		p->age[j]=recordbuf[i];
	}
	p->age[j]='\0';
	for(j=0,i++;recordbuf[i]!='#';j++,i++){
		p->addr[j]=recordbuf[i];
	}
	p->addr[j]='\0';
	for(j=0,i++;recordbuf[i]!='#';j++,i++){
		p->phone[j]=recordbuf[i];
	}
	p->phone[j]='\0';
	for(j=0,i++;recordbuf[i]!='#';j++,i++){
		p->email[j]=recordbuf[i];
	}
	p->email[j]='\0';
}

//
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값을 구조체에 저장한 후 아래의 insert() 함수를 호출한다.
//
void insert(FILE *fp, const Person *p)
{
	char pagebuf[PAGE_SIZE];
	char recordbuf[RECORD_SIZE];
	pack(recordbuf,p);
	if(page_num==-1&&record_num==-1){
		memset(pagebuf,(char)0xff,PAGE_SIZE);
		if(all_page*(RECORDS_IN_PAGE)==all_record){
			all_page+=1;
			memcpy(pagebuf,recordbuf,RECORD_SIZE);
		}
		else{
			readPage(fp,pagebuf,all_page-1);
			memcpy(pagebuf+RECORD_SIZE*(all_record%RECORDS_IN_PAGE),recordbuf,RECORD_SIZE);
		}
		all_record+=1;
		writePage(fp,pagebuf,all_page-1);
	}
	else{
		char tmp[RECORD_SIZE];
		readPage(fp,pagebuf,page_num);
		memcpy(tmp,pagebuf+RECORD_SIZE*record_num,RECORD_SIZE);
		memcpy(pagebuf+RECORD_SIZE*record_num,recordbuf,RECORD_SIZE);
		writePage(fp,pagebuf,page_num);
		memcpy(&page_num,tmp+1,sizeof(int));
		memcpy(&record_num,tmp+5,sizeof(int));
	}
}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *sn)
{
	char pagebuf[PAGE_SIZE];
	char recordbuf[RECORD_SIZE];
	Person per;
	int fflag=FALSE;
	int i=1;
	int j=0;
	for(i=1;i<all_page;i++){
		readPage(fp,pagebuf,i);
		for(j=0;j<RECORDS_IN_PAGE;j++){
			memcpy(recordbuf,pagebuf+j*RECORD_SIZE,RECORD_SIZE);
			if(recordbuf[0]==0||recordbuf[0]=='*')
				continue;
			unpack(recordbuf,&per);
			if(!strcmp(per.sn,sn)){
				fflag=TRUE;
				break;
			}
		}
		if(fflag)
			break;
	}

	if(!fflag)
		return;

	memcpy(recordbuf,"*",1);
	memcpy(recordbuf+1,&page_num,sizeof(int));
	memcpy(recordbuf+5,&record_num,sizeof(int));
	memcpy(pagebuf+RECORD_SIZE*j,recordbuf,RECORD_SIZE);
	writePage(fp,pagebuf,i);
	page_num=i;
	record_num=j;
}

void readHeader(FILE *fp){
	char pagebuf[PAGE_SIZE];

	readPage(fp,pagebuf,0);

	memcpy(&all_page,pagebuf,sizeof(int));
	memcpy(&all_record,pagebuf+4,sizeof(int));
	memcpy(&page_num,pagebuf+8,sizeof(int));
	memcpy(&record_num,pagebuf+12,sizeof(int));
}

void writeHeader(FILE *fp){
	char pagebuf[PAGE_SIZE];
	memset(pagebuf,(char)0xff,PAGE_SIZE);

	memcpy(pagebuf,&all_page,sizeof(int));
	memcpy(pagebuf+4,&all_record,sizeof(int));
	memcpy(pagebuf+8,&page_num,sizeof(int));
	memcpy(pagebuf+12,&record_num,sizeof(int));

	writePage(fp,pagebuf,0);
}

int main(int argc, char *argv[])
{
	FILE *fp;  // 레코드 파일의 파일 포인터

	if(access(argv[2],F_OK)!=0){
		fp=fopen(argv[2],"w+");
		writeHeader(fp);
		fclose(fp);
	}
	fp=fopen(argv[2],"r+");
	readHeader(fp);

	if(!strcmp(argv[1],"i")){
		Person per;
		memset(&per,0,sizeof(Person));
		strcpy(per.sn,argv[3]);
		strcpy(per.name,argv[4]);
		strcpy(per.age,argv[5]);
		strcpy(per.addr,argv[6]);
		strcpy(per.phone,argv[7]);
		strcpy(per.email,argv[8]);
		insert(fp,&per);
	}
	else if(!strcmp(argv[1],"d")){
		delete(fp,argv[3]);
	}
	else{
		fprintf(stderr,"usage err!\n");
	}
	writeHeader(fp);

	fclose(fp);

	return 1;
}
