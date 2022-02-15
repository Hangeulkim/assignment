#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_score.h"
#include "blank.h"

extern struct ssu_scoreTable score_table[QNUM];//문제들 점수 테이블
extern char id_table[SNUM][10];//학생들 id 테이블

struct ssu_scoreTable score_table[QNUM];//스코어 테이블을 가져옴
char id_table[SNUM][10];//학생들 id 테이블을 가져옴

char stuDir[BUFLEN];//학생 답 디렉토리
char ansDir[BUFLEN];//정답 디렉토리
char errorDir[BUFLEN];//에러 디렉토리
char threadFiles[ARGNUM][FILELEN];//t옵션을 사용하기 위한 배열
char iIDs[ARGNUM][FILELEN];//i옵션을 사용하기 위한 배열

int eOption = false;//e옵션 flag
int tOption = false;//t옵션 flag
int mOption = false;//m옵션 flag
int iOption = false;//i옵션 flag
int first_i = false;//i옵션이 처음으로 들어오는지 체크

void ssu_score(int argc, char *argv[])
{//메인함수
	int i;
	char saved_path[BUFLEN];//처음 작업 디렉토리 저장
	char *score="score_table.csv";

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-h")){//h옵션 사용시
			print_usage();//print usage를 실행하고 종료
			return;
		}
	}

	if(argc >= 3 && strcmp(argv[1], "-i") != 0){
		strcpy(stuDir, argv[1]);
		strcpy(ansDir, argv[2]);
	}//처음인자가 i옵션이 아닐경우 파일 경로가 있으므로 경로를 복사하여 전달

	if(!strcmp(argv[1],"-i"))//처음에 i인자를 줬으나 경로를 이상하게 줄 수 있으므로 따로 체크
		first_i=true;

	if(!check_option(argc, argv))//어떤 옵션으로 실행했는지 체크
		exit(1);

	if(first_i){//i옵션이 있을시 i옵션 실행
		do_iOption(iIDs);
		return;
	}

	getcwd(saved_path, BUFLEN);//처음 작업 디렉토리 복사

	if(chdir(stuDir) < 0){//학생 디렉토리가 있는지 체크
		fprintf(stderr, "ssu_score studir %s doesn't exist\n", stuDir);
		return;
	}
	if(getcwd(stuDir, BUFLEN)==NULL){
		fprintf(stderr,"can't use %s\n",stuDir);
		return;
	}//학생 디렉토리 작업 가능한지 체크

	chdir(saved_path);
	if(chdir(ansDir) < 0){//정답 디렉토리가 있는지 체크
		fprintf(stderr, "ssu_score ansdir %s doesn't exist\n", ansDir);
		return;
	}
	if(getcwd(ansDir, BUFLEN)==NULL){
		fprintf(stderr,"can't use %s\n",ansDir);
		return;
	}//정답 디렉토리 작업 가능한지 체크

	chdir(saved_path);

	set_scoreTable(ansDir);//스코어 테이블 작성
	set_idTable(stuDir);//id테이블 작성

	if(mOption)
		do_mOption(score);

	printf("grading student's test papers..\n");
	score_students();//학생 점수 매기기 실행

	if(iOption)//i옵션 플래그 체크
		do_iOption(iIDs);//i옵션 실행

	return;
}

int check_option(int argc, char *argv[])
{//어떤 옵션으로 실행했는지 체크
	int i, j;
	int c;//옵션명 저장

	while((c = getopt(argc, argv, "e:tmi")) != -1)
	{//e,m,t,i옵션 사용 가능
		switch(c){
			case 'e'://e옵션으로 실행
				eOption = true;//플래그 온
				strcpy(errorDir, optarg);//에러 디렉토리 경로 복사

				if(access(errorDir, F_OK) < 0)
					mkdir(errorDir, 0755);//디렉토리가 없을시 만듦
				else{
					rmdirs(errorDir);
					mkdir(errorDir, 0755);
				}//디렉토리가 있으므로 제거하고 새로만듦
				break;
			case 't'://t옵션으로 실행
				tOption = true;//t옵션플래그 온
				i = optind;//argv의 다음 위치를 확인하기 위해 사용
				j = 0;//넘겨줄 인자의 갯수 체크

				while(i < argc && argv[i][0] != '-'){//다른 옵션이 나오거나 끝까지 체크

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);//최대 멤버 변수 갯수를 넘음
					else
						strcpy(threadFiles[j], argv[i]);//t옵션 인자로 주기위해 복사
					i++;//다음 argv 체크
					j++;//넘겨줄 t옵션 인자의 갯수
				}
				break;
			case 'm'://m옵션 으로 실행했는지 체크
				mOption = true;//m옵션 플래그 온
				break;
			case 'i'://i옵션 으로 실행했는지 체크
				iOption = true;//i옵션 플래그 온
				i = optind;//argv의 다음 위치를 확인하기 위해 사용
				j = 0;//i옵션으로 사용하기위해 넘겨줄 인자의 갯수

				while(i < argc && argv[i][0] != '-'){//다음 인자가 옵션이거나 인자의 모든 걸 검사할때 까지 실행

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);//최대 인자의 갯수를 넘음
					else
						strcpy(iIDs[j], argv[i]);//i옵션을 주기위해 학생 번호를 i배열에 저장해줌
					i++;//다음 인자를 검사하기 위해 증가
					j++;//i배열 인자의 다음에 저장하기 위해 증가
				}
				break;
			case '?'://이상한 옵션이 들어왔을 경우
				printf("Unkown option %c\n", optopt);//이상한 옵션이 들어왔으므로 false 리턴
				return false;
		}
	}

	return true;//이상없이 모든 옵션을 구분했으므로 true 리턴
}


