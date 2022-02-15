#include "alloc.h"

plist dat;

int init_alloc(){
    memset(&dat,0,sizeof(plist));//메모리 공간 설정
    dat.fd=open("tmpfile",O_RDWR|O_CREAT|O_TRUNC,0644);
    write(dat.fd,"0",1);
    if((dat.mem=(char*)mmap(0, PAGESIZE, PROT_WRITE | PROT_READ, MAP_SHARED, dat.fd, 0))==MAP_FAILED)//맵핑 실패
        return 1;
    dat.list[0].flag=0;
    dat.list[0].start=dat.mem;
    dat.list[0].end=dat.mem+PAGESIZE;
    dat.list[0].sz=PAGESIZE;
    dat.cnt=1;

    return 0;
}

int cleanup(){
    close(dat.fd);
    remove("tmpfile");
    memset(&dat,0,sizeof(plist));//데이터 초기화
    return munmap(dat.mem,PAGESIZE);//맵핑 해제
}

char *alloc(int sz){
    for(int i=0;i<dat.cnt;i++){
        if(!dat.list[i].flag&&dat.list[i].sz>=sz){
            if(dat.list[i].sz==sz){//요구 sz와 남은 공간이 일치하는 경우
                dat.list[i].flag=1;
                return dat.list[i].start;
            }

            for(int j=dat.cnt;j>=i+1;j--){//데이터를 한칸씩 뒤로 미룸
                dat.list[j].flag=dat.list[j-1].flag;
                dat.list[j].start=dat.list[j-1].start;
                dat.list[j].end=dat.list[j-1].end;
                dat.list[j].sz=dat.list[j-1].sz;
            }
            dat.cnt++;
            dat.list[i+1].flag=0;//현재칸과 다음칸 데이터를 설정
            dat.list[i].flag=1;

            dat.list[i+1].end=dat.list[i].end;
            dat.list[i].end=dat.list[i].start+sz;

            dat.list[i+1].sz=dat.list[i].sz-sz;
            dat.list[i].sz=sz;

            dat.list[i+1].start=dat.list[i].end;

            return dat.list[i].start;
        }
    }
    return NULL;
}

void dealloc(char *input){
    for(int i=0;i<dat.cnt;i++){
        if(dat.list[i].start==input){
            dat.list[i].flag=0;
            while(1){//메모리 병합 과정
                int fflag=1;//변화가 있는지 체크
                for(int k=0;k<dat.cnt-1;k++){
                    if(!dat.list[k].flag&&!dat.list[k+1].flag){//메모리를 병합해야 하는 경우
                        fflag=0;
                        dat.list[k].sz+=dat.list[k+1].sz;
                        dat.list[k].end=dat.list[k+1].end;
                        for(int j=k+1;j<dat.cnt;j++){//메모리를 한칸씩 당겨옴
                            dat.list[j].flag=dat.list[j+1].flag;
                            dat.list[j].start=dat.list[j+1].start;
                            dat.list[j].end=dat.list[j+1].end;
                            dat.list[j].sz=dat.list[j+1].sz;
                        }
                        dat.cnt--;
                        dat.list[dat.cnt].start=0;
                        dat.list[dat.cnt].end=0;
                        dat.list[dat.cnt].sz=0;
                        dat.list[dat.cnt].flag=0;
                        break;
                    }
                }
                if(fflag)
                    break;
            }
            break;
        }
    }
}
