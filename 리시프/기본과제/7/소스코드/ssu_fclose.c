#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    char *fname="ssu_test.txt";
    FILE *fp;

    if((fp=fopen(fname,"r"))==NULL){//fname을 r로 열어 fp에 저장
        fprintf(stderr,"fopen error for %s\n",fname);
        exit(1);
    }

    else{
        printf("Success!\n");
        printf("Opening \"%s\" in \"r\" mode!\n",fname);
    }

    fclose(fp);//열었던 파일을 닫음
    exit(0);
}