void do_iOption(char (*ids)[FILELEN])
{//i옵션을 사용하기 위한 함수
	FILE *fp;
	char tmp[BUFLEN];
	char save_qname[QNUM][BUFLEN];
	int i = 0;
	char *p, *saved;

	if((fp = fopen("score.csv", "r")) == NULL){//채점 결과 파일이 있는지 체크
		fprintf(stderr, "file open error for score.csv\n");//채점 파일이 없으므로 종료시킴
		return;
	}

	//문제 번호 배열에 저장
	fscanf(fp, "%s\n", tmp);//테이블에 첫 줄을 읽어옴
	p = strtok(tmp, ",");//첫 줄 첫번째 값을 생략시킴
	strcpy(save_qname[i++],p);//첫번째 데이터 삽입
	while((p = strtok(NULL, ",")) != NULL){//문자열의 끝까지 ,를 기준으로 자름
		if(strcmp(p,"sum"))
			strcpy(save_qname[i++],p);//차례대로 문제 이름 삽입
	}


	while(fscanf(fp, "%s\n", tmp) != EOF)
	{//테이블의 모든 row를 체크
		p = strtok(tmp, ",");//tmp에 있는 스트링에서 ,를 기준으로 자름

		if(!is_exist(ids, tmp))//일치하는게 인자로 들어온게 있는지 확인
			continue;//없을경우 다음껄로 넘어감

		printf("%s's wrong answer : \n", tmp);

		i=0;
		while((p = strtok(NULL, ",")) != NULL){//문자열의 끝까지 ,를 기준으로 자름
			if(!strcmp(p,"0")){//0점일 경우 문제번호 출력
				printf("%s ", save_qname[i]);
			}
			i++;
		}
		printf("\n");
	}
	fclose(fp);//검사를 끝냈으므로 종료
}

