#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "blank.h"

char datatype[DATATYPE_SIZE][MINLEN] = {"int", "char", "double", "float", "long"
			, "short", "ushort", "FILE", "DIR","pid"
			,"key_t", "ssize_t", "mode_t", "ino_t", "dev_t"
			, "nlink_t", "uid_t", "gid_t", "time_t", "blksize_t"
			, "blkcnt_t", "pid_t", "pthread_mutex_t", "pthread_cond_t", "pthread_t"
			, "void", "size_t", "unsigned", "sigset_t", "sigjmp_buf"
			, "rlim_t", "jmp_buf", "sig_atomic_t", "clock_t", "struct"};


operator_precedence operators[OPERATOR_CNT] = {
	{"(", 0}, {")", 0}
	,{"->", 1}
	,{"*", 4}	,{"/", 3}	,{"%", 2}
	,{"+", 6}	,{"-", 5}
	,{"<", 7}	,{"<=", 7}	,{">", 7}	,{">=", 7}
	,{"==", 8}	,{"!=", 8}
	,{"&", 9}
	,{"^", 10}
	,{"|", 11}
	,{"&&", 12}
	,{"||", 13}
	,{"=", 14}	,{"+=", 14}	,{"-=", 14}	,{"&=", 14}	,{"|=", 14}
};

void compare_tree(node *root1,  node *root2, int *result)
{//트리 비교 함수
	node *tmp;
	int cnt1, cnt2;

	if(root1 == NULL || root2 == NULL){
		*result = false;
		return;
	}//정답 트리 or 학생답 트리가 존재하지 않을경우 종료

	if(!strcmp(root1->name, "<") || !strcmp(root1->name, ">") || !strcmp(root1->name, "<=") || !strcmp(root1->name, ">=")){//root1이 비교 연산자 일경우
		if(strcmp(root1->name, root2->name) != 0){//만약 두 트리 루트의 값이 다른경우

			if(!strncmp(root2->name, "<", 1))
				strncpy(root2->name, ">", 1);

			else if(!strncmp(root2->name, ">", 1))
				strncpy(root2->name, "<", 1);

			else if(!strncmp(root2->name, "<=", 2))
				strncpy(root2->name, ">=", 2);

			else if(!strncmp(root2->name, ">=", 2))
				strncpy(root2->name, "<=", 2);

			root2 = change_sibling(root2);
		}//반대로 되있을 경우도 있으므로 루트2의 값을 반대로 바꿈
	}

	if(strcmp(root1->name, root2->name) != 0){
		*result = false;
		return;
	}//다시 비교했을때도 다르면 틀린 것이므로 종료

	if((root1->child_head != NULL && root2->child_head == NULL)
		|| (root1->child_head == NULL && root2->child_head != NULL)){
		*result = false;
		return;
	}//root1의 자식은 없는데 root2의 자식이 있거나 root1의 자식이 있는데 root2의 자식이 없는경우 틀린것 이므로 false후 종료

	else if(root1->child_head != NULL){
		if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){//root1의 자식과 root2의 자식의 sibling의 갯수가 다를경우
			*result = false;
			return;
		}//결과값 false후 종료

		if(!strcmp(root1->name, "==") || !strcmp(root1->name, "!="))
		{//root1이 == !=일 경우
			compare_tree(root1->child_head, root2->child_head, result);//root1의 자식 root2의 자식을 비교

			if(*result == false)
			{//result가 false 일 경우
				*result = true;
				root2 = change_sibling(root2);
				compare_tree(root1->child_head, root2->child_head, result);
			}//result를 true로 설정한후 root2의 sibling을 바꾸고 자식을 다시 비교
		}
		else if(!strcmp(root1->name, "+") || !strcmp(root1->name, "*")
				|| !strcmp(root1->name, "|") || !strcmp(root1->name, "&")
				|| !strcmp(root1->name, "||") || !strcmp(root1->name, "&&"))
		{//root1의 연산자가 + * | & || &&일 경우
			if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){
				*result = false;
				return;
			}//서로의 child_head의 sibling cnt가 다를경우 false설정 후 종료

			tmp = root2->child_head;//root2의 자식으로 이동

			while(tmp->prev != NULL)
				tmp = tmp->prev;//tmp를 맨 왼쪽으로 이동

			while(tmp != NULL)
			{//null이 아닐경우
				compare_tree(root1->child_head, tmp, result);//root1의 자식과 tmp를 비교

				if(*result == true)
					break;//결과값이 true면 탈출
				else{
					if(tmp->next != NULL)
						*result = true;//tmp의 next가 널이 아니면 result를 true로 설정
					tmp = tmp->next;//tmp를 오른쪽으로 이동
				}
			}
		}
		else{
			compare_tree(root1->child_head, root2->child_head, result);//root1의 자식과 root2의 자식을 비교
		}
	}


	if(root1->next != NULL){//정답 트리에 비교할게 남은경우

		if(get_sibling_cnt(root1) != get_sibling_cnt(root2)){
			*result = false;
			return;
		}//두 루트의 sibling의 갯수가 다르면 false 리턴하고 종료

		if(*result == true)
		{//result 가 true라면
			tmp = get_operator(root1);//root1 부모노드를 가져옴

			if(!strcmp(tmp->name, "+") || !strcmp(tmp->name, "*")
					|| !strcmp(tmp->name, "|") || !strcmp(tmp->name, "&")
					|| !strcmp(tmp->name, "||") || !strcmp(tmp->name, "&&"))
			{//부모 노드의 오퍼레이터가 + * | & || **인 경우
				tmp = root2;

				while(tmp->prev != NULL)
					tmp = tmp->prev;//tmp를 root2의 맨 왼쪽 노드로 이동

				while(tmp != NULL)
				{//tmp가 null이 아닐때 까지 반복
					compare_tree(root1->next, tmp, result);//root1의 우측과 tmp를 비교

					if(*result == true)
						break;//result가 true면 탈출
					else{
						if(tmp->next != NULL)
							*result = true;//tmp의 우측이 null이 아닐 경우 result를 true로 설정
						tmp = tmp->next;//tmp를 우측으로 옮김
					}
				}
			}

			else
				compare_tree(root1->next, root2->next, result);//우측 값을 비교
		}
	}
}

