#include <stdio.h>
#include <stdlib.h>
#define BUFFER_MAX 256
int main(void)
{
    char command[BUFFER_MAX];
    char *prompt = "Prompt>>";
    while (1) {
        fputs(prompt, stdout);//prompt를 stdout에 출력
        if (fgets(command, sizeof(command), stdin) == NULL)//command에 stdin에 입력된 한줄을 저장하고 만약 끝이거나 에러일경우 탈출
            break;
        system(command);//커맨드 실행
    }
    fprintf(stdout, "Good bye...\n");//stdout에 good bye출력
    fflush(stdout);//stdout의 버퍼를 비움
    exit(0);
}