void do_mOption(char *filename)
{//m옵션 사용을 위한 함수
	char tmp[BUFLEN];
	char qname[FILELEN];
	int fd;
	int i;
	double new_score;
	int size=sizeof(score_table) / sizeof(score_table[0]);//테이블 최대 수

	if((fd = open(filename, O_RDWR | O_TRUNC)) < 0){//score_table.csv를 0666권한으로 OPEN
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	while(1){
		printf("Input question's number to modify >> ");
		scanf("%s",tmp);
		if(!strcmp(tmp,"no"))
			break;

		for(i = 0; i < size; i++)
		{//스코어 테이블에 입력받은 값이 있나 체크
			if(score_table[i].score == 0){
				printf("can't find %s\n",tmp);
				break;//입력받은 문제번호를 찾지 못함
			}

			strcpy(qname,score_table[i].qname);
			strcpy(qname,strtok(qname,"."));

			if(!strcmp(qname,tmp)){
				printf("Current score : %.2lf\n",score_table[i].score);//현재 점수 출력
				printf("New score : ");
				scanf("%lf",&new_score);//새로운 점수를 입력받음
				score_table[i].score=new_score;//점수 변경
				break;
			}
		}
	}
	write_scoreTable(filename);//점수를 변경하였으므로 테이블 다시 저장
}

int is_exist(char (*src)[FILELEN], char *target)
{//입력받은 인자에 존재하는지 확인
	int i = 0;

	while(1)
	{
		if(i >= ARGNUM)//최대 인자 수를 넘어갔을 경우 끝냄
			return false;
		else if(!strcmp(src[i], ""))//입력받은 인자를 이미 다 검사했을 경우 종료
			return false;
		else if(!strcmp(src[i++], target))//타겟과 일치하는 경우를 찾았을경우 true 리턴
			return true;
	}
	return false;
}

void set_scoreTable(char *ansDir)
{//스코어 테이블 설정 함수
	char filename[FILELEN];//파일명 저장 변수

	sprintf(filename, "%s", "score_table.csv");//스코어 테이블 이름 저장

	if(access(filename, F_OK) == 0)
		read_scoreTable(filename);//score table이 존재할경우 데이터를 읽어옴
	else{//스코어 테이블이 없을경우
		make_scoreTable(ansDir);//스코어 테이블을 만듦
		write_scoreTable(filename);//스코어 테이블을 작성
	}
}

void read_scoreTable(char *path)
{//스코어 테이블을 읽어오는 함수
	FILE *fp;//스코어 테이블을 저장할 포인터
	char qname[FILELEN];//임시로 문제명을 저장할 배열
	char score[BUFLEN];//스코어를 저장
	int idx = 0;

	if((fp = fopen(path, "r")) == NULL){//스코어 테이블을 읽을수 있는지 체크
		fprintf(stderr, "file open error for %s\n", path);//스코어 테이블을 여는데 실패
		return ;
	}

	while(fscanf(fp, "%[^,],%s\n", qname, score) != EOF){//스코어 테이블 정보를 끝까지 읽어옴
		strcpy(score_table[idx].qname, qname);//문제 이름
		score_table[idx++].score = atof(score);//문제의 스코어
	}

	fclose(fp);//스코어 파일을 다 읽어왔음으로 닫음
}

void make_scoreTable(char *ansDir)
{//스코어 테이블이 없으므로 만듦
	int type, num;
	double score, bscore, pscore;
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int idx = 0;
	int i;

	num = get_create_type();//어떤 옵션으로 스코어 테이블을 만들껀지 선언

	if(num == 1)
	{//1번 옵션으로 실행
		printf("Input value of blank question : ");//빈칸 문제 점수 입력
		scanf("%lf", &bscore);
		printf("Input value of program question : ");//프로그램 문제 점수 입력
		scanf("%lf", &pscore);
	}

	if((dp = opendir(ansDir)) == NULL){//정답 디렉토리를 열수 있는지 체크
		fprintf(stderr, "open dir error for %s\n", ansDir);//실패시 에러메시지
		return;
	}

	while((dirp = readdir(dp)) != NULL)
	{//디렉토리에 있는 파일을 하나씩 읽어옴
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;//. or ..일 경우 패스

		if((type = get_file_type(dirp->d_name)) < 0)//파일 타입을 가져오는데 실패했는지 체크
			continue;

		sprintf(score_table[idx++].qname, "%s", dirp->d_name);//tmp에 정답 파일 이름 저장
	}

	closedir(dp);//다 사용하였으므로 dp를 닫음
	sort_scoreTable(idx);//스코어 테이블 정렬

	for(i = 0; i < idx; i++)
	{//문제 전부 스코어를 할당
		type = get_file_type(score_table[i].qname);//빈칸 문제인지 프로그램 문제인지 구별

		if(num == 1)
		{
			if(type == TEXTFILE)
				score = bscore;//텍스트 파일일 경우 빈칸문제 점수 할당
			else if(type == CFILE)
				score = pscore;//c파일일 경우 프로그램 문제 점수 할당
		}
		else if(num == 2)
		{
			printf("Input of %s: ", score_table[i].qname);
			scanf("%lf", &score);//점수를 입력받아 할당
		}

		score_table[i].score = score;//i번째 문제 점수는 score로 설정
	}
}

void write_scoreTable(char *filename)
{//입력받은 점수로 스코어 테이블 작성
	int fd;
	char tmp[BUFLEN];
	int i;
	int num = sizeof(score_table) / sizeof(score_table[0]);//테이블 최대 수

	if((fd = open(filename, O_RDWR | O_TRUNC | O_CREAT,0666)) < 0){//score_table.csv를 0666권한으로 OPEN
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	for(i = 0; i < num; i++)
	{//스코어 테이블 전체 체크
		if(score_table[i].score == 0)
			break;

		sprintf(tmp, "%s,%.2f\n", score_table[i].qname, score_table[i].score);//테이블을 문제이름, 스코어로 작성
		write(fd, tmp, strlen(tmp));//파일에 tmp길이만큼 tmp를 작성 - 1 row씩 작성
	}

	close(fd);//파일 작성을 끝마쳤으므로 종료
}


void set_idTable(char *stuDir)
{//id테이블 세팅 함수
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int num = 0;

	if((dp = opendir(stuDir)) == NULL){//입력받은 학생 답 디렉토리를 오픈할수 있는지 체크
		fprintf(stderr, "opendir error for %s\n", stuDir);//오픈하는데 실패 했을 경우 종료
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){//dirp에 dp에 있는 파일을 전부 하나씩 불러옴
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;//. 과 ..은 무시

		sprintf(tmp, "%s/%s", stuDir, dirp->d_name);//stuDir에 있는 디렉토리 명을 tmp에 저장
		stat(tmp, &statbuf);//정보를 정상적으로 조회했는지 체크

		if(S_ISDIR(statbuf.st_mode))//받아온 파일이 디렉토리 인지 체크
			strcpy(id_table[num++], dirp->d_name);//받아온 파일의 디렉토리 명 저장
		else
			continue;
	}

	sort_idTable(num);//id테이블 솔팅
}

void sort_idTable(int size)
{//id테이블 솔팅 함수
	int i, j;
	char tmp[10];//아이디를 임시로 가지고 있을 배열

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 -i; j++){//id를 기준으로 오름차순으로 버블솔팅
			if(strcmp(id_table[j], id_table[j+1]) > 0){//앞에 id값이 뒤에 id값보다 클경우 스왑
				strcpy(tmp, id_table[j]);
				strcpy(id_table[j], id_table[j+1]);
				strcpy(id_table[j+1], tmp);
			}
		}
	}
}

