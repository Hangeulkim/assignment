#include<stdio.h>
#include<stdlib.h>

struct ssu_id{
    char name[64];
    int id;
};

int main(void)
{
    struct ssu_id test1,test2;
    char *fname="ssu_exam.dat";
    FILE *fp;

    if ((fp=fopen(fname,"w"))==NULL){//ssu_exam.dat를 w로 fopen하고 제대로 열렸는지 확인
        fprintf(stderr,"fopen error for %s\n",fname);
        exit(1);
    }

    printf("Input ID>> ");
    scanf("%d",&test1.id);
    printf("Input name>> ");
    scanf("%s",test1.name);//id와 name을 입력받아 test1에 저장

    if(fwrite(&test1, sizeof(struct ssu_id),1,fp)!=1){//test1의 데이터를 1개 fp에 씀
        fprintf(stderr,"fwrite error\n");
        exit(1);
    }

    fclose(fp);

    if((fp=fopen(fname,"r"))==NULL){//fname을 읽기로 fopen
        fprintf(stderr,"fopen error for %s\n",fname);
        exit(1);
    }

    if(fread(&test2,sizeof(struct ssu_id),1,fp)!=1){//fp에서 struct ssu_id만큼의 데이터를 읽어와 test2에 씀
        fprintf(stderr,"fread error\n");
        exit(1);
    }

    printf("\nID    name\n");
    printf(" ============\n");//읽어온 id와 name을 출력
    printf("%d      %s\n",test2.id,test2.name);
    fclose(fp);//fp의 사용을 종료
    exit(0);
}