int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN])
{//토큰을 만드는 함수
	char *start, *end;
	char tmp[BUFLEN];
	char str2[BUFLEN];
	char *op = "(),;><=!|&^/+-*\"";//여러 연산 기호들 배열
	int row = 0;
	int i;
 	int isPointer;
	int lcount, rcount;
	int p_str;

	clear_tokens(tokens);//토큰을 초기화

	start = str;//start를 전달받은 인자의 char배열로 설정

	if(is_typeStatement(str) == 0)//is_typeStatement가 0인 경우 리턴 false
		return false;

	while(1)
	{//str의 모든 연산자 체크
		if((end = strpbrk(start, op)) == NULL)//end를 start에서 찾은 연산자 위치로 지정
			break;//start에서 연산자를 찾지 못했을경우 탈출

		if(start == end){//start와 end가 일치할 경우 실행

			if(!strncmp(start, "--", 2) || !strncmp(start, "++", 2)){//start가 --,++인지 구분
				if(!strncmp(start, "++++", 4)||!strncmp(start,"----",4))
					return false;//++++이거나 ----는 false 리턴

				if(is_character(*ltrim(start + 2))){//++과 --뒤에 빈공간을 제거한 것을 is_character것을 넘김
					if(row > 0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]))//row-1번째의 널문자 전의 마지막 문자를 체크
						return false;//row 0이하거나 널문자 일경우 false 리턴

					end = strpbrk(start + 2, op);//++과 -- 이후 연산자를 찾아서 저장
					if(end == NULL)
						end = &str[strlen(str)];//end가 널일 경우 str의 마지막으로 이동
					while(start < end) {//start가 end 앞에 있을경우 반복
						if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))//start의 전칸이 space이고 토큰의 row의 널문자 이전 문자가 널일경우 false 리턴
							return false;
						else if(*start != ' ')//start가 space일 경우
							strncat(tokens[row], start, 1);//tokens[row]에 start위치의 1칸 뒤에 붙여넣음
						start++;//start한칸 이동
					}
				}

				else if(row>0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){//row-1의 마지막 문자가 널문자가 아닐경우
					if(strstr(tokens[row - 1], "++") != NULL || strstr(tokens[row - 1], "--") != NULL)
						return false;//row-1에 ++이나 --가 있는경우 false 리턴

					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);//start부터 2바이트를 tmp에 복사
					strcat(tokens[row - 1], tmp);//row-1에 tmp를 뒤에 붙여넣음
					start += 2;//2바이트를 읽었으므로 패스
					row--;//한줄 위로 이동
				}
				else{//start부터 2바이트를 row에 뒤에 붙인후 start를 2바이트 이동
					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);
					strcat(tokens[row], tmp);
					start += 2;
				}
			}

			else if(!strncmp(start, "==", 2) || !strncmp(start, "!=", 2) || !strncmp(start, "<=", 2)
				|| !strncmp(start, ">=", 2) || !strncmp(start, "||", 2) || !strncmp(start, "&&", 2)
				|| !strncmp(start, "&=", 2) || !strncmp(start, "^=", 2) || !strncmp(start, "!=", 2)
				|| !strncmp(start, "|=", 2) || !strncmp(start, "+=", 2)	|| !strncmp(start, "-=", 2)
				|| !strncmp(start, "*=", 2) || !strncmp(start, "/=", 2)){//start가 대입이거나 비교 연산자 일경우 바로 토큰에 삽입

				strncpy(tokens[row], start, 2);
				start += 2;
			}
			else if(!strncmp(start, "->", 2))
			{//start가 ->일경우
				end = strpbrk(start + 2, op);//end를 ->다음칸으로 옮기고 오퍼레이터를 찾음

				if(end == NULL)
					end = &str[strlen(str)];//없을경우 str의 끝으로 이동

				while(start < end){//start가 다음 오퍼레이터 혹은 문장의 끝에 다을때 까지 반복
					if(*start != ' ')//빈칸이 아닐경우 row-1에 1바이트씩 삽입
						strncat(tokens[row - 1], start, 1);
					start++;//start를 다음칸으로 이동
				}
				row--;//row한칸 위로 이동
			}
			else if(*end == '&')
			{//end가 &일경우

				if(row == 0 || (strpbrk(tokens[row - 1], op) != NULL)){//row가 0이거나 row-1에 오퍼레이터가 있을경우
					end = strpbrk(start + 1, op);//start다음 위치의 오퍼레이터를 end에 저장
					if(end == NULL)
						end = &str[strlen(str)];//오퍼레이터가 없을경우 문장의 끝으로 설정

					strncat(tokens[row], start, 1);//row에 start의 1바이트를 뒤에 삽입
					start++;

					while(start < end){
						if(*(start - 1) == ' ' && tokens[row][strlen(tokens[row]) - 1] != '&')//start의 전칸이 빈칸이거나 row의 마지막 문자가 &일경우 false 리턴
							return false;
						else if(*start != ' ')//start가 space가 아닐경우 row 뒤에 붙여넣기
							strncat(tokens[row], start, 1);
						start++;
					}
				}

				else{//아닌경우 row에 start의 값을 복사
					strncpy(tokens[row], start, 1);
					start += 1;
				}

			}
		  	else if(*end == '*')//end가 *일경우
			{
				isPointer=0;//포인터 플래그

				if(row > 0)
				{//row가 0보다 큰경우

					for(i = 0; i < DATATYPE_SIZE; i++) {//row-1에서 데이터 타입을 찾아서 포인터인지 체크
						if(strstr(tokens[row - 1], datatype[i]) != NULL){
							strcat(tokens[row - 1], "*");
							start += 1;
							isPointer = 1;
							break;
						}
					}
					if(isPointer == 1)
						continue;
					if(*(start+1) !=0)//포인터가 아니고 *다음 문자가 널이 아닌경우
						end = start + 1;//end를 start다음 위치로 설정


					if(row>1 && !strcmp(tokens[row - 2], "*") && (all_star(tokens[row - 1]) == 1)){//row가 1보다 크고 row-2가 *이며 row-1이 전부 *일경우
						strncat(tokens[row - 1], start, end - start);
						row--;//row-1의 뒤에 start의 값을 end-start만큼 복사하고 row를 한칸 위로 이동
					}


					else if(is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]) == 1){//row-1에 마지막 문자가 문자인경우
						strncat(tokens[row], start, end - start);//row에 start위치부터 end-start를 삽입
					}


					else if(strpbrk(tokens[row - 1], op) != NULL){
						strncat(tokens[row] , start, end - start);

					}//row-1에 오퍼레이터가 있는경우 row에 start의 end-start만큼 뒤에 붙여넣음
					else
						strncat(tokens[row], start, end - start);//아닌경우 row에 start부터 end-start만큼 뒤에 붙여넣기

					start += (end - start);//start를 붙여넣은 길이만큼 이동
				}

			 	else if(row == 0)
				{//row가 0인경우
					if((end = strpbrk(start + 1, op)) == NULL){
						strncat(tokens[row], start, 1);
						start += 1;
					}//start의 뒤에칸에 오퍼레이터가 없는경우 row에 start의 1바이트를 뒤에 붙여넣고 start를 한칸 이동
					else{//오퍼레이터가 있는경우
						while(start < end){
							if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))//start의 전칸이 space이고 row의 마지막 문자가 문자인경우 false 리턴
								return false;
							else if(*start != ' ')//start가 space가 아닌경우 row에 뒤에 start값을 붙여넣음
								strncat(tokens[row], start, 1);
							start++;
						}
						if(all_star(tokens[row]))//row가 전부 *인경우 한칸 아래로 이동
							row--;

					}
				}
			}
			else if(*end == '(')
			{//end가 (인 경우
				lcount = 0;
				rcount = 0;
				if(row>0 && (strcmp(tokens[row - 1],"&") == 0 || strcmp(tokens[row - 1], "*") == 0)){//row가 0보다 크고 row-1이 &이거나 *인 경우
					while(*(end + lcount + 1) == '(')//end 다음칸이 (일때마다 lcount 증가
						lcount++;
					start += lcount;

					end = strpbrk(start + 1, ")");//end를 start 다음칸 부터 )의 위치를 찾아 저장

					if(end == NULL)//)가 없을경우 ()가 맞지 않으므로 false 리턴
						return false;
					else{//존재 할경우
						while(*(end + rcount +1) == ')')//end 다음칸부터 )를 찾을경우 rcount 증가
							rcount++;
						end += rcount;

						if(lcount != rcount)
							return false;//()가 갯수가 다르므로 false 리턴

						if( (row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) || row == 1){//row가 1보다 크고 row-2의 마지막이 문자가 아닐거나 row가 1인경우
							strncat(tokens[row - 1], start + 1, end - start - rcount - 1);
							row--;
							start = end + 1;
						}
						else{
							strncat(tokens[row], start, 1);
							start += 1;
						}
					}

				}
				else{
					strncat(tokens[row], start, 1);
					start += 1;
				}

			}
			else if(*end == '\"')
			{//end가 \일경우
				end = strpbrk(start + 1, "\"");//start의 다음위치에 \를 찾음

				if(end == NULL)//없을경우 false 리턴
					return false;

				else{//row에 start의 end-start+1만큼 복사하여 뒤에 붙여넣음
					strncat(tokens[row], start, end - start + 1);
					start = end + 1;
				}

			}

			else{

				if(row > 0 && !strcmp(tokens[row - 1], "++"))//row가 0보다 크고 row-1이 ++인경우 false 리턴
					return false;


				if(row > 0 && !strcmp(tokens[row - 1], "--"))//row가 0보다 크고 row-1이 --인경우 false 리턴
					return false;

				strncat(tokens[row], start, 1);
				start += 1;


				if(!strcmp(tokens[row], "-") || !strcmp(tokens[row], "+") || !strcmp(tokens[row], "--") || !strcmp(tokens[row], "++")){//row 가 - + -- ++인경우



					if(row == 0)
						row--;//row가 0 이면 한칸 위로 이동


					else if(!is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){//row -1의 마지막 문자가 문자가 아닐경우

						if(strstr(tokens[row - 1], "++") == NULL && strstr(tokens[row - 1], "--") == NULL)//row-1에 ++이나 --가 있으면 row를 한칸 위로 이동
							row--;
					}
				}
			}
		}
		else{
			if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))//row-1이 전부 *이고 row가 1보다 크며 row-2의 마지막 문자가 문자가 아닐경우 row를 한칸 위로 이동
				row--;

			if(all_star(tokens[row - 1]) && row == 1)//row-1이 전부 *이고 row가 1일경우 한칸 위로이동
				row--;

			for(i = 0; i < end - start; i++){
				if(i > 0 && *(start + i) == '.'){//i가 0보다 크고 start로부터 i번째가 .일경우 //start+i번째를 row의 뒤에 붙여넣고
					strncat(tokens[row], start + i, 1);

					while( *(start + i +1) == ' ' && i< end - start )//start의 i+1번째가 space이고 i가 end-start보다 작을때까지 i 증가
						i++;
				}
				else if(start[i] == ' '){
					while(start[i] == ' ')//space을 건너뛸때까지 i증가
						i++;
					break;
				}
				else
					strncat(tokens[row], start + i, 1);
			}

			if(start[0] == ' '){//첫문자가 space일경우 i만큼 이동하고 시작
				start += i;
				continue;
			}
			start += i;//start를 i 칸만큼 옮김
		}

		strcpy(tokens[row], ltrim(rtrim(tokens[row])));//row의 왼쪽과 오른쪽 공백 제거

		 if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1])
				&& (is_typeStatement(tokens[row - 1]) == 2
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.' ) ){//row가 0보다 크고 row의 마지막 문자가 문자이고 row-1의 타입이 2이거나 row-1의 마지막 문자가 문자이거나 .인경우

			if(row > 1 && strcmp(tokens[row - 2],"(") == 0)//row가 1보다크고 row-2가 (인 경우
			{
				if(strcmp(tokens[row - 1], "struct") != 0 && strcmp(tokens[row - 1],"unsigned") != 0)//row-1이 struct가 아니고 unsigned가 아닐경우 false 리턴
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {//row가 1이고 row의 마지막 문자가 문자인경우
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)//0번째가 extern이 아니고 unsigned가 아니며 타입이 2가 아닌경우
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){//row가 1보다 크고 row-1의 타입이 2가 아닌경우
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)//row-2가 unsigned이거나 extern이 아닌경우 false 리턴
					return false;
			}

		}

		if((row == 0 && !strcmp(tokens[row], "gcc")) ){//row가 0이고 row가 gcc인경우
			clear_tokens(tokens);//토큰 초기화
			strcpy(tokens[0], str);//str을 0번으로 복사
			return 1;
		}

		row++;//아래칸으로 이동
	}

	if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))//row-1이 전부 *이고 row가 1보다 크며 row-2의 마지막 문자가 문자가 아닌경우 아래로 이동
		row--;
	if(all_star(tokens[row - 1]) && row == 1)//row-1이 전부 *이며 row가 1인경우 아래로 이동
		row--;

	for(i = 0; i < strlen(start); i++)
	{
		if(start[i] == ' ')//space인 경우
		{
			while(start[i] == ' ')
				i++;//space를 안나 올때까지 i 증가
			if(start[0]==' ') {
				start += i;//시작 지점을 i로 변경시키고 i 를 0으로 바꿈
				i = 0;
			}
			else
				row++;//아래로 한칸 이동

			i--;
		}
		else
		{
			strncat(tokens[row], start + i, 1);//row에 start+i를 를 뒤에 붙여넣음
			if( start[i] == '.' && i<strlen(start)){//start[i]가 .이고 i가 start길이보다 작은경우
				while(start[i + 1] == ' ' && i < strlen(start))//space를 건너 뛰기위해 i를 증가
					i++;

			}
		}
		strcpy(tokens[row], ltrim(rtrim(tokens[row])));//row의 왼쪽 오른쪽 space를 없앰

		if(!strcmp(tokens[row], "lpthread") && row > 0 && !strcmp(tokens[row - 1], "-")){//row가 lpthread이고 0보다 크면서 row-1이 -인경우
			strcat(tokens[row - 1], tokens[row]);
			memset(tokens[row], 0, sizeof(tokens[row]));
			row--;
		}//row를 row-1의 뒤로 옮기고 row를 0으로 초기화 시키고 한칸 아래로 이동
	 	else if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1])
				&& (is_typeStatement(tokens[row - 1]) == 2
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.') ){//row가 0보다 크고 마지막 문자가 문자이면서 row-1의 타입이 2이거나 마지막 문자가 문자거나 .인경우

			if(row > 1 && strcmp(tokens[row-2],"(") == 0)//row가 1보다크고 row-2가 (인 경우
			{
				if(strcmp(tokens[row-1], "struct") != 0 && strcmp(tokens[row-1], "unsigned") != 0)//row-1이 struct가 unsigned가 아니면 false 리턴
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {//row가 1이고 row의 마지막 문자가 문자인 경우
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)//row=0이 extern과 unsigned가 아니고 타입이 2가 아닌경우 false 리턴
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){//row가 1보다 크고 row-1의 타입이 2인경우
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)//row-2가 unsigned와 extern이 아닌경우 false 리턴
					return false;
			}
		}
	}


	if(row > 0)
	{//row가 0보다 큰경우
		if(strcmp(tokens[0], "#include") == 0 || strcmp(tokens[0], "include") == 0 || strcmp(tokens[0], "struct") == 0){
			clear_tokens(tokens);//토큰을 초기화
			strcpy(tokens[0], remove_extraspace(str));//str의 space를 지우고 tokens[0]에 복사
		}//row=0이 #include나 include혹은 struct인 경우
	}

	if(is_typeStatement(tokens[0]) == 2 || strstr(tokens[0], "extern") != NULL){//row 0이 타입 2이거나 extern이 있는경우
		for(i = 1; i < TOKEN_CNT; i++){
			if(strcmp(tokens[i],"") == 0)//빈칸이므로 종료
				break;

			if(i != TOKEN_CNT -1 )
				strcat(tokens[0], " ");
			strcat(tokens[0], tokens[i]);//
			memset(tokens[i], 0, sizeof(tokens[i]));//token[i]를 0으로 초기화
			//token[0]에 토큰화 시킨 값들을 space로 구분시켜 저장
		}
	}


	while((p_str = find_typeSpecifier(tokens)) != -1){//토큰에 형변환이 있는지 체크
		if(!reset_tokens(p_str, tokens))
			return false;
	}


	while((p_str = find_typeSpecifier2(tokens)) != -1){//토큰이 구조체인지 찾음
		if(!reset_tokens(p_str, tokens))
			return false;
	}

	return true;
}