void sort_scoreTable(int size)
{//스코어 테이블 정렬함수
	int i, j;
	struct ssu_scoreTable tmp;//임시로 값을 가지고 있을 구조체
	int num1_1, num1_2;
	int num2_1, num2_2;

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 - i; j++){//qname을 기준으로 오름차순 버블솔팅

			get_qname_number(score_table[j].qname, &num1_1, &num1_2);//비교할 첫번째 값 가져옴
			get_qname_number(score_table[j+1].qname, &num2_1, &num2_2);//비교할 두번째 값 가져옴


			if((num1_1 > num2_1) || ((num1_1 == num2_1) && (num1_2 > num2_2))){//앞에 숫자가 크거나 앞에숫자가 같은데 뒤에숫자가 클경우 스왑

				memcpy(&tmp, &score_table[j], sizeof(score_table[0]));
				memcpy(&score_table[j], &score_table[j+1], sizeof(score_table[0]));
				memcpy(&score_table[j+1], &tmp, sizeof(score_table[0]));
			}
		}
	}
}

void get_qname_number(char *qname, int *num1, int *num2)
{//qname을 가져오는 함수
	char *p;
	char dup[FILELEN];

	strncpy(dup, qname, strlen(qname));//qname의 길이만큼 qname을 dup에 복사해와 저장
	*num1 = atoi(strtok(dup, "-."));//-.중 앞에걸 찾아서 그 앞에걸 인트형으로 변환하여 저장

	p = strtok(NULL, "-.");//그 다음 -.을 찾아서 저장
	if(p == NULL)
		*num2 = 0;//찾은 값이 없을경우 0으로 저장
	else
		*num2 = atoi(p);//있을 경우 인트형으로 변환후 저장
}

int get_create_type()
{//스코어 테이블을 어떻게 만들지에 관한 함수
	int num;

	while(1)
	{
		printf("score_table.csv file doesn't exist in TREUDIR!\n");
		printf("1. input blank question and program question's score. ex) 0.5 1\n");//빈칸문제, 프로그램 문제에 대한 전체 스코어를 지정
		printf("2. input all question's score. ex) Input value of 1-1: 0.1\n");//문제별 스코어 지정
		printf("select type >> ");
		scanf(" %d", &num);

		if(num != 1 && num != 2)//주어진 옵션이 아닌 다른 경우를 입력했을시
			printf("not correct number!\n");
		else
			break;
	}

	return num;//입력 받은 값 반환
}

void score_students()
{//학생들한테 스코어를 매기는 함수
	int num;//반복문 변수
	int fd;//score.csv를 저장할 변수
	char tmp[BUFLEN];
	double score=0;
	int size = sizeof(id_table) / sizeof(id_table[0]);//최대 사이즈

	if((fd = creat("score.csv", 0666)) < 0){//0666권한으로 score.csv파일을 생성
		fprintf(stderr, "creat error for score.csv");//score 파일을 만드는데 실패
		return;
	}
	write_first_row(fd);//첫번째 row를 작성하는 함수

	for(num = 0; num < size; num++)
	{
		if(!strcmp(id_table[num], ""))
			break;//아이디 테이블이 비었을 경우 그 뒤는 없는것이므로 종료

		sprintf(tmp, "%s,", id_table[num]);//tmp에 학번,저장
		write(fd, tmp, strlen(tmp));//첫번째 칼럼에 들어가야 할 값이므로 tmp길이만큼 tmp를 테이블에 씀

		score+=score_student(fd, id_table[num]);//sum값과 문제에 대한 점수를 저장하기 위해 score_student 실행
	}

	printf("Total average : %.2f\n", score / num);
	close(fd);
}

