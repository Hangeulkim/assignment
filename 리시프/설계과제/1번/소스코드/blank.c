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
{//Ʈ�� �� �Լ�
	node *tmp;
	int cnt1, cnt2;

	if(root1 == NULL || root2 == NULL){
		*result = false;
		return;
	}//���� Ʈ�� or �л��� Ʈ���� �������� ������� ����

	if(!strcmp(root1->name, "<") || !strcmp(root1->name, ">") || !strcmp(root1->name, "<=") || !strcmp(root1->name, ">=")){//root1�� �� ������ �ϰ��
		if(strcmp(root1->name, root2->name) != 0){//���� �� Ʈ�� ��Ʈ�� ���� �ٸ����

			if(!strncmp(root2->name, "<", 1))
				strncpy(root2->name, ">", 1);

			else if(!strncmp(root2->name, ">", 1))
				strncpy(root2->name, "<", 1);

			else if(!strncmp(root2->name, "<=", 2))
				strncpy(root2->name, ">=", 2);

			else if(!strncmp(root2->name, ">=", 2))
				strncpy(root2->name, "<=", 2);

			root2 = change_sibling(root2);
		}//�ݴ�� ������ ��쵵 �����Ƿ� ��Ʈ2�� ���� �ݴ�� �ٲ�
	}

	if(strcmp(root1->name, root2->name) != 0){
		*result = false;
		return;
	}//�ٽ� ���������� �ٸ��� Ʋ�� ���̹Ƿ� ����

	if((root1->child_head != NULL && root2->child_head == NULL)
		|| (root1->child_head == NULL && root2->child_head != NULL)){
		*result = false;
		return;
	}//root1�� �ڽ��� ���µ� root2�� �ڽ��� �ְų� root1�� �ڽ��� �ִµ� root2�� �ڽ��� ���°�� Ʋ���� �̹Ƿ� false�� ����

	else if(root1->child_head != NULL){
		if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){//root1�� �ڽİ� root2�� �ڽ��� sibling�� ������ �ٸ����
			*result = false;
			return;
		}//����� false�� ����

		if(!strcmp(root1->name, "==") || !strcmp(root1->name, "!="))
		{//root1�� == !=�� ���
			compare_tree(root1->child_head, root2->child_head, result);//root1�� �ڽ� root2�� �ڽ��� ��

			if(*result == false)
			{//result�� false �� ���
				*result = true;
				root2 = change_sibling(root2);
				compare_tree(root1->child_head, root2->child_head, result);
			}//result�� true�� �������� root2�� sibling�� �ٲٰ� �ڽ��� �ٽ� ��
		}
		else if(!strcmp(root1->name, "+") || !strcmp(root1->name, "*")
				|| !strcmp(root1->name, "|") || !strcmp(root1->name, "&")
				|| !strcmp(root1->name, "||") || !strcmp(root1->name, "&&"))
		{//root1�� �����ڰ� + * | & || &&�� ���
			if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){
				*result = false;
				return;
			}//������ child_head�� sibling cnt�� �ٸ���� false���� �� ����

			tmp = root2->child_head;//root2�� �ڽ����� �̵�

			while(tmp->prev != NULL)
				tmp = tmp->prev;//tmp�� �� �������� �̵�

			while(tmp != NULL)
			{//null�� �ƴҰ��
				compare_tree(root1->child_head, tmp, result);//root1�� �ڽİ� tmp�� ��

				if(*result == true)
					break;//������� true�� Ż��
				else{
					if(tmp->next != NULL)
						*result = true;//tmp�� next�� ���� �ƴϸ� result�� true�� ����
					tmp = tmp->next;//tmp�� ���������� �̵�
				}
			}
		}
		else{
			compare_tree(root1->child_head, root2->child_head, result);//root1�� �ڽİ� root2�� �ڽ��� ��
		}
	}


	if(root1->next != NULL){//���� Ʈ���� ���Ұ� �������

		if(get_sibling_cnt(root1) != get_sibling_cnt(root2)){
			*result = false;
			return;
		}//�� ��Ʈ�� sibling�� ������ �ٸ��� false �����ϰ� ����

		if(*result == true)
		{//result �� true���
			tmp = get_operator(root1);//root1 �θ��带 ������

			if(!strcmp(tmp->name, "+") || !strcmp(tmp->name, "*")
					|| !strcmp(tmp->name, "|") || !strcmp(tmp->name, "&")
					|| !strcmp(tmp->name, "||") || !strcmp(tmp->name, "&&"))
			{//�θ� ����� ���۷����Ͱ� + * | & || **�� ���
				tmp = root2;

				while(tmp->prev != NULL)
					tmp = tmp->prev;//tmp�� root2�� �� ���� ���� �̵�

				while(tmp != NULL)
				{//tmp�� null�� �ƴҶ� ���� �ݺ�
					compare_tree(root1->next, tmp, result);//root1�� ������ tmp�� ��

					if(*result == true)
						break;//result�� true�� Ż��
					else{
						if(tmp->next != NULL)
							*result = true;//tmp�� ������ null�� �ƴ� ��� result�� true�� ����
						tmp = tmp->next;//tmp�� �������� �ű�
					}
				}
			}

			else
				compare_tree(root1->next, root2->next, result);//���� ���� ��
		}
	}
}

