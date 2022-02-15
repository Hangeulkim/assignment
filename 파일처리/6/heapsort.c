#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include "person.h"
//�ʿ��� ��� ��� ���ϰ� �Լ��� �߰��� �� ����
#define FALSE 0
#define TRUE 1
// ���� ������� �����ϴ� ����� ���� �ٸ� �� ������ �ణ�� ������ �Ӵϴ�.
// ���ڵ� ������ ������ ������ ���� �����Ǳ� ������ ����� ���α׷����� ���ڵ� ���Ϸκ��� �����͸� �а� �� ����
// ������ ������ ����մϴ�. ���� �Ʒ��� �� �Լ��� �ʿ��մϴ�.
// 1. readPage(): �־��� ������ ��ȣ�� ������ �����͸� ���α׷� ������ �о�ͼ� pagebuf�� �����Ѵ�
// 2. writePage(): ���α׷� ���� pagebuf�� �����͸� �־��� ������ ��ȣ�� �����Ѵ�
// ���ڵ� ���Ͽ��� ������ ���ڵ带 �аų� ���ο� ���ڵ带 �� ����
// ��� I/O�� ���� �� �Լ��� ���� ȣ���ؾ� �մϴ�. ��, ������ ������ �аų� ��� �մϴ�.
int all_page=1,all_record=PAGE_SIZE/RECORD_SIZE,page_num=-1,record_num=-1;
int cnt=0;
//
// ������ ��ȣ�� �ش��ϴ� �������� �־��� ������ ���ۿ� �о �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
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
// ������ ������ �����͸� �־��� ������ ��ȣ�� �ش��ϴ� ��ġ�� �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	fwrite((void *)pagebuf, PAGE_SIZE, 1, fp);
}

//
// �־��� ���ڵ� ���Ͽ��� ���ڵ带 �о� heap�� ����� ������. Heap�� �迭�� �̿��Ͽ� ����Ǹ�,
// heap�� ������ Chap9���� ������ �˰����� ������. ���ڵ带 ���� �� ������ ������ ����Ѵٴ� �Ϳ� �����ؾ� �Ѵ�.
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
// �ϼ��� heap�� �̿��Ͽ� �ֹι�ȣ�� �������� ������������ ���ڵ带 �����Ͽ� ���ο� ���ڵ� ���Ͽ� �����Ѵ�.
// Heap�� �̿��� ������ Chap9���� ������ �˰����� �̿��Ѵ�.
// ���ڵ带 ������� ������ ���� ������ ������ ����Ѵ�.
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
	FILE *inputfp;	// �Է� ���ڵ� ������ ���� ������
	FILE *outputfp;	// ���ĵ� ���ڵ� ������ ���� ������
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