node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses)
{//토큰으로 트리를 만드는 함수
	node *cur = root;
	node *new;
	node *saved_operator;
	node *operator;
	int fstart;
	int i;

	while(1)
	{
		if(strcmp(tokens[*idx], "") == 0)//idx위치에 토큰의 비었을경우 탈출
			break;

		if(!strcmp(tokens[*idx], ")"))
			return get_root(cur);//idx위치에 토큰이 )일경우 root를 가져옴

		else if(!strcmp(tokens[*idx], ","))
			return get_root(cur);//,일 경우 root를 가져옴

		else if(!strcmp(tokens[*idx], "("))
		{//(일 경우

			if(*idx > 0 && !is_operator(tokens[*idx - 1]) && strcmp(tokens[*idx - 1], ",") != 0){//idx가 0보다 크고 idx-1번째가 오퍼레이터가 아니고 ,가 아닐경우

				fstart = true;//fstart를 true로 설정

				while(1)
				{
					*idx += 1;

					if(!strcmp(tokens[*idx], ")"))
						break;//토큰이 )면 탈출

					new = make_tree(NULL, tokens, idx, parentheses + 1);//new에 새로운 트리 저장

					if(new != NULL){//new가 NULL이 아닐경우
						if(fstart == true){
							cur->child_head = new;
							new->parent = cur;//cur의 자식에 new할당하고 new의 부모를 cur로 설정

							fstart = false;
						}
						else{
							cur->next = new;//cur의 우측노드를 new로 설정
							new->prev = cur;
						}

						cur = new;//new노드로 이동
					}

					if(!strcmp(tokens[*idx], ")"))
						break;//토큰이 )면 탈출
				}
			}
			else{
				*idx += 1;

				new = make_tree(NULL, tokens, idx, parentheses + 1);//new에 새로만든 트리를 저장

				if(cur == NULL)
					cur = new;//cur이 NULL일 경우 새로만든 트리저장

				else if(!strcmp(new->name, cur->name)){//new와 cur의 name이 같은경우
					if(!strcmp(new->name, "|") || !strcmp(new->name, "||")
						|| !strcmp(new->name, "&") || !strcmp(new->name, "&&"))
					{//new의 name이 | || & &&인경우
						cur = get_last_child(cur);//cur을 맨 아래 자식으로 옮김

						if(new->child_head != NULL){//new의 자식이 NULL이 아닌경우
							new = new->child_head;

							new->parent->child_head = NULL;
							new->parent = NULL;
							new->prev = cur;
							cur->next = new;
						}//new의 자식으로 이동한 뒤 new의 부모의 자식을 null new의 부모를 null new의 왼쪽을 cur로 설정
					}
					else if(!strcmp(new->name, "+") || !strcmp(new->name, "*"))
					{//name이 + *인경우
						i = 0;

						while(1)
						{
							if(!strcmp(tokens[*idx + i], ""))
								break;//빈칸이면 탈출

							if(is_operator(tokens[*idx + i]) && strcmp(tokens[*idx + i], ")") != 0)//오퍼레이터 인데 )가 아닐경우 탈출
								break;

							i++;
						}

						if(get_precedence(tokens[*idx + i]) < get_precedence(new->name))
						{//토큰의 i번째보다 new의 precedence가 높을경우
							cur = get_last_child(cur);
							cur->next = new;
							new->prev = cur;
							cur = new;
						}//cur에 cur의 마지막 자식을 저장후 cur의 next에 new를 저장하고 new로 이동
						else
						{
							cur = get_last_child(cur);//cur에 cur의 마지막 자식을 저장

							if(new->child_head != NULL){
								new = new->child_head;

								new->parent->child_head = NULL;
								new->parent = NULL;
								new->prev = cur;
								cur->next = new;
							}//new가 자식이 있을경우 new를 cur의 next로 이동시킴
						}
					}
					else{//cur의 가장 마지막 자식에 next를 new로 옮기고 new로 이동
						cur = get_last_child(cur);
						cur->next = new;
						new->prev = cur;
						cur = new;
					}
				}

				else
				{//cur의 맨 마지막 자식을 구하고 next에 new를 저장한후 new로 이동
					cur = get_last_child(cur);

					cur->next = new;
					new->prev = cur;

					cur = new;
				}
			}
		}
		else if(is_operator(tokens[*idx]))
		{//idx가 오퍼레이터 일경우
			if(!strcmp(tokens[*idx], "||") || !strcmp(tokens[*idx], "&&")
					|| !strcmp(tokens[*idx], "|") || !strcmp(tokens[*idx], "&")
					|| !strcmp(tokens[*idx], "+") || !strcmp(tokens[*idx], "*"))
			{//idx가 || && | & + *인경우
				if(is_operator(cur->name) == true && !strcmp(cur->name, tokens[*idx]))//name이 오퍼레이터고 idx와 name이 같은경우
					operator = cur;//오퍼레이터를 cur로 설정

				else
				{
					new = create_node(tokens[*idx], parentheses);//토큰으로 새로운 노드를 제작
					operator = get_most_high_precedence_node(cur, new);//오퍼레이터에 new보다 가장높은 우선순위 노드를 저장

					if(operator->parent == NULL && operator->prev == NULL){//오퍼레이터의 부모가 없고 왼쪽 노드가 없을경우

						if(get_precedence(operator->name) < get_precedence(new->name)){
							cur = insert_node(operator, new);
						}//new의 precedence가 더 높으면 오퍼레이터에 new를 삽입하고 cur을 삽입한 노드로 이동

						else if(get_precedence(operator->name) > get_precedence(new->name))
						{//현재있는 오퍼레이터의 precedence가 더 높은경우
							if(operator->child_head != NULL){
								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}//오퍼레이터의 자식이 있으면 가장 맨 마지막 자식으로 이동한후 그 위치에 삽입하고 그 노드로 이동
						}
						else
						{
							operator = cur;//오퍼레이터를 현재 노드로 설정

							while(1)
							{
								if(is_operator(operator->name) == true && !strcmp(operator->name, tokens[*idx]))//name이 오퍼레이터고 idx와 다른경우 탈출
									break;

								if(operator->prev != NULL)
									operator = operator->prev;//오퍼레이터가 왼쪽 노드가 있을경우 이동
								else
									break;//없으면 탈출
							}

							if(strcmp(operator->name, tokens[*idx]) != 0)//name과 idx가 다른경우
								operator = operator->parent;//부모로 이동

							if(operator != NULL){//오퍼레이터가 null이 아닌경우
								if(!strcmp(operator->name, tokens[*idx]))//name과 idx가 같을경우
									cur = operator;//operator로 이동
							}
						}
					}

					else
						cur = insert_node(operator, new);//operator에 new를 삽입하고 그위치로 이동
				}

			}
			else
			{
				new = create_node(tokens[*idx], parentheses);//idx로 노드 생성

				if(cur == NULL)
					cur = new;//cur이 NULL이면 new로 이동

				else
				{
					operator = get_most_high_precedence_node(cur, new);//오퍼레이터를 new보다 높은 가장 precedence가 높은 노드로 이동

					if(operator->parentheses > new->parentheses)
						cur = insert_node(operator, new);//오퍼레이터의 parentheses가 높은경우 노드를 삽입하고 cur을 그 노드로 이동

					else if(operator->parent == NULL && operator->prev ==  NULL){//오퍼레이터의 parent와 prev이 NULL인 경우

						if(get_precedence(operator->name) > get_precedence(new->name))
						{//오퍼레이터의 precedence가 높은 경우
							if(operator->child_head != NULL){

								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}//자식노드가 있는경우 오퍼레이터의 가장 마지막 자식으로 이동하고 new를 삽입하고 그 노드로 이동
						}

						else
							cur = insert_node(operator, new);//오퍼레이터에 new를 삽입하고 이동
					}

					else
						cur = insert_node(operator, new);//오퍼레이터에 new를 삽입하고 이동
				}
			}
		}
		else
		{
			new = create_node(tokens[*idx], parentheses);//idx로 새로운 노드를 제작

			if(cur == NULL)
				cur = new;//cur이 NULL인 경우 new로 설정

			else if(cur->child_head == NULL){
				cur->child_head = new;
				new->parent = cur;

				cur = new;
			}//자식 노드가 없는경우 자식노드를 new로 설정하고 new로 이동
			else{

				cur = get_last_child(cur);

				cur->next = new;
				new->prev = cur;

				cur = new;
			}//cur의 마지막 자식으로 이동하고 next를 new로 설정한뒤 이동
		}

		*idx += 1;//한칸 아래로 이동
	}

	return get_root(cur);//만든 트리 루트 노드 반환
}

node *change_sibling(node *parent)
{//좌우 노드 변환 함수
	node *tmp;

	tmp = parent->child_head;//tmp를 자식 노드로 설정

	parent->child_head = parent->child_head->next;//자식노드를 자식의 next로 설정
	parent->child_head->parent = parent;//변경된 자식의 next노드에 부모를 parent로 설정
	parent->child_head->prev = NULL;//prev를 NULL로 초기화

	parent->child_head->next = tmp;//자식의 next를 원래 자식노드로 설정
	parent->child_head->next->prev = parent->child_head;//원래 자식노드의 prev를 변경된 자식노드로 설정함
	parent->child_head->next->next = NULL;
	parent->child_head->next->parent = NULL;//원래 자식노드의 next와 parent를 null로 설정

	return parent;//변경된 parent 반환
}

node *create_node(char *name, int parentheses)
{//노드 제작 함수
	node *new;

	new = (node *)malloc(sizeof(node));
	new->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(new->name, name);//노드와 노드의 name을 동적 할당

	new->parentheses = parentheses;
	new->parent = NULL;
	new->child_head = NULL;
	new->prev = NULL;
	new->next = NULL;//새로운 노드의 인자들 초기화 후 반환

	return new;
}

int get_precedence(char *op)
{//오퍼레이터인지 체크 후 반환 그에 해당하는 오퍼레이터 값 반환
	int i;

	for(i = 2; i < OPERATOR_CNT; i++){
		if(!strcmp(operators[i].operator, op))
			return operators[i].precedence;//일치할경우 일치하는 오퍼레이터의 precedence반환
	}
	return false;
}

int is_operator(char *op)
{//오퍼레이터 인지 체크
	int i;

	for(i = 0; i < OPERATOR_CNT; i++)
	{
		if(operators[i].operator == NULL)
			break;//오퍼레이터가 NULL일 경우 탈출
		if(!strcmp(operators[i].operator, op)){
			return true;//오퍼레이터와 일치하는걸 발견하였을 경우 true 반환
		}
	}

	return false;
}

void print(node *cur)
{//노드 출력 함수
	if(cur->child_head != NULL){
		print(cur->child_head);
		printf("\n");
	}//노드의 자식 출력

	if(cur->next != NULL){
		print(cur->next);
		printf("\t");
	}//우측 노드 출력
	printf("%s", cur->name);//자기 자신 출력
}

node *get_operator(node *cur)
{//맨 왼쪽 노드의 부모 노드를 반환
	if(cur == NULL)
		return cur;//노드가 NULL일경우 NULL 리턴

	if(cur->prev != NULL)//왼쪽 노드가 있을경우
		while(cur->prev != NULL)
			cur = cur->prev;//왼쪽 노드가 없을때 까지 반환

	return cur->parent;//들어온 노드의 맨 왼쪽 노드의 부모를 반환
}

node *get_root(node *cur)
{//루트를 얻어오는 함수
	if(cur == NULL)//들어온 인자가 NULL일 경우 그대로 반환
		return cur;

	while(cur->prev != NULL)
		cur = cur->prev;//현재 노드의 맨왼쪽 노드로 이동

	if(cur->parent != NULL)
		cur = get_root(cur->parent);//현재 노드의 부모노드로 이동

	return cur;//차상위 루트 노드 반환
}

node *get_high_precedence_node(node *cur, node *new)
{//new보다 높은 우선순위 노드 반환
	if(is_operator(cur->name))//cur이 오퍼레이터 인경우
		if(get_precedence(cur->name) < get_precedence(new->name))
			return cur;//new의 연산순위이 더 늦는경우 cur반환

	if(cur->prev != NULL){//cur의 왼쪽 노드가 null이 아닌경우
		while(cur->prev != NULL){
			cur = cur->prev;

			return get_high_precedence_node(cur, new);//노드의 왼쪽노드가 NULL일때까지 옮겨가며 new와 체크
		}


		if(cur->parent != NULL)//부모가 NULL이 아닌경우
			return get_high_precedence_node(cur->parent, new);//부모와 new의 우선순위 체크
	}

	if(cur->parent == NULL)
		return cur;//부모가 NULL인 경우 cur반환
}

node *get_most_high_precedence_node(node *cur, node *new)
{//가장 상위의 노드 반환
	node *operator = get_high_precedence_node(cur, new);//반환된 우선순위가 높은 노드 저장
	node *saved_operator = operator;

	while(1)
	{
		if(saved_operator->parent == NULL)//부모가 없는경우 탈출
			break;

		if(saved_operator->prev != NULL)
			operator = get_high_precedence_node(saved_operator->prev, new);//prev가 있는경우 우선순위가 높은 노드를 체크하여 저장

		else if(saved_operator->parent != NULL)
			operator = get_high_precedence_node(saved_operator->parent, new);//부모가 있는경우 부모의 우선순위가 높은 노드를 체크하여 저장

		saved_operator = operator;//임시 노드에 반환받은 노드 저장
	}

	return saved_operator;
}

node *insert_node(node *old, node *new)
{//old위치에 new 삽입
	if(old->prev != NULL){//old의 prev이 null이 아닐경우
		new->prev = old->prev;//new의 이전 노드를 old의 이전 노드로 설정
		old->prev->next = new;//old이전 노드의 우측 노드를 new로 설정
		old->prev = NULL;//old의 좌측노드를 NULL로 설정
	}

	new->child_head = old;//new의 자식을 old로 설정
	old->parent = new;//old의 부모를 new로 설정

	return new;//new노드 반환
}

node *get_last_child(node *cur)
{//마지막 자식 노드 반환
	if(cur->child_head != NULL)
		cur = cur->child_head;//자식노드가 있을경우 자식노드로 이동

	while(cur->next != NULL)
		cur = cur->next;//맨 오른쪽 노드로 이동후 반환

	return cur;
}

int get_sibling_cnt(node *cur)
{//좌우의 노드 수를 셈
	int i = 0;

	while(cur->prev != NULL)
		cur = cur->prev;//노드를 맨 왼쪽으로 옮김

	while(cur->next != NULL){
		cur = cur->next;//노드를 오른쪽으로 옮기면서 NULL을 만날때까지 카운트 증가
		i++;
	}

	return i;//카운트 반환
}