int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN])
{//��ū�� ����� �Լ�
	char *start, *end;
	char tmp[BUFLEN];
	char str2[BUFLEN];
	char *op = "(),;><=!|&^/+-*\"";//���� ���� ��ȣ�� �迭
	int row = 0;
	int i;
 	int isPointer;
	int lcount, rcount;
	int p_str;

	clear_tokens(tokens);//��ū�� �ʱ�ȭ

	start = str;//start�� ���޹��� ������ char�迭�� ����

	if(is_typeStatement(str) == 0)//is_typeStatement�� 0�� ��� ���� false
		return false;

	while(1)
	{//str�� ��� ������ üũ
		if((end = strpbrk(start, op)) == NULL)//end�� start���� ã�� ������ ��ġ�� ����
			break;//start���� �����ڸ� ã�� ��������� Ż��

		if(start == end){//start�� end�� ��ġ�� ��� ����

			if(!strncmp(start, "--", 2) || !strncmp(start, "++", 2)){//start�� --,++���� ����
				if(!strncmp(start, "++++", 4)||!strncmp(start,"----",4))
					return false;//++++�̰ų� ----�� false ����

				if(is_character(*ltrim(start + 2))){//++�� --�ڿ� ������� ������ ���� is_character���� �ѱ�
					if(row > 0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]))//row-1��°�� �ι��� ���� ������ ���ڸ� üũ
						return false;//row 0���ϰų� �ι��� �ϰ�� false ����

					end = strpbrk(start + 2, op);//++�� -- ���� �����ڸ� ã�Ƽ� ����
					if(end == NULL)
						end = &str[strlen(str)];//end�� ���� ��� str�� ���������� �̵�
					while(start < end) {//start�� end �տ� ������� �ݺ�
						if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))//start�� ��ĭ�� space�̰� ��ū�� row�� �ι��� ���� ���ڰ� ���ϰ�� false ����
							return false;
						else if(*start != ' ')//start�� space�� ���
							strncat(tokens[row], start, 1);//tokens[row]�� start��ġ�� 1ĭ �ڿ� �ٿ�����
						start++;//start��ĭ �̵�
					}
				}

				else if(row>0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){//row-1�� ������ ���ڰ� �ι��ڰ� �ƴҰ��
					if(strstr(tokens[row - 1], "++") != NULL || strstr(tokens[row - 1], "--") != NULL)
						return false;//row-1�� ++�̳� --�� �ִ°�� false ����

					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);//start���� 2����Ʈ�� tmp�� ����
					strcat(tokens[row - 1], tmp);//row-1�� tmp�� �ڿ� �ٿ�����
					start += 2;//2����Ʈ�� �о����Ƿ� �н�
					row--;//���� ���� �̵�
				}
				else{//start���� 2����Ʈ�� row�� �ڿ� ������ start�� 2����Ʈ �̵�
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
				|| !strncmp(start, "*=", 2) || !strncmp(start, "/=", 2)){//start�� �����̰ų� �� ������ �ϰ�� �ٷ� ��ū�� ����

				strncpy(tokens[row], start, 2);
				start += 2;
			}
			else if(!strncmp(start, "->", 2))
			{//start�� ->�ϰ��
				end = strpbrk(start + 2, op);//end�� ->����ĭ���� �ű�� ���۷����͸� ã��

				if(end == NULL)
					end = &str[strlen(str)];//������� str�� ������ �̵�

				while(start < end){//start�� ���� ���۷����� Ȥ�� ������ ���� ������ ���� �ݺ�
					if(*start != ' ')//��ĭ�� �ƴҰ�� row-1�� 1����Ʈ�� ����
						strncat(tokens[row - 1], start, 1);
					start++;//start�� ����ĭ���� �̵�
				}
				row--;//row��ĭ ���� �̵�
			}
			else if(*end == '&')
			{//end�� &�ϰ��

				if(row == 0 || (strpbrk(tokens[row - 1], op) != NULL)){//row�� 0�̰ų� row-1�� ���۷����Ͱ� �������
					end = strpbrk(start + 1, op);//start���� ��ġ�� ���۷����͸� end�� ����
					if(end == NULL)
						end = &str[strlen(str)];//���۷����Ͱ� ������� ������ ������ ����

					strncat(tokens[row], start, 1);//row�� start�� 1����Ʈ�� �ڿ� ����
					start++;

					while(start < end){
						if(*(start - 1) == ' ' && tokens[row][strlen(tokens[row]) - 1] != '&')//start�� ��ĭ�� ��ĭ�̰ų� row�� ������ ���ڰ� &�ϰ�� false ����
							return false;
						else if(*start != ' ')//start�� space�� �ƴҰ�� row �ڿ� �ٿ��ֱ�
							strncat(tokens[row], start, 1);
						start++;
					}
				}

				else{//�ƴѰ�� row�� start�� ���� ����
					strncpy(tokens[row], start, 1);
					start += 1;
				}

			}
		  	else if(*end == '*')//end�� *�ϰ��
			{
				isPointer=0;//������ �÷���

				if(row > 0)
				{//row�� 0���� ū���

					for(i = 0; i < DATATYPE_SIZE; i++) {//row-1���� ������ Ÿ���� ã�Ƽ� ���������� üũ
						if(strstr(tokens[row - 1], datatype[i]) != NULL){
							strcat(tokens[row - 1], "*");
							start += 1;
							isPointer = 1;
							break;
						}
					}
					if(isPointer == 1)
						continue;
					if(*(start+1) !=0)//�����Ͱ� �ƴϰ� *���� ���ڰ� ���� �ƴѰ��
						end = start + 1;//end�� start���� ��ġ�� ����


					if(row>1 && !strcmp(tokens[row - 2], "*") && (all_star(tokens[row - 1]) == 1)){//row�� 1���� ũ�� row-2�� *�̸� row-1�� ���� *�ϰ��
						strncat(tokens[row - 1], start, end - start);
						row--;//row-1�� �ڿ� start�� ���� end-start��ŭ �����ϰ� row�� ��ĭ ���� �̵�
					}


					else if(is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]) == 1){//row-1�� ������ ���ڰ� �����ΰ��
						strncat(tokens[row], start, end - start);//row�� start��ġ���� end-start�� ����
					}


					else if(strpbrk(tokens[row - 1], op) != NULL){
						strncat(tokens[row] , start, end - start);

					}//row-1�� ���۷����Ͱ� �ִ°�� row�� start�� end-start��ŭ �ڿ� �ٿ�����
					else
						strncat(tokens[row], start, end - start);//�ƴѰ�� row�� start���� end-start��ŭ �ڿ� �ٿ��ֱ�

					start += (end - start);//start�� �ٿ����� ���̸�ŭ �̵�
				}

			 	else if(row == 0)
				{//row�� 0�ΰ��
					if((end = strpbrk(start + 1, op)) == NULL){
						strncat(tokens[row], start, 1);
						start += 1;
					}//start�� �ڿ�ĭ�� ���۷����Ͱ� ���°�� row�� start�� 1����Ʈ�� �ڿ� �ٿ��ְ� start�� ��ĭ �̵�
					else{//���۷����Ͱ� �ִ°��
						while(start < end){
							if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))//start�� ��ĭ�� space�̰� row�� ������ ���ڰ� �����ΰ�� false ����
								return false;
							else if(*start != ' ')//start�� space�� �ƴѰ�� row�� �ڿ� start���� �ٿ�����
								strncat(tokens[row], start, 1);
							start++;
						}
						if(all_star(tokens[row]))//row�� ���� *�ΰ�� ��ĭ �Ʒ��� �̵�
							row--;

					}
				}
			}
			else if(*end == '(')
			{//end�� (�� ���
				lcount = 0;
				rcount = 0;
				if(row>0 && (strcmp(tokens[row - 1],"&") == 0 || strcmp(tokens[row - 1], "*") == 0)){//row�� 0���� ũ�� row-1�� &�̰ų� *�� ���
					while(*(end + lcount + 1) == '(')//end ����ĭ�� (�϶����� lcount ����
						lcount++;
					start += lcount;

					end = strpbrk(start + 1, ")");//end�� start ����ĭ ���� )�� ��ġ�� ã�� ����

					if(end == NULL)//)�� ������� ()�� ���� �����Ƿ� false ����
						return false;
					else{//���� �Ұ��
						while(*(end + rcount +1) == ')')//end ����ĭ���� )�� ã����� rcount ����
							rcount++;
						end += rcount;

						if(lcount != rcount)
							return false;//()�� ������ �ٸ��Ƿ� false ����

						if( (row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) || row == 1){//row�� 1���� ũ�� row-2�� �������� ���ڰ� �ƴҰų� row�� 1�ΰ��
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
			{//end�� \�ϰ��
				end = strpbrk(start + 1, "\"");//start�� ������ġ�� \�� ã��

				if(end == NULL)//������� false ����
					return false;

				else{//row�� start�� end-start+1��ŭ �����Ͽ� �ڿ� �ٿ�����
					strncat(tokens[row], start, end - start + 1);
					start = end + 1;
				}

			}

			else{

				if(row > 0 && !strcmp(tokens[row - 1], "++"))//row�� 0���� ũ�� row-1�� ++�ΰ�� false ����
					return false;


				if(row > 0 && !strcmp(tokens[row - 1], "--"))//row�� 0���� ũ�� row-1�� --�ΰ�� false ����
					return false;

				strncat(tokens[row], start, 1);
				start += 1;


				if(!strcmp(tokens[row], "-") || !strcmp(tokens[row], "+") || !strcmp(tokens[row], "--") || !strcmp(tokens[row], "++")){//row �� - + -- ++�ΰ��



					if(row == 0)
						row--;//row�� 0 �̸� ��ĭ ���� �̵�


					else if(!is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){//row -1�� ������ ���ڰ� ���ڰ� �ƴҰ��

						if(strstr(tokens[row - 1], "++") == NULL && strstr(tokens[row - 1], "--") == NULL)//row-1�� ++�̳� --�� ������ row�� ��ĭ ���� �̵�
							row--;
					}
				}
			}
		}
		else{
			if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))//row-1�� ���� *�̰� row�� 1���� ũ�� row-2�� ������ ���ڰ� ���ڰ� �ƴҰ�� row�� ��ĭ ���� �̵�
				row--;

			if(all_star(tokens[row - 1]) && row == 1)//row-1�� ���� *�̰� row�� 1�ϰ�� ��ĭ �����̵�
				row--;

			for(i = 0; i < end - start; i++){
				if(i > 0 && *(start + i) == '.'){//i�� 0���� ũ�� start�κ��� i��°�� .�ϰ�� //start+i��°�� row�� �ڿ� �ٿ��ְ�
					strncat(tokens[row], start + i, 1);

					while( *(start + i +1) == ' ' && i< end - start )//start�� i+1��°�� space�̰� i�� end-start���� ���������� i ����
						i++;
				}
				else if(start[i] == ' '){
					while(start[i] == ' ')//space�� �ǳʶ۶����� i����
						i++;
					break;
				}
				else
					strncat(tokens[row], start + i, 1);
			}

			if(start[0] == ' '){//ù���ڰ� space�ϰ�� i��ŭ �̵��ϰ� ����
				start += i;
				continue;
			}
			start += i;//start�� i ĭ��ŭ �ű�
		}

		strcpy(tokens[row], ltrim(rtrim(tokens[row])));//row�� ���ʰ� ������ ���� ����

		 if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1])
				&& (is_typeStatement(tokens[row - 1]) == 2
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.' ) ){//row�� 0���� ũ�� row�� ������ ���ڰ� �����̰� row-1�� Ÿ���� 2�̰ų� row-1�� ������ ���ڰ� �����̰ų� .�ΰ��

			if(row > 1 && strcmp(tokens[row - 2],"(") == 0)//row�� 1����ũ�� row-2�� (�� ���
			{
				if(strcmp(tokens[row - 1], "struct") != 0 && strcmp(tokens[row - 1],"unsigned") != 0)//row-1�� struct�� �ƴϰ� unsigned�� �ƴҰ�� false ����
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {//row�� 1�̰� row�� ������ ���ڰ� �����ΰ��
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)//0��°�� extern�� �ƴϰ� unsigned�� �ƴϸ� Ÿ���� 2�� �ƴѰ��
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){//row�� 1���� ũ�� row-1�� Ÿ���� 2�� �ƴѰ��
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)//row-2�� unsigned�̰ų� extern�� �ƴѰ�� false ����
					return false;
			}

		}

		if((row == 0 && !strcmp(tokens[row], "gcc")) ){//row�� 0�̰� row�� gcc�ΰ��
			clear_tokens(tokens);//��ū �ʱ�ȭ
			strcpy(tokens[0], str);//str�� 0������ ����
			return 1;
		}

		row++;//�Ʒ�ĭ���� �̵�
	}

	if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))//row-1�� ���� *�̰� row�� 1���� ũ�� row-2�� ������ ���ڰ� ���ڰ� �ƴѰ�� �Ʒ��� �̵�
		row--;
	if(all_star(tokens[row - 1]) && row == 1)//row-1�� ���� *�̸� row�� 1�ΰ�� �Ʒ��� �̵�
		row--;

	for(i = 0; i < strlen(start); i++)
	{
		if(start[i] == ' ')//space�� ���
		{
			while(start[i] == ' ')
				i++;//space�� �ȳ� �ö����� i ����
			if(start[0]==' ') {
				start += i;//���� ������ i�� �����Ű�� i �� 0���� �ٲ�
				i = 0;
			}
			else
				row++;//�Ʒ��� ��ĭ �̵�

			i--;
		}
		else
		{
			strncat(tokens[row], start + i, 1);//row�� start+i�� �� �ڿ� �ٿ�����
			if( start[i] == '.' && i<strlen(start)){//start[i]�� .�̰� i�� start���̺��� �������
				while(start[i + 1] == ' ' && i < strlen(start))//space�� �ǳ� �ٱ����� i�� ����
					i++;

			}
		}
		strcpy(tokens[row], ltrim(rtrim(tokens[row])));//row�� ���� ������ space�� ����

		if(!strcmp(tokens[row], "lpthread") && row > 0 && !strcmp(tokens[row - 1], "-")){//row�� lpthread�̰� 0���� ũ�鼭 row-1�� -�ΰ��
			strcat(tokens[row - 1], tokens[row]);
			memset(tokens[row], 0, sizeof(tokens[row]));
			row--;
		}//row�� row-1�� �ڷ� �ű�� row�� 0���� �ʱ�ȭ ��Ű�� ��ĭ �Ʒ��� �̵�
	 	else if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1])
				&& (is_typeStatement(tokens[row - 1]) == 2
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.') ){//row�� 0���� ũ�� ������ ���ڰ� �����̸鼭 row-1�� Ÿ���� 2�̰ų� ������ ���ڰ� ���ڰų� .�ΰ��

			if(row > 1 && strcmp(tokens[row-2],"(") == 0)//row�� 1����ũ�� row-2�� (�� ���
			{
				if(strcmp(tokens[row-1], "struct") != 0 && strcmp(tokens[row-1], "unsigned") != 0)//row-1�� struct�� unsigned�� �ƴϸ� false ����
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {//row�� 1�̰� row�� ������ ���ڰ� ������ ���
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)//row=0�� extern�� unsigned�� �ƴϰ� Ÿ���� 2�� �ƴѰ�� false ����
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){//row�� 1���� ũ�� row-1�� Ÿ���� 2�ΰ��
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)//row-2�� unsigned�� extern�� �ƴѰ�� false ����
					return false;
			}
		}
	}


	if(row > 0)
	{//row�� 0���� ū���
		if(strcmp(tokens[0], "#include") == 0 || strcmp(tokens[0], "include") == 0 || strcmp(tokens[0], "struct") == 0){
			clear_tokens(tokens);//��ū�� �ʱ�ȭ
			strcpy(tokens[0], remove_extraspace(str));//str�� space�� ����� tokens[0]�� ����
		}//row=0�� #include�� includeȤ�� struct�� ���
	}

	if(is_typeStatement(tokens[0]) == 2 || strstr(tokens[0], "extern") != NULL){//row 0�� Ÿ�� 2�̰ų� extern�� �ִ°��
		for(i = 1; i < TOKEN_CNT; i++){
			if(strcmp(tokens[i],"") == 0)//��ĭ�̹Ƿ� ����
				break;

			if(i != TOKEN_CNT -1 )
				strcat(tokens[0], " ");
			strcat(tokens[0], tokens[i]);//
			memset(tokens[i], 0, sizeof(tokens[i]));//token[i]�� 0���� �ʱ�ȭ
			//token[0]�� ��ūȭ ��Ų ������ space�� ���н��� ����
		}
	}


	while((p_str = find_typeSpecifier(tokens)) != -1){//��ū�� ����ȯ�� �ִ��� üũ
		if(!reset_tokens(p_str, tokens))
			return false;
	}


	while((p_str = find_typeSpecifier2(tokens)) != -1){//��ū�� ����ü���� ã��
		if(!reset_tokens(p_str, tokens))
			return false;
	}

	return true;
}

