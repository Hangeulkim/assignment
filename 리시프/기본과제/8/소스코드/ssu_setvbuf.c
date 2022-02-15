#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);

int main(void)
{
    char buf[BUFFER_SIZE]; char *fname = "/dev/pts/0";
    FILE *fp;
    if ((fp = fopen(fname, "w")) == NULL) {//fname을 w로 fopen
        fprintf(stderr, "fopen error for %s", fname);
        exit(1);
    }
    ssu_setbuf(fp, buf);//buf를 버퍼로 사용
    fprintf(fp, "Hello, "); sleep(1);
    fprintf(fp, "UNIX!!"); sleep(1);
    fprintf(fp, "\n"); sleep(1);

    ssu_setbuf(fp, NULL);//버퍼를 사용하지 않음
    fprintf(fp, "HOW"); sleep(1);
    fprintf(fp, " ARE"); sleep(1);
    fprintf(fp, " YOU?"); sleep(1);
    fprintf(fp, "\n"); sleep(1);
    exit(0);
}

void ssu_setbuf(FILE *fp, char *buf) {
    size_t size; int fd; int mode;
    fd = fileno(fp);//파일의 에러
    if (isatty(fd))//fd가 터미널에 연결되어 있는지 확인
        mode = _IOLBF;//입출력을 라인 단위로 버퍼링을 이용
    else
        mode = _IOFBF;//입출력을 버퍼 단위로 이용
    if (buf == NULL) {
        mode = _IONBF;//입출력이 버퍼를 사용하지 않게 되며 buf와 size는 무시
        size = 0;
    }
    else
        size = BUFFER_SIZE;//버퍼사이즈 저장

    setvbuf(fp, buf, mode, size);//mode로 설정하고 buf를 size만큼의 버퍼로 사용
}
