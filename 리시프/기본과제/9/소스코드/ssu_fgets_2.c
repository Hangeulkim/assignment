#include <stdio.h>
#include <stdlib.h>
#define BUFFER_MAX 256
int main(void)
{
    char command[BUFFER_MAX];
    char *prompt = "Prompt>>";
    while (1) {
        fputs(prompt, stdout);//prompt�� stdout�� ���
        if (fgets(command, sizeof(command), stdin) == NULL)//command�� stdin�� �Էµ� ������ �����ϰ� ���� ���̰ų� �����ϰ�� Ż��
            break;
        system(command);//Ŀ�ǵ� ����
    }
    fprintf(stdout, "Good bye...\n");//stdout�� good bye���
    fflush(stdout);//stdout�� ���۸� ���
    exit(0);
}