node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses)
{//��ū���� Ʈ���� ����� �Լ�
	node *cur = root;
	node *new;
	node *saved_operator;
	node *operator;
	int fstart;
	int i;

	while(1)
	{
		if(strcmp(tokens[*idx], "") == 0)//idx��ġ�� ��ū�� �������� Ż��
			break;

		if(!strcmp(tokens[*idx], ")"))
			return get_root(cur);//idx��ġ�� ��ū�� )�ϰ�� root�� ������

		else if(!strcmp(tokens[*idx], ","))
			return get_root(cur);//,�� ��� root�� ������

		else if(!strcmp(tokens[*idx], "("))
		{//(�� ���

			if(*idx > 0 && !is_operator(tokens[*idx - 1]) && strcmp(tokens[*idx - 1], ",") != 0){//idx�� 0���� ũ�� idx-1��°�� ���۷����Ͱ� �ƴϰ� ,�� �ƴҰ��

				fstart = true;//fstart�� true�� ����

				while(1)
				{
					*idx += 1;

					if(!strcmp(tokens[*idx], ")"))
						break;//��ū�� )�� Ż��

					new = make_tree(NULL, tokens, idx, parentheses + 1);//new�� ���ο� Ʈ�� ����

					if(new != NULL){//new�� NULL�� �ƴҰ��
						if(fstart == true){
							cur->child_head = new;
							new->parent = cur;//cur�� �ڽĿ� new�Ҵ��ϰ� new�� �θ� cur�� ����

							fstart = false;
						}
						else{
							cur->next = new;//cur�� ������带 new�� ����
							new->prev = cur;
						}

						cur = new;//new���� �̵�
					}

					if(!strcmp(tokens[*idx], ")"))
						break;//��ū�� )�� Ż��
				}
			}
			else{
				*idx += 1;

				new = make_tree(NULL, tokens, idx, parentheses + 1);//new�� ���θ��� Ʈ���� ����

				if(cur == NULL)
					cur = new;//cur�� NULL�� ��� ���θ��� Ʈ������

				else if(!strcmp(new->name, cur->name)){//new�� cur�� name�� �������
					if(!strcmp(new->name, "|") || !strcmp(new->name, "||")
						|| !strcmp(new->name, "&") || !strcmp(new->name, "&&"))
					{//new�� name�� | || & &&�ΰ��
						cur = get_last_child(cur);//cur�� �� �Ʒ� �ڽ����� �ű�

						if(new->child_head != NULL){//new�� �ڽ��� NULL�� �ƴѰ��
							new = new->child_head;

							new->parent->child_head = NULL;
							new->parent = NULL;
							new->prev = cur;
							cur->next = new;
						}//new�� �ڽ����� �̵��� �� new�� �θ��� �ڽ��� null new�� �θ� null new�� ������ cur�� ����
					}
					else if(!strcmp(new->name, "+") || !strcmp(new->name, "*"))
					{//name�� + *�ΰ��
						i = 0;

						while(1)
						{
							if(!strcmp(tokens[*idx + i], ""))
								break;//��ĭ�̸� Ż��

							if(is_operator(tokens[*idx + i]) && strcmp(tokens[*idx + i], ")") != 0)//���۷����� �ε� )�� �ƴҰ�� Ż��
								break;

							i++;
						}

						if(get_precedence(tokens[*idx + i]) < get_precedence(new->name))
						{//��ū�� i��°���� new�� precedence�� �������
							cur = get_last_child(cur);
							cur->next = new;
							new->prev = cur;
							cur = new;
						}//cur�� cur�� ������ �ڽ��� ������ cur�� next�� new�� �����ϰ� new�� �̵�
						else
						{
							cur = get_last_child(cur);//cur�� cur�� ������ �ڽ��� ����

							if(new->child_head != NULL){
								new = new->child_head;

								new->parent->child_head = NULL;
								new->parent = NULL;
								new->prev = cur;
								cur->next = new;
							}//new�� �ڽ��� ������� new�� cur�� next�� �̵���Ŵ
						}
					}
					else{//cur�� ���� ������ �ڽĿ� next�� new�� �ű�� new�� �̵�
						cur = get_last_child(cur);
						cur->next = new;
						new->prev = cur;
						cur = new;
					}
				}

				else
				{//cur�� �� ������ �ڽ��� ���ϰ� next�� new�� �������� new�� �̵�
					cur = get_last_child(cur);

					cur->next = new;
					new->prev = cur;

					cur = new;
				}
			}
		}
		else if(is_operator(tokens[*idx]))
		{//idx�� ���۷����� �ϰ��
			if(!strcmp(tokens[*idx], "||") || !strcmp(tokens[*idx], "&&")
					|| !strcmp(tokens[*idx], "|") || !strcmp(tokens[*idx], "&")
					|| !strcmp(tokens[*idx], "+") || !strcmp(tokens[*idx], "*"))
			{//idx�� || && | & + *�ΰ��
				if(is_operator(cur->name) == true && !strcmp(cur->name, tokens[*idx]))//name�� ���۷����Ͱ� idx�� name�� �������
					operator = cur;//���۷����͸� cur�� ����

				else
				{
					new = create_node(tokens[*idx], parentheses);//��ū���� ���ο� ��带 ����
					operator = get_most_high_precedence_node(cur, new);//���۷����Ϳ� new���� ������� �켱���� ��带 ����

					if(operator->parent == NULL && operator->prev == NULL){//���۷������� �θ� ���� ���� ��尡 �������

						if(get_precedence(operator->name) < get_precedence(new->name)){
							cur = insert_node(operator, new);
						}//new�� precedence�� �� ������ ���۷����Ϳ� new�� �����ϰ� cur�� ������ ���� �̵�

						else if(get_precedence(operator->name) > get_precedence(new->name))
						{//�����ִ� ���۷������� precedence�� �� �������
							if(operator->child_head != NULL){
								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}//���۷������� �ڽ��� ������ ���� �� ������ �ڽ����� �̵����� �� ��ġ�� �����ϰ� �� ���� �̵�
						}
						else
						{
							operator = cur;//���۷����͸� ���� ���� ����

							while(1)
							{
								if(is_operator(operator->name) == true && !strcmp(operator->name, tokens[*idx]))//name�� ���۷����Ͱ� idx�� �ٸ���� Ż��
									break;

								if(operator->prev != NULL)
									operator = operator->prev;//���۷����Ͱ� ���� ��尡 ������� �̵�
								else
									break;//������ Ż��
							}

							if(strcmp(operator->name, tokens[*idx]) != 0)//name�� idx�� �ٸ����
								operator = operator->parent;//�θ�� �̵�

							if(operator != NULL){//���۷����Ͱ� null�� �ƴѰ��
								if(!strcmp(operator->name, tokens[*idx]))//name�� idx�� �������
									cur = operator;//operator�� �̵�
							}
						}
					}

					else
						cur = insert_node(operator, new);//operator�� new�� �����ϰ� ����ġ�� �̵�
				}

			}
			else
			{
				new = create_node(tokens[*idx], parentheses);//idx�� ��� ����

				if(cur == NULL)
					cur = new;//cur�� NULL�̸� new�� �̵�

				else
				{
					operator = get_most_high_precedence_node(cur, new);//���۷����͸� new���� ���� ���� precedence�� ���� ���� �̵�

					if(operator->parentheses > new->parentheses)
						cur = insert_node(operator, new);//���۷������� parentheses�� ������� ��带 �����ϰ� cur�� �� ���� �̵�

					else if(operator->parent == NULL && operator->prev ==  NULL){//���۷������� parent�� prev�� NULL�� ���

						if(get_precedence(operator->name) > get_precedence(new->name))
						{//���۷������� precedence�� ���� ���
							if(operator->child_head != NULL){

								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}//�ڽĳ�尡 �ִ°�� ���۷������� ���� ������ �ڽ����� �̵��ϰ� new�� �����ϰ� �� ���� �̵�
						}

						else
							cur = insert_node(operator, new);//���۷����Ϳ� new�� �����ϰ� �̵�
					}

					else
						cur = insert_node(operator, new);//���۷����Ϳ� new�� �����ϰ� �̵�
				}
			}
		}
		else
		{
			new = create_node(tokens[*idx], parentheses);//idx�� ���ο� ��带 ����

			if(cur == NULL)
				cur = new;//cur�� NULL�� ��� new�� ����

			else if(cur->child_head == NULL){
				cur->child_head = new;
				new->parent = cur;

				cur = new;
			}//�ڽ� ��尡 ���°�� �ڽĳ�带 new�� �����ϰ� new�� �̵�
			else{

				cur = get_last_child(cur);

				cur->next = new;
				new->prev = cur;

				cur = new;
			}//cur�� ������ �ڽ����� �̵��ϰ� next�� new�� �����ѵ� �̵�
		}

		*idx += 1;//��ĭ �Ʒ��� �̵�
	}

	return get_root(cur);//���� Ʈ�� ��Ʈ ��� ��ȯ
}

