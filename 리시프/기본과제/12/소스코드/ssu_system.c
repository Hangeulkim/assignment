#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {
    printf("before system()\n");
    system("pwd");//pwd�� ����
    printf("after system()\n");
    exit(0);
}
