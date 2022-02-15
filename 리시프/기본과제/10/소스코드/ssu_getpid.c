#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main(void)
{
    printf("Process ID = %d\n", getpid());
    printf("Parent process ID = %d\n", getppid());//부모 프로세스를 가져옴
    printf("Real user ID = %d\n", getuid());
    printf("Effective user ID = %d\n", geteuid());//유효 사용자 아이디를 가져옴
    printf("Real group ID = %d\n", getgid());
    printf("Effective group ID = %d\n", getegid());//유효 그룹 아이디를 가져옴
    exit(0);
}
