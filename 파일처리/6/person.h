#ifndef	_PERSON_H_
#define	_PERSON_H_

#define RECORD_SIZE	100 // including 6 delimeters
#define PAGE_SIZE		256 // ���� ������

typedef struct _Person
{
	char sn[14];		//�ֹι�ȣ
	char name[18];		//�̸�
	char age[4];		//����
	char addr[22];	    	//�ּ�
	char phone[16];		//��ȭ��ȣ
	char email[26];		//�̸��� �ּ�
} Person;

#endif