double score_student(int fd, char *id)
{//학생 답안 채점 함수
	int type;
	double result;
	double score = 0;
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);//최대 사이즈

	for(i = 0; i < size ; i++)
	{
		if(score_table[i].score == 0)
			break;//스코어 테이블에 스코어가 없을경우 답안이 없는 것 이므로 종료

		sprintf(tmp, "%s/%s/%s", stuDir, id, score_table[i].qname);//학생 답안 디렉토리의 그 학번에 학생 답 경로를 tmp에 저장

		if(access(tmp, F_OK) < 0)//학생 답안에 접근 가능한지 체크
			result = false;//접근 실패했을 경우 false
		else
		{
			if((type = get_file_type(score_table[i].qname)) < 0)//정답 파일이 프로그램 문제인지 빈칸 문제인지 체크
				continue;//둘다 아니므로 넘김

			if(type == TEXTFILE)
				result = score_blank(id, score_table[i].qname);//텍스트 파일일 경우 빈칸 문제 채점 실행
			else if(type == CFILE)
				result = score_program(id, score_table[i].qname);//프로그램 문제일 경우 프로그램 채점 실행
		}

		if(result == false)//문제가 틀렸으므로 종료
			write(fd, "0,", 2);//0점 처리
		else{
			if(result == true){//정답 이므로 스코어만 더해줌
				score += score_table[i].score;//총점을 구하기 위해 score에 문제 스코어 합산
				sprintf(tmp, "%.2f,", score_table[i].score);//테이블에 쓰기위해 tmp에 스코어 복사
			}
			else if(result < 0){//문제를 맞았으나 정확하게 맞지 않았으므로 그만큼 제외하고 합산
				score = score + score_table[i].score + result;//총점을 구하기위해 스코어+현재 문제 점수+result로 계산
				sprintf(tmp, "%.2f,", score_table[i].score + result);//tmp에 문제스코어 + result값을 씀
			}
			write(fd, tmp, strlen(tmp));//fd에 tmp의 길이만큼 tmp 작성
		}
	}

	printf("%s is finished.. score : %.2f\n", id, score);//id 채점 종료

	sprintf(tmp, "%.2f\n", score);//총점을 쓰기위해 tmp 에 score 복사
	write(fd, tmp, strlen(tmp));//총점을 fd에 tmp길이만큼 tmp를 씀

	return score;
}

void write_first_row(int fd)
{//첫번째 row를 작성
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);

	write(fd, ",", 1);//처음 원소는 빈칸

	for(i = 0; i < size; i++){
		if(score_table[i].score == 0)
			break;//스코어가 0점일 경우 채점 파일이 없으므로 종료

		sprintf(tmp, "%s,", score_table[i].qname);//tmp에 스코어 테이블에 있는 정답 파일 이름과,를 저장
		write(fd, tmp, strlen(tmp));//파일에 tmp의 길이만큼 tmp를 씀
	}
	write(fd, "sum\n", 4);//총합과 다음 로우로 넘기기 위해 \n삽입
}

char *get_answer(int fd, char *result)
{//정답 가져오는 함수
	char c;
	int idx = 0;

	memset(result, 0, BUFLEN);//result를 BUFLEN만큼 0으로 채움
	while(read(fd, &c, 1) > 0)//fd에서 한글자 씩 EOF를 만나거나 에러가 발생할때 까지 읽어와서 c에 저장
	{
		if(c == ':')
			break;//c가 : 일경우 탈출

		result[idx++] = c;//한글자씩 result에 저장
	}
	if(result[strlen(result) - 1] == '\n')
		result[strlen(result) - 1] = '\0';//만약 '\n'일경우 널문자로 변경하여 끝을 알림

	return result;
}

