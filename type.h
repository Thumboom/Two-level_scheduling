#include<stdlib.h>
#include<stdio.h>

struct jcb{
	int num;
    char name[10];    //��ҵ�� 
    int arrival_time; //��ҵ����ʱ��
    int start_time;   //��ҵ��ʼʱ�� 
    int need_time;      //��ҵ��������ʱ�� 
    int finish_time;  //���н���ʱ�� 
    int size;       //�����ڴ�ʱ��  
    int resource;		//����Ŵ������� 
    char state;		//��ҵ״̬ 
    struct jcb * link;   //������һ��jcb��ָ�� 
};
typedef struct jcb jcb; 
const int jcb_size = sizeof(struct jcb);

struct pcb{
    char name[10];    //������ 
    int arrival_time; //���̵���ʱ��
    int start_time;   //���̿�ʼʱ�� 
    int need_time;      //������������ʱ�� 
    int finish_time;  //���н���ʱ�� 
    struct pcb * link;   //������һ��pcb��ָ�� 
};

typedef struct pcb PCB;    //����ṹ����� 

const int pcb_size = sizeof(struct pcb);

typedef int Status;
 
typedef struct freearea//����һ��������˵����ṹ
{
	int ID;   //������
	long size;   //������С
	long address; //������ַ
	int state;   //״̬
}ElemType;
 
//----------  ���Ա��˫������洢�ṹ  ------------
typedef struct DuLNode //double linked list
{
	ElemType data; 
	struct DuLNode *prior; //ǰ��ָ��
	struct DuLNode *next;  //���ָ��
}DuLNode,*DuLinkList;
 
DuLinkList block_first; //ͷ���
DuLinkList block_last;  //β���

