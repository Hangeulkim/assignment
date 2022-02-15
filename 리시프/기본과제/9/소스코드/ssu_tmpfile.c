#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void)
{
    char buf[BUFFER_SIZE];
    char name[L_tmpnam];
    FILE *fp;
    printf("Temporary filename <<%s>>\n", tmpnam(name));//name에 임시파일 이름을 저장하고 출력
    if ((fp = tmpfile()) == NULL) {//w+로 임시파일 생성
        fprintf(stderr, "tmpfile create error!!\n");
        exit(1);
    }
    fputs("create tmpfile success!!\n", fp);//fp에 스트링 입력
    rewind(fp);//fp의 처음으로 오프셋 이동
    fgets(buf, sizeof(buf), fp);//fp에서 한줄을 읽어와 buf에 저장
    puts(buf);//buf를 출력
    exit(0);
}
