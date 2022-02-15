#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
void ssu_nested_func(int loc_var, int loc_volatile, int loc_register);
static jmp_buf glob_buffer;
int main(void)
{
    register int loc_register;
    volatile int loc_volatile;
    int loc_var;

    loc_var = 10;
    loc_volatile = 11;
    loc_register = 12;
    if (setjmp(glob_buffer) != 0) {//glob_buffer을 env변수에 저장
        printf("after longjmp, loc_var = %d, loc_volatile = %d,loc_register = %d\n", loc_var, loc_volatile, loc_register);//longjmp이후의 변수들 출력
        exit(0);
    }
    loc_var = 80;
    loc_volatile = 81;
    loc_register = 83;
    ssu_nested_func(loc_var, loc_volatile, loc_register);//ssu_nested_func실행
    exit(0);
}
void ssu_nested_func(int loc_var, int loc_volatile, int loc_register) {
    printf("before longjmp, loc_var = %d, loc_volatile = %d,loc_register = %d\n", loc_var, loc_volatile, loc_register);//longjmp이전의 변수들을 출력
    longjmp(glob_buffer, 1);//longjmp실행
}
