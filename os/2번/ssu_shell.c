#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int pipefd[2];

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

int pps(char **cmd){

}

int ttop(char **cmd){

}

void run_code(char  **tokens){//코드를 실행
    int ttopflag[MAX_TOKEN_SIZE],ppsflag[MAX_TOKEN_SIZE];//ttop나 pps를 실행하는지에 대한 플래그
    char *cmd[MAX_NUM_TOKENS][MAX_TOKEN_SIZE];
    pid_t pid;
    memset(ttopflag,0,sizeof(ttopflag));
    memset(ppsflag,0,sizeof(ppsflag));

    int num=0,pipecmd=0,i=0;
    while(tokens[i]!=NULL){
        if(!strcmp(tokens[i],"|")){//파이프 처리
            num=0;
            pipecmd++;//파이프 이후의 명령어는 따로 저장
        }
        else{//명령어를 실행하기 위해 토큰들을 하나로 합침
            cmd[pipecmd][num]=(char *)malloc(sizeof(char)*MAX_TOKEN_SIZE);
            if(strcmp(tokens[i],"pps")==0)
                ppsflag[i]=1;
            if(strcmp(tokens[i],"ttop")==0)
                ttopflag[i]=1;
            strcpy(cmd[pipecmd][num],tokens[i]);
            num++;
        }
        i++;
    }


    if(pipecmd!=0){//파이프가 존재할 경우
        int fd_in=0;
        for(int i=0;i<=pipecmd;i++){
            pipe(pipefd);//파이프를 생성
            pid=fork();
            if(pid==-1){
                fprintf(stderr,"fork error\n");
                exit(1);
            }
            else if(pid==0){//자식이 프로그램을 실행
                dup2(fd_in, 0);//인풋 위치 fd_in으로 이동
                if(i!=pipecmd)
                    dup2(pipefd[1],1);//출력 fd 이동

                close(pipefd[0]);//파이프 입력쪽은 닫아놈
                if(ppsflag[i]){
                    if(pps(cmd[i])==-1){
                        fprintf(stderr,"SSUShell : Incorrect command\n");
                        exit(0);
                    }
                }
                else if(ttopflag[i]){
                    if(ttop(cmd[i])==-1){
                        fprintf(stderr,"SSUShell : Incorrect command\n");
                        exit(0);
                    }
                }
                else if(execvp(cmd[i][0],cmd[i])==-1){
                    fprintf(stderr,"SSUShell : Incorrect command\n");
                    exit(0);
                }
            }
            else{
                wait(NULL);
                close(pipefd[1]);//파이프 출력쪽은 닫아놈
                fd_in=pipefd[0];//인풋 위치를 파이프 입력으로 변경
            }
        }
    }
    else{//파이프가 없을 경우 단순 실행
        if(ppsflag[pipecmd]){
            if(pps(cmd[pipecmd])==-1){
                fprintf(stderr,"SSUShell : Incorrect command\n");
                exit(0);
            }
        }
        else if(ttopflag[pipecmd]){
            if(ttop(cmd[pipecmd])==-1){
                fprintf(stderr,"SSUShell : Incorrect command\n");
                exit(0);
            }
        }
        else if(execvp(cmd[pipecmd][0],cmd[pipecmd])==-1){
            fprintf(stderr,"SSUShell : Incorrect command\n");
            exit(0);
        }
    }
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

       //do whatever you want with the commands, here we just print them

        int pid=fork();
        if(pid==0)
            run_code(tokens);
        else
            wait(NULL);

		// Freeing the allocated memory
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