void free_node(node *cur)
{//노드 해제 함수
	if(cur->child_head != NULL)
		free_node(cur->child_head);//자식 노드 해제

	if(cur->next != NULL)
		free_node(cur->next);//노드의 next 해제

	if(cur != NULL){
		cur->prev = NULL;
		cur->next = NULL;
		cur->parent = NULL;
		cur->child_head = NULL;
		free(cur);//노드와 연결 지점들을 없애고 free
	}
}


int is_character(char c)
{//인자로 들어온 값이 문자열인지 반환
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_typeStatement(char *str)
{//타입의 상태를 반환
	char *start;
	char str2[BUFLEN] = {0};
	char tmp[BUFLEN] = {0};
	char tmp2[BUFLEN] = {0};
	int i;

	start = str;//start를 전달받은 인자로 설정
	strncpy(str2,str,strlen(str));//str2에 str의 길이만큼 str을 복사
	remove_space(str2);//str2의 공백을 제거

	while(start[0] == ' ')
		start += 1;//start의 왼쪽 공백을 건너뛰기 위해 공백이 나오지 않을때까지 start의 포인터를 한칸씩 이동

	if(strstr(str2, "gcc") != NULL)
	{//gcc란 문자열을 str2에서 검색
		strncpy(tmp2, start, strlen("gcc"));//tmp2에 gcc란 문자열의 길이만큼 start에서 복사하여 삽입
		if(strcmp(tmp2,"gcc") != 0)//tmp2가 gcc가 아닐경우
			return 0;//0 리턴
		else
			return 2;//tmp2가 gcc일 경우 2 리턴
	}

	for(i = 0; i < DATATYPE_SIZE; i++)
	{//모든 데이터 타입을 검사
		if(strstr(str2,datatype[i]) != NULL)
		{//str2에서 데이터 타입을 찾은경우
			strncpy(tmp, str2, strlen(datatype[i]));//tmp에 str2의 발견한 데이터타입의 길이만큼 복사
			strncpy(tmp2, start, strlen(datatype[i]));//tmp2에 start에서 데이터타입의 길이만큼 복사

			if(strcmp(tmp, datatype[i]) == 0)//만약 tmp가 데이터 타입과 일치하는경우
				if(strcmp(tmp, tmp2) != 0)
					return 0;//tmp와 tmp2가 다른경우 리턴 0
				else
					return 2;//같은 경우 리턴 2
		}

	}
	return 1;

}

int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN])
{//타입 구분 함수1
	int i, j;

	for(i = 0; i < TOKEN_CNT; i++)
	{
		for(j = 0; j < DATATYPE_SIZE; j++)
		{//형변환 체크
			if(strstr(tokens[i], datatype[j]) != NULL && i > 0)
			{//데이터 타입과 일치하는게 토큰에 있고 i가 0보다 클경우
				if(!strcmp(tokens[i - 1], "(") && !strcmp(tokens[i + 1], ")")//i-1이 (이고 i+1이 )이며 i+2가 & * ( ) - + 문자인경우 i 리턴
						&& (tokens[i + 2][0] == '&' || tokens[i + 2][0] == '*'
							|| tokens[i + 2][0] == ')' || tokens[i + 2][0] == '('
							|| tokens[i + 2][0] == '-' || tokens[i + 2][0] == '+'
							|| is_character(tokens[i + 2][0])))
					return i;
			}
		}
	}
	return -1;
}

int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN])
{//타입 구분 함수 2
    int i, j;


    for(i = 0; i < TOKEN_CNT; i++)
    {//구조체인지 찾기위해서
		if(!strcmp(tokens[i], "struct") && (i+1) <= TOKEN_CNT && is_character(tokens[i + 1][strlen(tokens[i + 1]) - 1]))
				return i;
    }
    return -1;
}

int all_star(char *str)
{//들어 있는 값이 전부 *인지 체크
	int i;
	int length= strlen(str);

 	if(length == 0)//길이가 0인 경우 0리턴
		return 0;

	for(i = 0; i < length; i++)
		if(str[i] != '*')//str에 *이 아닌값이 있는경우 0 리턴
			return 0;
	return 1;

}

int all_character(char *str)
{//문자가 있는지 체크
	int i;

	for(i = 0; i < strlen(str); i++)
		if(is_character(str[i]))
			return 1;//문자가 있을경우 1리턴
	return 0;

}

