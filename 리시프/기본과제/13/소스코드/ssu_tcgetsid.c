#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <termios.h>
int main(void)
{
    pid_t sid_stderr;
    pid_t sid_stdin;
    pid_t sid_stdout;
    sid_stdin = tcgetsid(STDIN_FILENO);//STDIN_FILENO와 관련된 세션 리더의 프로세스 그룹 ID 저장
    if (sid_stdin == -1) {
        fprintf(stderr, "tcgetsid error\n");
        exit(1);
    }
    else
        printf("Session Leader for stdin: %d\n", sid_stdin);//들어온 입력값 출력

    sid_stdout = tcgetsid(STDOUT_FILENO);//STDIN_FILENO와 관련된 세션 리더의 프로세스 그룹 ID 저장

    if (sid_stdout == -1) {
        fprintf(stderr, "tcgetsid error\n");
        exit(1);
    }
    else
        printf("Session Leader for stdout: %d\n", sid_stdout);//STDIN_FILENO와 관련된 세션 리더의 프로세스 그룹 ID 저장
        
    sid_stderr = tcgetsid(STDERR_FILENO);//STDIN_FILENO와 관련된 세션 리더의 프로세스 그룹 ID 저장
    if (sid_stderr == -1) {
        fprintf(stderr, "tcgetsid error\n");
        exit(1);
    }
    else
        printf("Session Leader for stderr: %d\n", sid_stderr);//STDIN_FILENO와 관련된 세션 리더의 프로세스 그룹 ID 저장
    exit(0);
}