node *change_sibling(node *parent)
{//�¿� ��� ��ȯ �Լ�
	node *tmp;

	tmp = parent->child_head;//tmp�� �ڽ� ���� ����

	parent->child_head = parent->child_head->next;//�ڽĳ�带 �ڽ��� next�� ����
	parent->child_head->parent = parent;//����� �ڽ��� next��忡 �θ� parent�� ����
	parent->child_head->prev = NULL;//prev�� NULL�� �ʱ�ȭ

	parent->child_head->next = tmp;//�ڽ��� next�� ���� �ڽĳ��� ����
	parent->child_head->next->prev = parent->child_head;//���� �ڽĳ���� prev�� ����� �ڽĳ��� ������
	parent->child_head->next->next = NULL;
	parent->child_head->next->parent = NULL;//���� �ڽĳ���� next�� parent�� null�� ����

	return parent;//����� parent ��ȯ
}

node *create_node(char *name, int parentheses)
{//��� ���� �Լ�
	node *new;

	new = (node *)malloc(sizeof(node));
	new->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(new->name, name);//���� ����� name�� ���� �Ҵ�

	new->parentheses = parentheses;
	new->parent = NULL;
	new->child_head = NULL;
	new->prev = NULL;
	new->next = NULL;//���ο� ����� ���ڵ� �ʱ�ȭ �� ��ȯ

	return new;
}

