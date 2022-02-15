#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 4096
int main(void) {
    char buf[MAX_LINE];
    char name[L_tmpnam];
    FILE *fp;
    printf("temp file 1 : %s\n", tmpnam(NULL));//내부 버퍼를 이용해 그 포인터를 리턴
    tmpnam(name);//name에 경로에 해당하는 임시 파일 이름을 저장
    printf("temp file 2 : %s\n", name);
    if ((fp = tmpfile()) == NULL) {//w+모드로 임시 파일을 생성
        fprintf(stderr, "tmpfile error\n");
        exit(1);
    }
    fputs("tmpfile created temporary file.\n", fp);//임시 파일에 스트링 입력
    fseek(fp, 0, SEEK_SET);//fp의 오프셋을 처음으로 이동
    if (fgets(buf, sizeof(buf), fp) == NULL) {//fp에서 한줄을 읽어와 buf에 저장
        fprintf(stderr, "fgets error\n");
        exit(1);
    }
    fputs(buf, stdout);//buf를 stdout으로 출력
    exit(0);
}
