#include"ftlmgr.c"

int main()
{
    char sectorbuf[SECTOR_SIZE];
    memset((void*)sectorbuf, (char)0xFF, SECTOR_SIZE);
    ftl_open();
	while(1){
		printf("what do you want?(w,r,p) ");
		char c;
		int lsn;
		scanf("%c",&c);

		if(c=='w'){
			scanf(" %d %s",&lsn,sectorbuf);
			getchar();
            ftl_write(lsn,sectorbuf);
		}
		else if(c=='r'){
			scanf(" %d",&lsn);
			getchar();
            ftl_read(lsn,sectorbuf);
            for(int i=0;i<SECTOR_SIZE;i++){
                if(sectorbuf[i]!=(char)0xFF)
                    printf("%c",sectorbuf[i]);
            }
            printf("\n");
		}
		else if(c=='p'){
			getchar();
			ftl_print();
		}
	}
}