int get_precedence(char *op)
{//���۷��������� üũ �� ��ȯ �׿� �ش��ϴ� ���۷����� �� ��ȯ
	int i;

	for(i = 2; i < OPERATOR_CNT; i++){
		if(!strcmp(operators[i].operator, op))
			return operators[i].precedence;//��ġ�Ұ�� ��ġ�ϴ� ���۷������� precedence��ȯ
	}
	return false;
}

int is_operator(char *op)
{//���۷����� ���� üũ
	int i;

	for(i = 0; i < OPERATOR_CNT; i++)
	{
		if(operators[i].operator == NULL)
			break;//���۷����Ͱ� NULL�� ��� Ż��
		if(!strcmp(operators[i].operator, op)){
			return true;//���۷����Ϳ� ��ġ�ϴ°� �߰��Ͽ��� ��� true ��ȯ
		}
	}

	return false;
}

void print(node *cur)
{//��� ��� �Լ�
	if(cur->child_head != NULL){
		print(cur->child_head);
		printf("\n");
	}//����� �ڽ� ���

	if(cur->next != NULL){
		print(cur->next);
		printf("\t");
	}//���� ��� ���
	printf("%s", cur->name);//�ڱ� �ڽ� ���
}

node *get_operator(node *cur)
{//�� ���� ����� �θ� ��带 ��ȯ
	if(cur == NULL)
		return cur;//��尡 NULL�ϰ�� NULL ����

	if(cur->prev != NULL)//���� ��尡 �������
		while(cur->prev != NULL)
			cur = cur->prev;//���� ��尡 ������ ���� ��ȯ

	return cur->parent;//���� ����� �� ���� ����� �θ� ��ȯ
}

