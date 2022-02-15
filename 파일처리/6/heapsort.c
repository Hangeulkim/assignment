#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include "person.h"
//필요한 경우 헤더 파일과 함수를 추가할 수 있음
#define FALSE 0
#define TRUE 1
// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓸 때나
// 모든 I/O는 위의 두 함수를 먼저 호출해야 합니다. 즉, 페이지 단위로 읽거나 써야 합니다.
int all_page=1,all_record=PAGE_SIZE/RECORD_SIZE,page_num=-1,record_num=-1;
int cnt=0;
//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void heapsort(char **heaparray,int tmp);
void buildHeap(FILE *inputfp, char **heaparray);
void makeSortedFile(FILE *outputfp, char **heaparray);

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
// 주어진 레코드 파일에서 레코드를 읽어 heap을 만들어 나간다. Heap은 배열을 이용하여 저장되며,
// heap의 생성은 Chap9에서 제시한 알고리즘을 따른다. 레코드를 읽을 때 페이지 단위를 사용한다는 것에 주의해야 한다.
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

void buildHeap(FILE *inputfp, char **heaparray)
{
	char pagebuf[PAGE_SIZE];

	for(int i=1;i<all_page;i++){
		memset(pagebuf,'\0',PAGE_SIZE);
		readPage(inputfp,pagebuf,i);
		for(int j=0;j<PAGE_SIZE/RECORD_SIZE&&cnt<all_record;j++){
			char recordbuf[RECORD_SIZE];
			memcpy(recordbuf,pagebuf+j*RECORD_SIZE,RECORD_SIZE);
			memcpy(heaparray[++cnt],recordbuf,RECORD_SIZE);
			heapsort(heaparray,cnt);
		}
	}
}

void heapsort(char **heaparray,int tmp){
	Person par;
	Person now;
	char tmprec[RECORD_SIZE];
	unpack(heaparray[tmp],&now);

	while(1){
		if(tmp<=1)
			break;
		unpack(heaparray[tmp/2],&par);

		if(strcmp(now.sn,par.sn)<0){
			memcpy(tmprec,heaparray[tmp/2],RECORD_SIZE);
			memcpy(heaparray[tmp/2],heaparray[tmp],RECORD_SIZE);
			memcpy(heaparray[tmp],tmprec,RECORD_SIZE);
			tmp/=2;
		}
		else{
			break;
		}
	}
}
//
// 완성한 heap을 이용하여 주민번호를 기준으로 오름차순으로 레코드를 정렬하여 새로운 레코드 파일에 저장한다.
// Heap을 이용한 정렬은 Chap9에서 제시한 알고리즘을 이용한다.
// 레코드를 순서대로 저장할 때도 페이지 단위를 사용한다.
//
void makeSortedFile(FILE *outputfp, char **heaparray)
{
	char pagebuf[PAGE_SIZE];
	int tmpcnt=0;

	for(int i=1;i<=all_page;i++){
		memset(pagebuf,(char)0xff,PAGE_SIZE);
		for(int j=0;j<PAGE_SIZE/RECORD_SIZE&&tmpcnt<all_record;j++){
			memcpy(pagebuf+j*RECORD_SIZE,heaparray[++tmpcnt],RECORD_SIZE);
		}
		writePage(outputfp,pagebuf,i);
	}

}

void readHeader(FILE *fp){
	char pagebuf[PAGE_SIZE];

	readPage(fp,pagebuf,0);

	memcpy(&all_page,pagebuf,sizeof(int));
	memcpy(&all_record,pagebuf+4,sizeof(int));
	all_record-=2;
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
	FILE *inputfp;	// 입력 레코드 파일의 파일 포인터
	FILE *outputfp;	// 정렬된 레코드 파일의 파일 포인터
	char **heaparray;

	if(argc!=4){
		fprintf(stderr,"Usage : %s s <input file> <output file>\n",argv[0]);
		exit(1);
	}

	inputfp=fopen(argv[2],"r+");
	outputfp=fopen(argv[3],"w+");

	if(inputfp==NULL){
		fprintf(stderr,"fopen error\n");
		exit(1);
	}

	readHeader(inputfp);
	heaparray=(char**)malloc(sizeof(char)*(all_record+2));
	for(int i=0;i<=all_record;i++){
		heaparray[i]=(char*)malloc(sizeof(char)*(RECORD_SIZE+1));
		memset(heaparray[i],'\0',RECORD_SIZE+1);
	}
	writeHeader(outputfp);
	buildHeap(inputfp,heaparray);
	makeSortedFile(outputfp,heaparray);
	fclose(outputfp);

	return 1;
}
