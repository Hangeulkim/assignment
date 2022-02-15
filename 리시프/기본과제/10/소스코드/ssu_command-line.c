#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++)
        printf("argv[%d] : %s\n", i, argv[i]);//인자로 입력받은 값들을 출력
    exit(0);
}