node *get_root(node *cur)
{//��Ʈ�� ������ �Լ�
	if(cur == NULL)//���� ���ڰ� NULL�� ��� �״�� ��ȯ
		return cur;

	while(cur->prev != NULL)
		cur = cur->prev;//���� ����� �ǿ��� ���� �̵�

	if(cur->parent != NULL)
		cur = get_root(cur->parent);//���� ����� �θ���� �̵�

	return cur;//������ ��Ʈ ��� ��ȯ
}

node *get_high_precedence_node(node *cur, node *new)
{//new���� ���� �켱���� ��� ��ȯ
	if(is_operator(cur->name))//cur�� ���۷����� �ΰ��
		if(get_precedence(cur->name) < get_precedence(new->name))
			return cur;//new�� ��������� �� �ʴ°�� cur��ȯ

	if(cur->prev != NULL){//cur�� ���� ��尡 null�� �ƴѰ��
		while(cur->prev != NULL){
			cur = cur->prev;

			return get_high_precedence_node(cur, new);//����� ���ʳ�尡 NULL�϶����� �Űܰ��� new�� üũ
		}


		if(cur->parent != NULL)//�θ� NULL�� �ƴѰ��
			return get_high_precedence_node(cur->parent, new);//�θ�� new�� �켱���� üũ
	}

	if(cur->parent == NULL)
		return cur;//�θ� NULL�� ��� cur��ȯ
}

node *get_most_high_precedence_node(node *cur, node *new)
{//���� ������ ��� ��ȯ
	node *operator = get_high_precedence_node(cur, new);//��ȯ�� �켱������ ���� ��� ����
	node *saved_operator = operator;

	while(1)
	{
		if(saved_operator->parent == NULL)//�θ� ���°�� Ż��
			break;

		if(saved_operator->prev != NULL)
			operator = get_high_precedence_node(saved_operator->prev, new);//prev�� �ִ°�� �켱������ ���� ��带 üũ�Ͽ� ����

		else if(saved_operator->parent != NULL)
			operator = get_high_precedence_node(saved_operator->parent, new);//�θ� �ִ°�� �θ��� �켱������ ���� ��带 üũ�Ͽ� ����

		saved_operator = operator;//�ӽ� ��忡 ��ȯ���� ��� ����
	}

	return saved_operator;
}

node *insert_node(node *old, node *new)
{//old��ġ�� new ����
	if(old->prev != NULL){//old�� prev�� null�� �ƴҰ��
		new->prev = old->prev;//new�� ���� ��带 old�� ���� ���� ����
		old->prev->next = new;//old���� ����� ���� ��带 new�� ����
		old->prev = NULL;//old�� ������带 NULL�� ����
	}

	new->child_head = old;//new�� �ڽ��� old�� ����
	old->parent = new;//old�� �θ� new�� ����

	return new;//new��� ��ȯ
}

node *get_last_child(node *cur)
{//������ �ڽ� ��� ��ȯ
	if(cur->child_head != NULL)
		cur = cur->child_head;//�ڽĳ�尡 ������� �ڽĳ��� �̵�

	while(cur->next != NULL)
		cur = cur->next;//�� ������ ���� �̵��� ��ȯ

	return cur;
}

int get_sibling_cnt(node *cur)
{//�¿��� ��� ���� ��
	int i = 0;

	while(cur->prev != NULL)
		cur = cur->prev;//��带 �� �������� �ű�

	while(cur->next != NULL){
		cur = cur->next;//��带 ���������� �ű�鼭 NULL�� ���������� ī��Ʈ ����
		i++;
	}

	return i;//ī��Ʈ ��ȯ
}

void free_node(node *cur)
{//��� ���� �Լ�
	if(cur->child_head != NULL)
		free_node(cur->child_head);//�ڽ� ��� ����

	if(cur->next != NULL)
		free_node(cur->next);//����� next ����

	if(cur != NULL){
		cur->prev = NULL;
		cur->next = NULL;
		cur->parent = NULL;
		cur->child_head = NULL;
		free(cur);//���� ���� �������� ���ְ� free
	}
}


