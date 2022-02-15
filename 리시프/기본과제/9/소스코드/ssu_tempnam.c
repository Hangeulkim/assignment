#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    char *arg_directory = NULL;
    char *arg_prefix = NULL;
    if (argc != 3) {//인자가 제대로 주어졌는지 확인
        fprintf(stderr, "usage: %s <directory> <prefix>\n", argv[0]);
        exit(1);
    }
    arg_directory = argv[1][0] != ' ' ? argv[1] : NULL;//처음이 빈칸이 아니면 argv[1]로 설정
    arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL;//처음이 빈칸이아니면 argv[2]로 설정
    printf("created : %s\n", tempnam(arg_directory, arg_prefix));//arg_directory에 arg_prefix을 앞에 붙여서 임시파일을 생성
    exit(0);
}
