#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/time.h>
#include<time.h>
#include"ssu_crontab.h"

FILE *fp;
int start[10]={0,0,1,1,0};
int end[10]={59,23,31,12,6};

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

struct timeval begin_t, end_t;//시작 시간과 종료시간을 기록

int main()
{
    char input[BUF];
    char tmp[BUF];
    gettimeofday(&begin_t, NULL);//시작시각 저장
    while(1){
        fp=fopen("ssu_crontab_file","r+");
        if(fp==NULL){
            fp=fopen("ssu_crontab_file","w+");
        }
        for(int i=0;!feof(fp);i++){
            memset(tmp,'\0',BUF);
            fgets(tmp,BUF,fp);
            if(tmp[0]=='\0')
                break;
            printf("%d. %s",i,tmp);
        }
        fclose(fp);//파일에 있는 데이터를 출력
        printf("20182602> ");

        scanf("%[^\n]",input);
        getchar();
        strcpy(tmp,input);

        char *ptr=strtok(tmp," ");
        if(!strcmp(ptr,"add")){//add모드로 이동
            if(!errchk(input)){
                fprintf(stderr, "error occur\n");
                continue;
            }
            add(input);
        }
        else if(!strcmp(ptr,"remove")){//remove 모드로 이동
            del(atoi(strtok(NULL," ")));
        }
        else if(!strcmp(ptr,"exit")){//exit 모드로 이동
            exit_function();
        }
    }
}

int errchk(char *str){
    char tmp[BUF];
    char digit[BUF];
    int sFlag=FALSE;
    int a,range,cnt;

    strcpy(tmp,str);
    char *ptr=strtok(tmp," ");

    for(int i=0;i<5;i++){//분, 시, 일, 월, 요일순으로 범위 내에 있는지, - , /의 경우 앞뒤에 제대로 인자가 들어왔는지 체크
        ptr=strtok(NULL," ");
        a=-1;
        range=-1;
        cnt=-1;
        for(int j=0;j<strlen(ptr);j++){
            if(ptr[j]=='-'){
                range=atoi(&ptr[j+1]);
                if(j==0||(a<start[i]||a>end[i])||(range<start[i]||range>end[i])||ptr[j+1]=='\0')//특수 문자일경우 그 이전과 그 이후가 숫자인지 따짐
                    return FALSE;
                if(range<a){
                    int tmp=a;
                    a=range;
                    range=tmp;
                }
                sprintf(digit,"%d",range);
                j+=strlen(digit);
            }
            else if(ptr[j]==','){
                a=-1;
                range=-1;
                cnt=-1;
                sFlag=FALSE;
                if(ptr[j+1]=='\0'){
                    return FALSE;
                }
            }
            else if(ptr[j]=='/'){
                cnt=atoi(&ptr[j+1]);
                if(j==0||(a<start[i]||a>end[i])||(range<start[i]||range>end[i])||sFlag==TRUE||(range-a+1<cnt)||ptr[j+1]=='\0'||ptr[j+1]=='*')//특수 문자일경우 그 이전과 그 이후가 숫자인지 따짐
                    return FALSE;
                sFlag=TRUE;
                sprintf(digit,"%d",cnt);
                j+=strlen(digit);
            }
            else if(ptr[j]>='0'&&ptr[j]<='9'){//숫자가 범위내부에 있는지 따짐
                a=atoi(&ptr[j]);
                range=a;
                if(a<start[i]||a>end[i])
                    return FALSE;
                sprintf(digit,"%d",a);
                j+=strlen(digit)-1;
            }
            else if(ptr[j]=='*'){//*인데 뒤에 숫자가 나오거나 하는경우를 따짐
                if(j==0&&(ptr[j+1]!='/'&&ptr[j+1]!='\0'))
                    return FALSE;
                else if(j>0&&((ptr[j+1]!='/'&&ptr[j+1]!='\0')||(ptr[j-1]!='\0'&&ptr[j-1]!=',')))
                    return FALSE;
                a=start[i];
                range=end[i];
            }
            else{
                return FALSE;
            }
        }
    }
    return TRUE;
}

void add(char *str){
    char tmp[BUF];
    time_t t;
    FILE *logfp;

    fp=fopen("ssu_crontab_file","a+");
    fprintf(fp,"%s\n",str+4);//errchk에서 맞는지 체크했으므로 명령어를 추가시킴
    fclose(fp);

    logfp=fopen("ssu_crontab_log","a");
    t=time(NULL);
    strcpy(tmp,ctime(&t));
    tmp[strlen(tmp)-1]='\0';

    fprintf(logfp,"[%s] %s\n",tmp,str);
    fclose(logfp);//add log기록
}

void del(int num){
    FILE *tmpfp, *logfp;
    time_t t;
    char tmp[BUF], time_str[BUF];
    int i;

    tmpfp=fopen("ssu_tmp","w+");//파일 내용을 저장할 임시 데이터 파일
    fp=fopen("ssu_crontab_file","r+");

    for(i=0;!feof(fp);i++){
        memset(tmp,'\0',BUF);
        fgets(tmp,BUF,fp);
        if(tmp[0]=='\0')
            break;
        if(i==num){
            logfp=fopen("ssu_crontab_log","a");
            t=time(NULL);
            strcpy(time_str,ctime(&t));
            time_str[strlen(time_str)-1]='\0';

            fprintf(logfp,"[%s] remove %s",time_str,tmp);
            fclose(logfp);//remove log기록
            continue;
        }
        fprintf(tmpfp,"%s",tmp);
    }
    if(i-1<num){
        fprintf(stderr, "%d num is bigger than line number\n",i);
    }

    fclose(fp);
    fclose(tmpfp);
    remove("ssu_crontab_file");//기존의 파일을 삭제시킴
    rename("ssu_tmp","ssu_crontab_file");//임시 파일로 원본을 바꿈
}

void exit_function(){
    gettimeofday(&end_t, NULL);//끝난시각 저장
    ssu_runtime(&begin_t, &end_t);//걸린시간 구하는 함수 실행
    exit(0);
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{//실행시간 체크 함수
	end_t->tv_sec -= begin_t->tv_sec;//끝난 시각에서 시작시각의 초를 뺌

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}//usec가 시작지점이 큰경우 end의 1초를 usec로 변환

	end_t->tv_usec -= begin_t->tv_usec;//끝난 시각에서 시작시각의 usec를 뺌
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);//걸린 시각을 출력
}