int is_character(char c)
{//���ڷ� ���� ���� ���ڿ����� ��ȯ
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_typeStatement(char *str)
{//Ÿ���� ���¸� ��ȯ
	char *start;
	char str2[BUFLEN] = {0};
	char tmp[BUFLEN] = {0};
	char tmp2[BUFLEN] = {0};
	int i;

	start = str;//start�� ���޹��� ���ڷ� ����
	strncpy(str2,str,strlen(str));//str2�� str�� ���̸�ŭ str�� ����
	remove_space(str2);//str2�� ������ ����

	while(start[0] == ' ')
		start += 1;//start�� ���� ������ �ǳʶٱ� ���� ������ ������ ���������� start�� �����͸� ��ĭ�� �̵�

	if(strstr(str2, "gcc") != NULL)
	{//gcc�� ���ڿ��� str2���� �˻�
		strncpy(tmp2, start, strlen("gcc"));//tmp2�� gcc�� ���ڿ��� ���̸�ŭ start���� �����Ͽ� ����
		if(strcmp(tmp2,"gcc") != 0)//tmp2�� gcc�� �ƴҰ��
			return 0;//0 ����
		else
			return 2;//tmp2�� gcc�� ��� 2 ����
	}

	for(i = 0; i < DATATYPE_SIZE; i++)
	{//��� ������ Ÿ���� �˻�
		if(strstr(str2,datatype[i]) != NULL)
		{//str2���� ������ Ÿ���� ã�����
			strncpy(tmp, str2, strlen(datatype[i]));//tmp�� str2�� �߰��� ������Ÿ���� ���̸�ŭ ����
			strncpy(tmp2, start, strlen(datatype[i]));//tmp2�� start���� ������Ÿ���� ���̸�ŭ ����

			if(strcmp(tmp, datatype[i]) == 0)//���� tmp�� ������ Ÿ�԰� ��ġ�ϴ°��
				if(strcmp(tmp, tmp2) != 0)
					return 0;//tmp�� tmp2�� �ٸ���� ���� 0
				else
					return 2;//���� ��� ���� 2
		}

	}
	return 1;

}

int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN])
{//Ÿ�� ���� �Լ�1
	int i, j;

	for(i = 0; i < TOKEN_CNT; i++)
	{
		for(j = 0; j < DATATYPE_SIZE; j++)
		{//����ȯ üũ
			if(strstr(tokens[i], datatype[j]) != NULL && i > 0)
			{//������ Ÿ�԰� ��ġ�ϴ°� ��ū�� �ְ� i�� 0���� Ŭ���
				if(!strcmp(tokens[i - 1], "(") && !strcmp(tokens[i + 1], ")")//i-1�� (�̰� i+1�� )�̸� i+2�� & * ( ) - + �����ΰ�� i ����
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
{//Ÿ�� ���� �Լ� 2
    int i, j;


    for(i = 0; i < TOKEN_CNT; i++)
    {//����ü���� ã�����ؼ�
		if(!strcmp(tokens[i], "struct") && (i+1) <= TOKEN_CNT && is_character(tokens[i + 1][strlen(tokens[i + 1]) - 1]))
				return i;
    }
    return -1;
}

int all_star(char *str)
{//��� �ִ� ���� ���� *���� üũ
	int i;
	int length= strlen(str);

 	if(length == 0)//���̰� 0�� ��� 0����
		return 0;

	for(i = 0; i < length; i++)
		if(str[i] != '*')//str�� *�� �ƴѰ��� �ִ°�� 0 ����
			return 0;
	return 1;

}

int all_character(char *str)
{//���ڰ� �ִ��� üũ
	int i;

	for(i = 0; i < strlen(str); i++)
		if(is_character(str[i]))
			return 1;//���ڰ� ������� 1����
	return 0;

}

int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN])
{//��ū �ռ� �Լ�
	int i;
	int j = start - 1;
	int lcount = 0, rcount = 0;
	int sub_lcount = 0, sub_rcount = 0;

	if(start > -1){
		if(!strcmp(tokens[start], "struct")) {//��ū�� ����ü�� �ִ°��
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start+1]);//��ū�� �ڿ� space�� �����ϰ� start+1��°�� start �ڿ� ����

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);//��ū�� ��ĭ�� ���� �ű�
				memset(tokens[i + 1], 0, sizeof(tokens[0]));//i+1�� 0���� �ʱ�ȭ
			}
		}

		else if(!strcmp(tokens[start], "unsigned") && strcmp(tokens[start+1], ")") != 0) {//unsigned�� �̸� )�� start+1�� �ִ°��
			strcat(tokens[start], " ");
			strcat(tokens[start], tokens[start + 1]);
			strcat(tokens[start], tokens[start + 2]);//start�� space ������ start+1�� start+2�� ���������� start �ڿ� ����

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);
				memset(tokens[i + 1], 0, sizeof(tokens[0]));//��ū�� ��ĭ�� ���� �ű�� �ű� ������ĭ�� 0���� �ʱ�ȭ
			}
		}

     		j = start + 1;
        	while(!strcmp(tokens[j], ")")){//��ū�� j��ġ�� )�� �ִ°��
                	rcount ++;//���� ��ȣ ī��Ʈ ����
                	if(j==TOKEN_CNT)
                        	break;//j�� ��ū�� �ִ�ġ�� ���޽� Ż��
                	j++;//�ƴҶ� ���� �ݺ�
        	}

		j = start - 1;
		while(!strcmp(tokens[j], "(")){//��ū�� j��ġ�� (�� �ִ� ���
        	        lcount ++;//���� ��ȣ ī��Ʈ ����
                	if(j == 0)//0�̸� Ż��
                        	break;
               		j--;//j�� ��ĭ�� �������� �̵�
		}
		if( (j!=0 && is_character(tokens[j][strlen(tokens[j])-1]) ) || j==0)//j�� 0�� �ƴҶ� j�� ������ ���ڰ� �����̰ų� j�� 0�ΰ��
			lcount = rcount;//lcount�� rcount ����

		if(lcount != rcount )//���ʰ� ������ ��ȣ ���� �ٸ��Ƿ� false ����
			return false;

		if( (start - lcount) >0 && !strcmp(tokens[start - lcount - 1], "sizeof")){//start���� ���� ��ȣ�� ��ŭ �������� �̵���Ų�� 1���� ũ�� ��ȣ ����ĭ�� sizeof�� ������� true ��ȯ
			return true;
		}

		else if((!strcmp(tokens[start], "unsigned") || !strcmp(tokens[start], "struct")) && strcmp(tokens[start+1], ")")) {//unsigned�� struct�̰� �� �Ʒ�ĭ�� )�� �������
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcpy(tokens[start - lcount + 1], tokens[start + rcount]);//start - lcount�� start�� start+1�� ���̰� �� ����ĭ�� start+rcount�� ������ ����

			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount]);
				memset(tokens[i + lcount + rcount], 0, sizeof(tokens[0]));
			}//��ȣ ���� ������ ������ ������� ������ �ִ����� 0���� �ʱ�ȭ


		}
 		else{
			if(tokens[start + 2][0] == '('){//start+2�� (�� �����ϴ� ���
				j = start + 2;
				while(!strcmp(tokens[j], "(")){//(�� �������� �ȳ��ö����� ���� lcount����
					sub_lcount++;
					j++;
				}
				if(!strcmp(tokens[j + 1],")")){//�� ����ĭ�� ��ū��)�ΰ��
					j = j + 1;
					while(!strcmp(tokens[j], ")")){//)�� ���������� ������ ���������� ���� rcount����
						sub_rcount++;
						j++;
					}
				}
				else//�ƴѰ�� false ����
					return false;

				if(sub_lcount != sub_rcount)
					return false;//���� lcount�� rcount�� �ٸ���� false ����

				strcpy(tokens[start + 2], tokens[start + 2 + sub_lcount]);//start+2�� start +2 +sub_lcount�� ������ ����
				for(int i = start + 3; i<TOKEN_CNT; i++)
					memset(tokens[i], 0, sizeof(tokens[0]));//������ �Űܿ����Ƿ� �� ���� ���� �ʱ�ȭ

			}
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcat(tokens[start - lcount], tokens[start + rcount + 1]);//start-lcount�� start�� start+1�� start + rcount +1��°�� ���������� �ڿ� �ٿ�����

			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount -1; i++) {//�����͸� �ű�� �� ����ĭ�� 0���� �ʱ�ȭ
				strcpy(tokens[i], tokens[i + lcount + rcount +1]);
				memset(tokens[i + lcount + rcount + 1], 0, sizeof(tokens[0]));

			}
		}
	}
	return true;//true ��ȯ
}