int score_blank(char *id, char *filename)
{//빈칸 문제 채첨
	char tokens[TOKEN_CNT][MINLEN];
	node *std_root = NULL, *ans_root = NULL;
	int idx, start;
	char tmp[BUFLEN];//임시 스트링 저장 배열
	char s_answer[BUFLEN], a_answer[BUFLEN];
	char qname[FILELEN];
	int fd_std, fd_ans;
	int result = true;
	int has_semicolon = false;

	memset(qname, 0, sizeof(qname));//qname 변수의 사이즈만큼 0으로 설정
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname에 확장자의 길이 만큼 뺀 filename을 복사

	sprintf(tmp, "%s/%s/%s", stuDir, id, filename);//tmp에 학생디렉토리/id/파일명을 복사하여 저장
	fd_std = open(tmp, O_RDONLY);//fd_std에 학생 정답 파일을 읽기 권한으로 open
	strcpy(s_answer, get_answer(fd_std, s_answer));//학생 정답을 읽어와서 s_answer에 저장

	if(!strcmp(s_answer, "")){//정답을 ""와 비교하여 답이 없는경우 0점 처리
		close(fd_std);//열었던 학생답 파일을 닫고 false 리턴
		return false;
	}

	if(!check_brackets(s_answer)){//()를 체크
		close(fd_std);//괄호의 갯수가 다르므로 틀림 따라서 학생 답 파일을 닫고 false 리턴
		return false;
	}

	strcpy(s_answer, ltrim(rtrim(s_answer)));//s_answer의 왼쪽 오른쪽 공백을 제거하여 s_answer에 다시 저장

	if(s_answer[strlen(s_answer) - 1] == ';'){//학생 답의 맨 오른쪽 값이 ;일경우
		has_semicolon = true;//세미콜론 플래그 트루로 설정
		s_answer[strlen(s_answer) - 1] = '\0';//널 문자로 변경
	}

	if(!make_tokens(s_answer, tokens)){//s_answer을 토큰화
		close(fd_std);//실패했을 경우 fd_std를 닫고 false 리턴
		return false;
	}

	idx = 0;
	std_root = make_tree(std_root, tokens, &idx, 0);//tokens을 트리화

	sprintf(tmp, "%s/%s", ansDir, filename);
	fd_ans = open(tmp, O_RDONLY);//qname에 해당하는 정답 파일을 open

	while(1)
	{//:을 구분하여 따로따로 체크하기위해 반복시킴
		ans_root = NULL;
		result = true;

		for(idx = 0; idx < TOKEN_CNT; idx++)
			memset(tokens[idx], 0, sizeof(tokens[idx]));//정답을 토큰화 시키기위해 토큰 초기화

		strcpy(a_answer, get_answer(fd_ans, a_answer));//a_answer에 정답 복사

		if(!strcmp(a_answer, ""))//읽어올 답이 없을경우 탈출
			break;

		strcpy(a_answer, ltrim(rtrim(a_answer)));//a_answer에 왼쪽 오른쪽 공백 제거

		if(has_semicolon == false){
			if(a_answer[strlen(a_answer) -1] == ';')//has_semicolon이 false일때 정답의 마지막 글자가 ;인경우 다음 답으로
				continue;
		}

		else if(has_semicolon == true)
		{
			if(a_answer[strlen(a_answer) - 1] != ';')//has_semicolon이 true일때 정답의 마지막 글자가 ;가 아닌경우 다음 답으로
				continue;
			else
				a_answer[strlen(a_answer) - 1] = '\0';//;인 경우 널문자로 변경
		}

		if(!make_tokens(a_answer, tokens))
			continue;//a_answer을 토큰화 실패하였을 경우 다음 답으로 넘어감

		idx = 0;
		ans_root = make_tree(ans_root, tokens, &idx, 0);//토큰을 트리화 시킴

		compare_tree(std_root, ans_root, &result);//학생 답과 정답 트리를 비교

		if(result == true){//맞았을 경우
			close(fd_std);
			close(fd_ans);

			if(std_root != NULL)
				free_node(std_root);
			if(ans_root != NULL)
				free_node(ans_root);
			return true;//할당한 메모리를 해제하고 true 반환

		}
	}

	close(fd_std);
	close(fd_ans);

	if(std_root != NULL)
		free_node(std_root);
	if(ans_root != NULL)
		free_node(ans_root);

	return false;//할당한 메모리를 해제하고 false 반환
}

double score_program(char *id, char *filename)
{//프로그램 채점
	double compile;
	int result;

	compile = compile_program(id, filename);//학생 아이디에 해당하는 파일을 컴파일

	if(compile == ERROR || compile == false)
		return false;//컴파일 에러가 났을시 false 리턴

	result = execute_program(id, filename);//파일 실행한 결과를 받아옴

	if(!result)
		return false;//정답이 틀렸을 경우 false리턴

	if(compile < 0)
		return compile;//warning이 있을경우 넘겨받은값 리턴

	return true;
}

int is_thread(char *qname)
{//t옵션에 해당하는지 체크
	int i;
	int size = sizeof(threadFiles) / sizeof(threadFiles[0]);//최대 갯수

	for(i = 0; i < size; i++){
		if(!strcmp(threadFiles[i], qname))
			return true;//threadFiles에 qname이 있는지 검사 있을경우 true 리턴
	}
	return false;
}

