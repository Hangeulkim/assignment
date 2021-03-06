#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include "flash.h"
#include"fdevicedriver.c"
// 필요한 경우 헤더파일을 추가한다

FILE *flashfp;	// fdevicedriver.c에서 사용

//
// 이 함수는 FTL의 역할 중 일부분을 수행하는데 물리적인 저장장치 flash memory에 Flash device driver를 이용하여 데이터를
// 읽고 쓰거나 블록을 소거하는 일을 한다 (동영상 강의를 참조).
// flash memory에 데이터를 읽고 쓰거나 소거하기 위해서 fdevicedriver.c에서 제공하는 인터페이스를
// 호출하면 된다. 이때 해당되는 인터페이스를 호출할 때 연산의 단위를 정확히 사용해야 한다.
// 읽기와 쓰기는 페이지 단위이며 소거는 블록 단위이다.
//
int main(int argc, char *argv[])
{
	char blockbuf[BLOCK_SIZE];
	char sectorbuf[SECTOR_SIZE];
	char sparebuf[SPARE_SIZE];
	char pagebuf[PAGE_SIZE];

	// flash memory 파일 생성: 위에서 선언한 flashfp를 사용하여 flash 파일을 생성한다. 그 이유는 fdevicedriver.c에서
	//                 flashfp 파일포인터를 extern으로 선언하여 사용하기 때문이다.
	if(!strcmp(argv[1],"c")){
		if(argc!=4){
			fprintf(stderr,"usage c option: %s c <flashfile> <#blocks>",argv[0]);
			exit(1);
		}
		flashfp=fopen(argv[2],"w");

		memset((void*)blockbuf, (char)0xFF, BLOCK_SIZE);

		int pages=atoi(argv[3]);

		while(pages--){
			fwrite((void *)blockbuf, BLOCK_SIZE, 1, flashfp);
		}
	}
	// 페이지 쓰기: pagebuf의 섹터와 스페어에 각각 입력된 데이터를 정확히 저장하고 난 후 해당 인터페이스를 호출한다
	else if(!strcmp(argv[1],"w")){
		if(argc!=6){
			fprintf(stderr,"usage w option: %s w <flashfile> <ppn> <sectordata> <sparedata>",argv[0]);
			exit(1);
		}

		int ppn=atoi(argv[3]);

		flashfp=fopen(argv[2],"r+");

		memset((void*)sectorbuf, (char)0xFF, SECTOR_SIZE);
		strcpy(sectorbuf,argv[4]);

		memset((void*)sparebuf, (char)0xFF, SPARE_SIZE);
		strcpy(sparebuf,argv[5]);

		memset((void*)pagebuf, (char)0xFF, PAGE_SIZE);
		strcpy(pagebuf,sectorbuf);
		strcpy(pagebuf+SECTOR_SIZE,sparebuf);

		dd_write(ppn,pagebuf);
	}
	// 페이지 읽기: pagebuf를 인자로 사용하여 해당 인터페이스를 호출하여 페이지를 읽어 온 후 여기서 섹터 데이터와
	//                  스페어 데이터를 분리해 낸다
	else if(!strcmp(argv[1],"r")){
		if(argc!=4){
			fprintf(stderr,"usage r option: %s r <flashfile> <ppn>",argv[0]);
			exit(1);
		}

		flashfp=fopen(argv[2],"r");
		int ppn=atoi(argv[3]);
		dd_read(ppn,pagebuf);

		for(int i=0;i<PAGE_SIZE;i++){
			if(i==SECTOR_SIZE)
				printf("\t");
			if(pagebuf[i]!=(char)0xFF)
				printf("%c",pagebuf[i]);
		}
		printf("\n");
	}
	// memset(), memcpy() 등의 함수를 이용하면 편리하다. 물론, 다른 방법으로 해결해도 무방하다.
	else if(!strcmp(argv[1],"e")){
		if(argc!=4){
			fprintf(stderr,"usage e option: %s r <flashfile> <ppn>",argv[0]);
			exit(1);
		}

		int ppn=atoi(argv[3]);
		flashfp=fopen(argv[2],"r+");

		dd_erase(ppn);
	}

	else{
		fprintf(stderr,"usage err %s\n",argv[0]);
		exit(1);
	}

	return 0;
}
