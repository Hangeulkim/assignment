#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "person.h"

#define FALSE 0
#define TRUE 1
//�ʿ��� ��� ��� ���ϰ� �Լ��� �߰��� �� ����

// ���� ������� �����ϴ� ����� ���� �ٸ� �� ������ �ణ�� ������ �Ӵϴ�.
// ���ڵ� ������ ������ ������ ���� �����Ǳ� ������ ����� ���α׷����� ���ڵ� ���Ϸκ��� �����͸� �а� �� ����
// ������ ������ ����մϴ�. ���� �Ʒ��� �� �Լ��� �ʿ��մϴ�.
// 1. readPage(): �־��� ������ ��ȣ�� ������ �����͸� ���α׷� ������ �о�ͼ� pagebuf�� �����Ѵ�
// 2. writePage(): ���α׷� ���� pagebuf�� �����͸� �־��� ������ ��ȣ�� �����Ѵ�
// ���ڵ� ���Ͽ��� ������ ���ڵ带 �аų� ���ο� ���ڵ带 ���ų� ���� ���ڵ带 ������ ����
// ��� I/O�� ���� �� �Լ��� ���� ȣ���ؾ� �մϴ�. �� ������ ������ �аų� ��� �մϴ�.
int RECORDS_IN_PAGE=PAGE_SIZE/RECORD_SIZE;
int all_page=1,all_record=PAGE_SIZE/RECORD_SIZE,page_num=-1,record_num=-1;

//
// ������ ��ȣ�� �ش��ϴ� �������� �־��� ������ ���ۿ� �о �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
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
// ���ο� ���ڵ带 ������ �� �͹̳ηκ��� �Է¹��� ������ Person ����ü�� ���� �����ϰ�, pack() �Լ��� ����Ͽ�
// ���ڵ� ���Ͽ� ������ ���ڵ� ���¸� recordbuf�� �����. �׷� �� �� ���ڵ带 ������ �������� readPage()�� ���� ���α׷� ��
// �о� �� �� pagebuf�� recordbuf�� ����Ǿ� �ִ� ���ڵ带 �����Ѵ�. �� ���� writePage() ȣ���Ͽ� pagebuf�� �ش� ������ ��ȣ��
// �����Ѵ�.
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
// �Ʒ��� unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� ���ڵ带 ����ü�� ��ȯ�� �� ����Ѵ�. �� �Լ��� ���� ȣ��Ǵ�����
// ������ ������ pack()�� �ó������� �����ϸ� �ȴ�.
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
// ���ο� ���ڵ带 �����ϴ� ����� �����ϸ�, �͹̳ηκ��� �Է¹��� �ʵ尪�� ����ü�� ������ �� �Ʒ��� insert() �Լ��� ȣ���Ѵ�.
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
// �ֹι�ȣ�� ��ġ�ϴ� ���ڵ带 ã�Ƽ� �����ϴ� ����� �����Ѵ�.
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
	FILE *fp;  // ���ڵ� ������ ���� ������

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
