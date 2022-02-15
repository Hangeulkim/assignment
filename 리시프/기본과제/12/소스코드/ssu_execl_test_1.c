#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    extern char **environ;
    char **str;
    int i;
    for (i = 0;i < argc;i++)
        printf("argv[%d]: %s\n",i, argv[i]);//들어온 인자들을 출력
    for (str = environ; *str != 0; str++)
        printf("%s\n", *str);//환경변수를 출력
    exit(0);
}
