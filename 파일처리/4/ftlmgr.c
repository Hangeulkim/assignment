// 주의사항
// 1. sectormap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. sectormap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(sectormap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include "sectormap.h"
#include "fdevicedriver.c"
// 필요한 경우 헤더 파일을 추가하시오.

#define ALL_PAGES (BLOCKS_PER_DEVICE*PAGES_PER_BLOCK)

typedef struct Map{
	int lpn;
	int ppn;
}map;

typedef struct Garbage_free{
	int gflag[4];
	int pbn;
	struct Garbage_free *next;
	struct Garbage_free *prev;
}garbage;

FILE *flashfp;
map mapping[DATAPAGES_PER_DEVICE];

garbage *root=NULL;
int free_pbn=BLOCKS_PER_DEVICE-1;
int using[ALL_PAGES];

void garbage_add(int psn);
int garbage_collect();
void ftl_open();
void ftl_read(int lsn, char *sectorbuf);
void ftl_write(int lsn, char *sectorbuf);
void ftl_print();

void garbage_add(int psn){
	garbage *cur;
	int fflag=FALSE;
	cur=root;

	while(cur->next!=NULL){
		cur=cur->next;
		if(cur->pbn==psn/4){
			fflag=TRUE;
			break;
		}
	}

	if(!fflag){
		cur->next=(garbage*)malloc(sizeof(garbage));
		cur->next->prev=cur;
		cur=cur->next;
		cur->next=NULL;
		cur->pbn=psn/4;
		for(int i=0;i<4;i++){
			cur->gflag[i]=FALSE;
		}
	}
	cur->gflag[psn%4]=TRUE;
}

int garbage_collect(){
	garbage *cur;
	char pagebuf[PAGE_SIZE];
	int return_value=free_pbn*4;

	cur=root;

	for(int i=return_value;i<return_value+4;i++){
		using[i]=FALSE;
	}

	if(cur->next==NULL){
		free_pbn=-1;
	}

	else{
		garbage *tmp;
		tmp=cur->next;
		cur->next=tmp->next;
		if(tmp->next!=NULL)
			tmp->next->prev=cur;

		for(int i=0;i<4;i++){
			if(!tmp->gflag[i]){
				dd_read(tmp->pbn*4+i,pagebuf);
				dd_write(free_pbn*4+i,pagebuf);
				using[free_pbn*4+i]=TRUE;
				for(int j=0;j<DATAPAGES_PER_DEVICE;j++){
					if(mapping[j].ppn==tmp->pbn*4+i){
						mapping[j].ppn=return_value+i;
						break;
					}
				}
			}
			else{
				using[free_pbn*4+i]=FALSE;
			}
		}
		dd_erase(free_pbn);
		free_pbn=tmp->pbn;
		free(tmp);
	}

	for(int i=return_value;i<return_value+4;i++){
		if(!using[i])
			return i;
	}
}

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다.
//
void ftl_open()
{
	//
	// address mapping table 초기화
	// free block's pbn 초기화
    	// address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일
	flashfp=fopen("flashmemory","w+");
	char blockbuf[BLOCK_SIZE];

	memset((void*)blockbuf, (char)0xFF, BLOCK_SIZE);

	for(int i=0;i<BLOCKS_PER_DEVICE;i++){
		fwrite((void *)blockbuf, BLOCK_SIZE, 1, flashfp);
	}

	for(int i=0;i<DATAPAGES_PER_DEVICE;i++){
		mapping[i].lpn=i;
		mapping[i].ppn=-1;
	}

	root=(garbage*)malloc(sizeof(garbage));
	root->next=NULL;
	root->prev=NULL;
	root->pbn=free_pbn;

	for(int i=0;i<DATAPAGES_PER_DEVICE;i++){
		using[i]=FALSE;
	}
	for(int i=DATAPAGES_PER_DEVICE;i<ALL_PAGES;i++){
		using[i]=TRUE;
	}
	fclose(flashfp);

	return;
}

//
// 이 함수를 호출하기 전에 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 한다.
// 즉, 이 함수에서 메모리를 할당받으면 안된다.
//
void ftl_read(int lsn, char *sectorbuf)
{
	flashfp=fopen("flashmemory","r+");
	char pagebuf[PAGE_SIZE];
	dd_read(mapping[lsn].ppn,pagebuf);
	memcpy(sectorbuf,pagebuf,SECTOR_SIZE);
	fclose(flashfp);
	return;
}


void ftl_write(int lsn, char *sectorbuf)
{
	flashfp=fopen("flashmemory","r+");
	char pagebuf[PAGE_SIZE];
	SpareData spare;
	memset((void*)pagebuf, (char)0xFF, PAGE_SIZE);
	memset((void*)spare.dummy, (char)0xFF, SPARE_SIZE-8);
	int can_ppn=-1;

	if(mapping[lsn].ppn!=-1){
		if(free_pbn==-1){
			printf("can't overwrite %d!\n",lsn);
			return;
		}
		garbage_add(mapping[lsn].ppn);
	}

	for(int i=0;i<ALL_PAGES;i++){
		if(using[i]==FALSE){
			can_ppn=i;
			break;
		}
	}

	if(can_ppn==-1){
		can_ppn=garbage_collect();
	}
	spare.lpn=lsn;
	spare.is_invalid=TRUE;

	using[can_ppn]=TRUE;
	memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
	memcpy(pagebuf+SECTOR_SIZE,&spare,SPARE_SIZE);
	dd_write(can_ppn,pagebuf);
	mapping[lsn].ppn=can_ppn;

	fclose(flashfp);
	return;
}

void ftl_print()
{
	printf("lpn ppn\n");
	for(int i=0;i<DATAPAGES_PER_DEVICE;i++){
		printf("%-3d %-3d\n",mapping[i].lpn,mapping[i].ppn);
	}
	printf("free block's pbn=%d\n",free_pbn);
	return;
}
