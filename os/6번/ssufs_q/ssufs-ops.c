#include "ssufs-ops.h"

extern struct filehandle_t file_handle_array[MAX_OPEN_FILES];

int ssufs_allocFileHandle() {
	for(int i = 0; i < MAX_OPEN_FILES; i++) {
		if (file_handle_array[i].inode_number == -1) {
			return i;
		}
	}
	return -1;
}

int ssufs_create(char *filename){
	/* 1 */
	struct inode_t *tmp = (struct inode_t *) malloc(sizeof(struct inode_t));
	if(open_namei(filename)!=-1) //파일이 존재하는지 확인
		return -1;

	int wrnode=ssufs_allocInode();
	if(wrnode==-1)
		return -1;

	ssufs_readInode(wrnode,tmp);
	strcpy(tmp->name,filename);//파일명 복사
	tmp->status=INODE_IN_USE;//status 변경
	ssufs_writeInode(wrnode,tmp);

	return wrnode;
}

void ssufs_delete(char *filename){
	/* 2 */
	if(filename,F_OK==-1) //파일이 존재하는지 확인
		return ;

	int fd=open_namei(filename);//파일이 열려있는지 확인
	if(fd!=-1){//열려 있는 경우
		for(int i=0; i<MAX_OPEN_FILES; i++){//파일 핸들러 없앰
			if(file_handle_array[i].inode_number==fd){
				ssufs_close(i);
				break;
			}
		}
		ssufs_freeInode(fd);
	}//열려 있는 경우 닫고 데이터 해제

	remove(filename);
}

int ssufs_open(char *filename){
	/* 3 */
	int fd=ssufs_allocFileHandle();//비어있는 file_handle을 찾음
	if(fd==-1)
		return -1;

	file_handle_array[fd].inode_number=open_namei(filename);//filename에 해당하는 inode를 설정
	file_handle_array[fd].offset=0;

	return fd;
}

void ssufs_close(int file_handle){
	file_handle_array[file_handle].inode_number = -1;
	file_handle_array[file_handle].offset = 0;
}

int ssufs_read(int file_handle, char *buf, int nbytes){
	/* 4 */
	int offset = file_handle_array[file_handle].offset;
	int block=offset/BLOCKSIZE, offs=offset%BLOCKSIZE;

	if(file_handle_array[file_handle].inode_number==-1)
		return -1;

	struct inode_t *tmp = (struct inode_t *) malloc(sizeof(struct inode_t));
	ssufs_readInode(file_handle_array[file_handle].inode_number, tmp);

	if(offset+nbytes>tmp->file_size||tmp->direct_blocks[block]==-1){//파일 사이즈를 초과하는 경우나 블럭이 없는데 읽으려고 하는 경우 에러
		free(tmp);
		return -1;
	}

	char tmpbuf[BLOCKSIZE];
	int read_bytes=0;
	while(read_bytes<nbytes){//모든 데이터를 읽어올 때까지 반복
		ssufs_readDataBlock(tmp->direct_blocks[block],tmpbuf);
		if(nbytes-read_bytes<BLOCKSIZE-offs){
			memcpy(&buf[read_bytes], &tmpbuf[offs], nbytes-read_bytes);
			read_bytes=nbytes;
		}

		else{
			memcpy(&buf[read_bytes], &tmpbuf[offs], BLOCKSIZE-offs);
			read_bytes+=BLOCKSIZE-offs;
		}
		block++;
		offs=0;
	}
	ssufs_lseek(file_handle,nbytes);
	free(tmp);

	return 0;
}

int ssufs_write(int file_handle, char *buf, int nbytes){
	/* 5 */
	int offset = file_handle_array[file_handle].offset;
	int inode = file_handle_array[file_handle].inode_number;

	if(file_handle_array[file_handle].inode_number==-1)
		return -1;

	struct inode_t *tmp = (struct inode_t *) malloc(sizeof(struct inode_t));
	ssufs_readInode(inode, tmp);


	if(offset+nbytes>BLOCKSIZE*MAX_FILE_SIZE){//최대 사이즈를 넘어가는 경우 에러
		free(tmp);
		return -1;
	}

	int block=offset/BLOCKSIZE, offs=offset%BLOCKSIZE;
	char white_block[BLOCKSIZE];//블럭을 초기화 시키기 위함
	memset(white_block,0,BLOCKSIZE);
	char tmpbuf[MAX_FILE_SIZE][BLOCKSIZE];
	int read_bytes=0;
	while(read_bytes<nbytes){//모든 데이터를 쓸때까지 반복
		if(tmp->direct_blocks[block]==-1){
			tmp->direct_blocks[block]=ssufs_allocDataBlock();
			if(tmp->direct_blocks[block]==-1)
				return -1;
			ssufs_writeDataBlock(tmp->direct_blocks[block],white_block);
		}
		ssufs_readDataBlock(tmp->direct_blocks[block],tmpbuf[block]);//기존의 데이터를 읽어옴
		if(nbytes-read_bytes<BLOCKSIZE-offs){
			memcpy(&tmpbuf[block][offs], &buf[read_bytes], nbytes-read_bytes);
			read_bytes=nbytes;
		}

		else{
			memcpy(&tmpbuf[block][offs], &buf[read_bytes], BLOCKSIZE-offs);
			read_bytes+=BLOCKSIZE-offs;
		}

		block++;
		offs=0;

	}
	for(int i=offset/BLOCKSIZE;i<block;i++){
		if(tmp->direct_blocks[i]==-1)
			break;
		ssufs_writeDataBlock(tmp->direct_blocks[i],tmpbuf[i]);//변경된 데이터를 씀
	}

	if(offset+nbytes>tmp->file_size)
		tmp->file_size=offset+nbytes;

	ssufs_writeInode(inode,tmp);
	ssufs_lseek(file_handle,nbytes);
	free(tmp);

	return 0;
}

int ssufs_lseek(int file_handle, int nseek){
	int offset = file_handle_array[file_handle].offset;

	struct inode_t *tmp = (struct inode_t *) malloc(sizeof(struct inode_t));
	ssufs_readInode(file_handle_array[file_handle].inode_number, tmp);

	int fsize = tmp->file_size;

	offset += nseek;

	if ((fsize == -1) || (offset < 0) || (offset > fsize)) {
		free(tmp);
		return -1;
	}

	file_handle_array[file_handle].offset = offset;
	free(tmp);

	return 0;
}
