#include <stdio.h>
#include <time.h>
#define N 30		//20건 이상

typedef struct Borrow {
	int stnum;
	int booknum;
	time_t bortime;
	time_t rettime;
}borrow;

borrow caltime(borrow);	//반납일자계산

int main()
{	//학번,도서번호,대여일자순
    FILE *borfp=fopen("borrow.txt","w+");
	borrow borrow[N] = {
		{20180121,5462691,1542427200,0},
		{20187667,5543567,1542427250,0},
		{20187667,5429946,1542427260,0},
		{20187667,5316237,1542427200,0},
		{20187818,5511283,1542513620,0},
		{20187818,5683326,1542513640,0},
		{20187818,5504363,1542513660,0},
		{20187818,5489330,1542513680,0},
		{20181869,5454806,1542600010,0},
		{20181869,5753349,1542600023,0},
		{20189462,5410780,1542600185,0},
		{20189462,5726370,1542600243,0},
		{20182792,5647746,1542686382,0},
		{20182792,5280076,1542686785,0},
		{20182792,5716023,1542686999,0},
		{20183721,5504458,1542686452,0},
		{20183903,5504568,1542772800,0},
		{20183903,5418951,1542772832,0},
		{20183903,5720261,1542772888,0},
		{20183903,5656827,1542772982,0},

	};

	for (int i = 0; i < N; i++)//반납일자
	{
		borrow[i] = caltime(borrow[i]);
	}
    for(int i=0; i<20; i++) fprintf(borfp,"%d|%d|%ld|%ld\n",borrow[i].stnum,borrow[i].booknum,borrow[i].bortime,borrow[i].rettime);
	fclose(borfp);
	return 0;
}

borrow caltime(borrow st)
{
	struct tm *t;
	t = localtime(&st.bortime);
	if (t->tm_wday == 0)//일요일
	{
		st.rettime = st.bortime + 2678400;	//대출기간 31일
	}
	else
		st.rettime = st.bortime + 2592000;	//대출기간 30일
	return st;
}