void clear_tokens(char tokens[TOKEN_CNT][MINLEN])
{//��ū�� �ʱ�ȭ
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		memset(tokens[i], 0, sizeof(tokens[i]));//��ū�� ���� 0���� ä��
}

char *rtrim(char *_str)
{//������ ��������
	char tmp[BUFLEN];
	char *end;

	strcpy(tmp, _str);//tmp�� ���ڸ� ����
	end = tmp + strlen(tmp) - 1;
	while(end != _str && isspace(*end))
		--end;//end�� space�̰� ùĭ�� �ƴ� ��� ��ĭ ���ҽ�Ŵ

	*(end + 1) = '\0';//������ ���ڸ� �ι��ڷ� ����
	_str = tmp;
	return _str;
}

char *ltrim(char *_str)
{//���� ���� ����
	char *start = _str;

	while(*start != '\0' && isspace(*start))
		++start;//start�� �ι��ڰ� �ƴϰ� start�� �����ϰ�� start�� ����
	_str = start;
	return _str;
}

char* remove_extraspace(char *str)
{//������ ���ִ� �Լ�
	int i;
	char *str2 = (char*)malloc(sizeof(char) * BUFLEN);
	char *start, *end;
	char temp[BUFLEN] = "";
	int position;

	if(strstr(str,"include<")!=NULL){//str���� include<�� �˻�
		start = str;
		end = strpbrk(str, "<");//end�� <�� ������ �������� ����
		position = end - start;

		strncat(temp, str, position);
		strncat(temp, " ", 1);
		strncat(temp, str + position, strlen(str) - position + 1);//include�� <���� ���� ����

		str = temp;
	}

	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] ==' ')
		{//str[i]�� ������ ���
			if(i == 0 && str[0] ==' ')
				while(str[i + 1] == ' ')
					i++;//ùĭ�� space�̰� i�� 0�ΰ�� �������� ������ �ȳ��ö����� i����
			else{
				if(i > 0 && str[i - 1] != ' ')
					str2[strlen(str2)] = str[i];//��ĭ�� �����ϰ�� str2�� ������ĭ�� str[i]���� �ڿ� �ٿ�����
				while(str[i + 1] == ' ')
					i++;//�������� ������ �ȳ��ö����� i����
			}
		}
		else
			str2[strlen(str2)] = str[i];//str2�� ������ ĭ�� str[i]�� ����
	}

	return str2;//str2����
}



void remove_space(char *str)
{//������ �����ϴ� �Լ�
	char* i = str;
	char* j = str;

	while(*j != 0)//str[j]�� �ι��ڰ� �ƴҰ��
	{
		*i = *j++;//str[i]�� str[j]���� �ְ� j�� ���������� ��ĭ �ű�
		if(*i != ' ')
			i++;//str[i]�� space�� �ƴҰ�� i�� ���������� ��ĭ �̵�
	}
	*i = 0;//������ ���ڸ� �ι��ڷ� ����
}

int check_brackets(char *str)
{//��ȣ�� ������ �ִ��� üũ
	char *start = str;
	int lcount = 0, rcount = 0;

	while(1){
		if((start = strpbrk(start, "()")) != NULL){//start ���� ()�� �˻�
			if(*(start) == '(')//(�� ��� ���� ��ȣ�� �����Ƿ� lcount�� ����
				lcount++;
			else
				rcount++;//)�� ��� ������ ��ȣ�� �����Ƿ� rcount�� ����

			start += 1;//���� ��ȣ�� ã�� ���� ��ġ�� �̵�
		}
		else
			break;//��ȣ�� �����Ƿ� Ż��
	}

	if(lcount != rcount)
		return 0;//��ȣ�� ���� �������� ������ �ٸ��Ƿ� Ʋ��
	else
		return 1;//��ȣ�� ������ �����Ƿ� 1����
}

int get_token_cnt(char tokens[TOKEN_CNT][MINLEN])
{//��ū�� ������ ��ȯ
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		if(!strcmp(tokens[i], ""))//��ū�� i��°�� ��ĭ�ϰ�� ����
			break;

	return i;
}