double compile_program(char *id, char *filename)
{//프로그램 컴파일 함수
	int fd;
	char tmp_f[BUFLEN], tmp_e[BUFLEN];
	char command[BUFLEN];
	char qname[FILELEN];
	int isthread;
	off_t size;
	double result;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname을 초기화 시킨뒤 확장자를 제외한 문제이름을 qname에 복사

	isthread = is_thread(qname);//thread flag 체크

	sprintf(tmp_f, "%s/%s", ansDir, filename);//컴파일할 정답 파일 경로
	sprintf(tmp_e, "%s/%s.exe", ansDir, qname);//컴파일해서 나올 정답 파일 이름

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);//t옵션이 true일 경우
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);//t옵션이 false일 경우

	sprintf(tmp_e, "%s/%s_error.txt", ansDir, qname);
	fd = creat(tmp_e, 0666);//정답 파일의 에러 텍스트 생성

	redirection(command, fd, STDERR);//에러 체크
	size = lseek(fd, 0, SEEK_END);//에러 파일의 크기
	close(fd);
	unlink(tmp_e);//실행했던 파일을 닫음

	if(size > 0)
		return false;//에러가 났으므로 종료

	sprintf(tmp_f, "%s/%s/%s", stuDir, id, filename);//학생 답 파일명
	sprintf(tmp_e, "%s/%s/%s.stdexe", stuDir, id, qname);//학생 답 실행 파일

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);//t옵션이 있을때
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);//t옵션이 없을때

	sprintf(tmp_f, "%s/%s/%s_error.txt", stuDir, id, qname);
	fd = creat(tmp_f, 0666);//에러 파일 생성

	redirection(command, fd, STDERR);
	size = lseek(fd, 0, SEEK_END);//에러파일 있는지 체크
	close(fd);

	if(size > 0){//에러 파일이 있는경우
		if(eOption)
		{//e옵션 실행시
			sprintf(tmp_e, "%s/%s", errorDir, id);//에러 디렉토리 경로
			if(access(tmp_e, F_OK) < 0)
				mkdir(tmp_e, 0755);//0755권한으로 디렉토리 생성

			sprintf(tmp_e, "%s/%s/%s_error.txt", errorDir, id, qname);//에러 파일 경로 저장
			rename(tmp_f, tmp_e);//tmp_e를 tmp_f로 바꿈

			result = check_error_warning(tmp_e);//에러 체크
		}
		else{
			result = check_error_warning(tmp_f);//에러 체크
			unlink(tmp_f);//tmp_f파일 삭제
		}

		return result;//워닝 카운트와 에러가 있는지 리턴
	}

	unlink(tmp_f);//파일 삭제후 true 리턴
	return true;
}

double check_error_warning(char *filename)
{//error와 warning를 체크
	FILE *fp;
	char tmp[BUFLEN];
	double warning = 0;

	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", filename);
		return false;
	}//파일을 r권한으로 엶

	while(fscanf(fp, "%s", tmp) > 0){
		if(!strcmp(tmp, "error:"))
			return ERROR;//tmp가 error로 시작하는지 체크
		else if(!strcmp(tmp, "warning:"))
			warning += WARNING;//파일에서 읽어온 스트링에 warning이 있을때마다 카운트 증가
	}

	return warning;//워닝 카운트 리턴
}

int execute_program(char *id, char *filename)
{//프로그램 실행함
	char std_fname[BUFLEN], ans_fname[BUFLEN];
	char tmp[BUFLEN];
	char qname[FILELEN];
	time_t start, end;
	pid_t pid;
	int fd;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));//qname을 초기화 시키고 확장자를 제외한걸 복사

	sprintf(ans_fname, "%s/%s.stdout", ansDir, qname);
	fd = creat(ans_fname, 0666);//정답 실행 결과 저장할 파일 생성

	sprintf(tmp, "%s/%s.exe", ansDir, qname);//정답 실행 command 넘겨줌
	redirection(tmp, fd, STDOUT);//정답 실행 하여 결과 fd에 저장
	close(fd);

	sprintf(std_fname, "%s/%s/%s.stdout", stuDir, id, qname);
	fd = creat(std_fname, 0666);//학생 답 실행 결과 저장 파일 생성

	sprintf(tmp, "%s/%s/%s.stdexe &", stuDir, id, qname);//학생 실행 파일 명령어 저장

	start = time(NULL);//시작 시간 체크
	redirection(tmp, fd, STDOUT);

	sprintf(tmp, "%s.stdexe", qname);
	while((pid = inBackground(tmp)) > 0){//시간 체크를 하기위해 백그라운드에서 실행
		end = time(NULL);

		if(difftime(end, start) > OVER){//5초를 넘었을시 프로그램 강제 종료
			kill(pid, SIGKILL);
			close(fd);
			return false;
		}
	}

	close(fd);//파일 닫음

	return compare_resultfile(std_fname, ans_fname);//학생 답 실행파일과 정답 실행파일 비교 값 리턴
}

