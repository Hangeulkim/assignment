#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
void ssu_local_func(void);

int main(void)
{
    printf("Before vfork\n");
    ssu_local_func();
    printf("After ssu_local_func, my PID is %d\n", getpid());//함수를 실행한 후 pid를 가져와서 출력
    exit(0);
}
void ssu_local_func(void) {
    pid_t pid;
    if ((pid = vfork()) == 0)//vfork를 실행
        printf("I'm child. My PID is %d\n", getpid());//자식일 경우 자식의 pid출력
    else if (pid > 0)
        sleep(3);//부모일경우 잠시 딜레이
    else
        fprintf(stderr, "vfork error\n");//vfork 에러처리
}