int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN])
{//토큰 합성 함수
	int i;
	int j = start - 1;
	int lcount = 0, rcount = 0;
	int sub_lcount = 0, sub_rcount = 0;

	if(start > -1){
		if(!strcmp(tokens[start], "struct")) {//토큰에 구조체가 있는경우
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start+1]);//토큰의 뒤에 space를 삽입하고 start+1번째를 start 뒤에 붙임

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);//토큰을 한칸씩 위로 옮김
				memset(tokens[i + 1], 0, sizeof(tokens[0]));//i+1을 0으로 초기화
			}
		}

		else if(!strcmp(tokens[start], "unsigned") && strcmp(tokens[start+1], ")") != 0) {//unsigned형 이며 )가 start+1에 있는경우
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start + 1]);
			strcat(tokens[start], tokens[start + 2]);//start에 space 삽입후 start+1과 start+2를 순차적으로 start 뒤에 붙임

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));//토큰을 한칸씩 위로 옮기고 옮긴 데이터칸은 0으로 초기화
			}
		}

     		j = start + 1;
        	while(!strcmp(tokens[j], ")")){//토큰의 j위치가 )가 있는경우
                	rcount ++;//우측 괄호 카운트 증가
                	if(j==TOKEN_CNT)
                        	break;//j가 토큰의 최대치에 도달시 탈출
                	j++;//아닐때 까지 반복
        	}

		j = start - 1;
		while(!strcmp(tokens[j], "(")){//토큰의 j위치에 (이 있는 경우
        	        lcount ++;//좌측 괄호 카운트 증가
                	if(j == 0)//0이면 탈출
                        	break;
               		j--;//j를 한칸씩 왼쪽으로 이동
		}
		if( (j!=0 && is_character(tokens[j][strlen(tokens[j])-1]) ) || j==0)//j가 0이 아닐때 j의 마지막 문자가 문자이거나 j가 0인경우
			lcount = rcount;//lcount에 rcount 대입

		if(lcount != rcount )//왼쪽과 오른쪽 괄호 수가 다르므로 false 리턴
			return false;

		if( (start - lcount) >0 && !strcmp(tokens[start - lcount - 1], "sizeof")){//start에서 왼쪽 괄호수 만큼 왼쪽으로 이동시킨게 1보다 크고 괄호 위에칸에 sizeof가 있을경우 true 반환
			return true;
		}

		else if((!strcmp(tokens[start], "unsigned") || !strcmp(tokens[start], "struct")) && strcmp(tokens[start+1], ")")) {//unsigned나 struct이고 그 아래칸에 )이 있을경우
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcpy(tokens[start - lcount + 1], tokens[start + rcount]);//start - lcount에 start와 start+1을 붙이고 그 다음칸에 start+rcount의 내용을 복사

			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount]);
				memset(tokens[i + lcount + rcount], 0, sizeof(tokens[0]));
			}//괄호 뒤의 내용을 앞으로 끌어오고 내용이 있던곳은 0으로 초기화


		}
 		else{
			if(tokens[start + 2][0] == '('){//start+2가 (로 시작하는 경우
				j = start + 2;
				while(!strcmp(tokens[j], "(")){//(가 연속으로 안나올때까지 서브 lcount증가
					sub_lcount++;
					j++;
				}
				if(!strcmp(tokens[j + 1],")")){//그 다음칸의 토큰이)인경우
					j = j + 1;
					while(!strcmp(tokens[j], ")")){//)이 연속적으로 나오지 않을때까지 서브 rcount증가
						sub_rcount++;
						j++;
					}
				}
				else//아닌경우 false 리턴
					return false;

				if(sub_lcount != sub_rcount)
					return false;//서브 lcount와 rcount가 다를경우 false 리턴

				strcpy(tokens[start + 2], tokens[start + 2 + sub_lcount]);//start+2에 start +2 +sub_lcount의 내용을 복사
				for(int i = start + 3; i<TOKEN_CNT; i++)
					memset(tokens[i], 0, sizeof(tokens[0]));//내용을 옮겨왔으므로 그 사이 내용 초기화

			}
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcat(tokens[start - lcount], tokens[start + rcount + 1]);//start-lcount에 start와 start+1과 start + rcount +1번째를 순차적으로 뒤에 붙여넣음

			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount -1; i++) {//데이터를 옮기고 그 사이칸들 0으로 초기화
				strcpy(tokens[i], tokens[i + lcount + rcount +1]);
				memset(tokens[i + lcount + rcount + 1], 0, sizeof(tokens[0]));

			}
		}
	}
	return true;//true 반환
}

void clear_tokens(char tokens[TOKEN_CNT][MINLEN])
{//토큰을 초기화
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		memset(tokens[i], 0, sizeof(tokens[i]));//토큰을 전부 0으로 채움
}

char *rtrim(char *_str)
{//오른쪽 공백제거
	char tmp[BUFLEN];
	char *end;

	strcpy(tmp, _str);//tmp에 인자를 복사
	end = tmp + strlen(tmp) - 1;
	while(end != _str && isspace(*end))
		--end;//end가 space이고 첫칸이 아닌 경우 한칸 감소시킴

	*(end + 1) = '\0';//마지막 문자를 널문자로 설정
	_str = tmp;
	return _str;
}

char *ltrim(char *_str)
{//왼쪽 공백 제거
	char *start = _str;

	while(*start != '\0' && isspace(*start))
		++start;//start가 널문자가 아니고 start가 공백일경우 start를 증가
	_str = start;
	return _str;
}

char* remove_extraspace(char *str)
{//공백을 없애는 함수
	int i;
	char *str2 = (char*)malloc(sizeof(char) * BUFLEN);
	char *start, *end;
	char temp[BUFLEN] = "";
	int position;

	if(strstr(str,"include<")!=NULL){//str에서 include<를 검색
		start = str;
		end = strpbrk(str, "<");//end를 <가 나오는 지점으로 설정
		position = end - start;

		strncat(temp, str, position);
		strncat(temp, " ", 1);
		strncat(temp, str + position, strlen(str) - position + 1);//include와 <사이 공백 삽입

		str = temp;
	}

	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] ==' ')
		{//str[i]가 공백일 경우
			if(i == 0 && str[0] ==' ')
				while(str[i + 1] == ' ')
					i++;//첫칸이 space이고 i가 0인경우 연속으로 공백이 안나올때까지 i증가
			else{
				if(i > 0 && str[i - 1] != ' ')
					str2[strlen(str2)] = str[i];//전칸이 공백일경우 str2의 마지막칸에 str[i]값을 뒤에 붙여넣음
				while(str[i + 1] == ' ')
					i++;//연속으로 공백이 안나올때까지 i증가
			}
		}
		else
			str2[strlen(str2)] = str[i];//str2의 마지막 칸에 str[i]값 삽입
	}

	return str2;//str2리턴
}



void remove_space(char *str)
{//공백을 제거하는 함수
	char* i = str;
	char* j = str;

	while(*j != 0)//str[j]가 널문자가 아닐경우
	{
		*i = *j++;//str[i]에 str[j]값을 넣고 j를 오른쪽으로 한칸 옮김
		if(*i != ' ')
			i++;//str[i]가 space가 아닐경우 i를 오른쪽으로 한칸 이동
	}
	*i = 0;//마지막 문자를 널문자로 설정
}

int check_brackets(char *str)
{//괄호가 온전히 있는지 체크
	char *start = str;
	int lcount = 0, rcount = 0;

	while(1){
		if((start = strpbrk(start, "()")) != NULL){//start 에서 ()를 검색
			if(*(start) == '(')//(일 경우 왼쪽 괄호가 있으므로 lcount를 증가
				lcount++;
			else
				rcount++;//)일 경우 오른쪽 괄호가 있으므로 rcount를 증가

			start += 1;//다음 괄호를 찾기 위해 위치를 이동
		}
		else
			break;//괄호가 없으므로 탈출
	}

	if(lcount != rcount)
		return 0;//괄호가 왼쪽 오른쪽이 갯수가 다르므로 틀림
	else
		return 1;//괄호의 갯수가 같으므로 1리턴
}

int get_token_cnt(char tokens[TOKEN_CNT][MINLEN])
{//토큰의 갯수를 반환
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		if(!strcmp(tokens[i], ""))//토큰의 i번째가 빈칸일경우 종료
			break;

	return i;
}