pid_t inBackground(char *name)
{//백그라운드에서 실행하는 함수
	pid_t pid;
	char command[64];
	char tmp[64];
	int fd;
	off_t size;

	memset(tmp, 0, sizeof(tmp));
	fd = open("background.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);//backgroun.txt를 읽기 쓰기, 제작, 있을경우 초기화 0666권한으로 open

	sprintf(command, "ps | grep %s", name);
	redirection(command, fd, STDOUT);//name이 들어간 프로세스 체크

	lseek(fd, 0, SEEK_SET);
	read(fd, tmp, sizeof(tmp));//tmp의 사이즈만큼 결과값을 읽어옴

	if(!strcmp(tmp, "")){
		unlink("background.txt");
		close(fd);
		return 0;
	}//tmp에 값이 없을경우 backgroun.txt를 지우고 파일을 닫음

	pid = atoi(strtok(tmp, " "));//공백을 찾아 자른뒤 인트로 변경
	close(fd);

	unlink("background.txt");
	return pid;//결과 값으로 찾은 pid 리턴
}

int compare_resultfile(char *file1, char *file2)
{//결과 파일 비교
	int fd1, fd2;
	char c1, c2;
	int len1, len2;

	fd1 = open(file1, O_RDONLY);//첫번째 파일을 읽기전용 권한으로 엶
	fd2 = open(file2, O_RDONLY);//두번째 파일을 읽기전용 권한으로 엶

	while(1)//파일의 결과를 끝까지 체크
	{
		while((len1 = read(fd1, &c1, 1)) > 0){
			if(c1 == ' ')
				continue;//빈칸은 건너 뜀
			else
				break;//아닐경우 비교하기위해 탈출
		}
		while((len2 = read(fd2, &c2, 1)) > 0){
			if(c2 == ' ')
				continue;//빈칸은 건너 뜀
			else
				break;//아닐경우 비교하기위해 탈출
		}

		if(len1 == 0 && len2 == 0)//둘 다 읽어오는 게 끝났을 경우 탈출
			break;

		to_lower_case(&c1);//file1에서 읽어온 문자를 소문자로 치환
		to_lower_case(&c2);//file1에서 읽어온 문자를 소문자로 치환

		if(c1 != c2){//답이 틀렸으므로 열었던 파일을 닫고 false 리턴
			close(fd1);
			close(fd2);
			return false;
		}
	}
	close(fd1);
	close(fd2);
	return true;//답이 일치하므로 열었던 파일을 닫고 true 리턴
}

void redirection(char *command, int new, int old)
{//재연결 함수
	int saved;

	saved = dup(old);//saved를 old로 사용
	dup2(new, old);//old를 new로 사용

	system(command);//command 실행

	dup2(saved, old);//old를 saved로 바꿔서 이전에 사용하던 old값으로 다시 가져옴
	close(saved);//saved사용을 끝냄
}

int get_file_type(char *filename)//파일 타입을 가져옴
{
	char *extension = strrchr(filename, '.');//마지막 .위치를 리턴받아 저장

	if(!strcmp(extension, ".txt"))
		return TEXTFILE;//.txt 파일일 경우 TEXTFILE(3) 리턴
	else if (!strcmp(extension, ".c"))
		return CFILE;//.c 파일일 경우 CFILE(4) 리턴
	else
		return -1;//둘다 아니므로 -1리턴
}

void rmdirs(const char *path)//디렉토리 삭제
{
	struct dirent *dirp;//readdir의 정보를 저장할 구조
	struct stat statbuf;//읽어온 파일의 상태 및 정보를 저장할 구조체
	DIR *dp;//지울 디렉토리 저장 DIR
	char tmp[BUFLEN];//파일의 경로를 임시 저장할 배열

	if((dp = opendir(path)) == NULL)
		return;//디렉토리를 가져오는것에 대한 예외처리

	while((dirp = readdir(dp)) != NULL)//디렉토리
	{
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))//.이거나 ..일경우 패스
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name);//tmp에 경로/파일명 저장

		if(lstat(tmp, &statbuf) == -1)//파일 정보를 가져오는데 예외처리
			continue;

		if(S_ISDIR(statbuf.st_mode))
			rmdirs(tmp);//디렉토리일경우 rmdirs로 삭제
		else
			unlink(tmp);//파일일 경우 unlink로 파일 삭제
	}

	closedir(dp);//열었던 디렉토리를 닫음
	rmdir(path);//path경로의 디렉토리 삭제
}

void to_lower_case(char *c)//대문자 소문자로 치환
{
	if(*c >= 'A' && *c <= 'Z')
		*c = *c + 32;
}

void print_usage()//-h옵션 사용시 실행
{
	printf("Usage : ssu_score <STUDENTDIR> <TRUEDIR> [OPTION]\n");
	printf("Option : \n");
	printf(" -m <IDS>          modify question's score\n");
	printf(" -e <DIRNAME>      print error on 'DIRNAME/ID/qname_error.txt' file \n");
	printf(" -t <QNAMES>       compile QNAME.C with -lpthread option\n");
	printf(" -i <IDS>          print student's wrong questions\n");
	printf(" -h                print usage\n");
}
