#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main(void)
{
    char operator;
    FILE *fp;
    int character;
    int number = 0;
    if ((fp = fopen("ssu_expr.txt", "r")) == NULL) {//ssu_expr.txt를 r로 fopen
        fprintf(stderr, "fopen error for ssu_expr.txt\n");
        exit(1);
    }
    while (!feof(fp)) {//eof를 만나기 전까지
        while ((character = fgetc(fp)) != EOF && isdigit(character))
            number = 10 * number + character - 48;//fp에서 한글자를 읽어와 숫자인지 확인하고 10진수로 변환
        fprintf(stdout, " %d\n", number);//number 출력
        number = 0;
        if (character != EOF) {//eof가 아닐경우
            ungetc(character, fp);//character가 숫자를 아님을 확인하면서 한글자를 더 읽었으므로 다시 되돌려놓음
            operator = fgetc(fp);//숫자 다름엔 operator가 나오므로 operator저장
            printf("Operator => %c\n", operator);//오퍼레이터 출력
        }
    }
    fclose(fp);//파일 사용 종료
    exit(0);
}
