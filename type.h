#include<stdlib.h>
#include<stdio.h>

struct jcb{
	int id;
    char name[10];    //��ҵ�� 
    int arrival_time; //��ҵ����ʱ��
    int start_time;   //��ҵ��ʼ�����ڴ�ʱ�� 
    int need_time;      //��ҵ��������ʱ�� 
    int finish_time;  //���н���ʱ�� 
    int size;       //�����ڴ��С  
    int resource;		//����Ŵ������� 
    char state;		//��ҵ״̬ 
    struct jcb * link;   //������һ��jcb��ָ�� 
};
typedef struct jcb jcb; 
const int jcb_size = sizeof(struct jcb);
struct jcb_list {
	int num;
	struct jcb* head;
	struct jcb* end;
};

typedef struct jcb_list jcb_list;
const int jcb_list_size = sizeof(struct jcb_list);


struct pcb{
	int id;
    char name[10];    //������ 
    int arrival_time; //���̵���ʱ��
    int start_time;   //���̿�ʼʱ�� 
    int need_time;      //������������ʱ�� 
    int run_time;   //��ҵ������ʱ�� 
    int finish_time;  //���н���ʱ�� 
    int resource;   //������Ŵ���
	char state; 
    struct pcb * link;   //������һ��pcb��ָ�� 
};

typedef struct pcb PCB;    //����ṹ����� 

const int pcb_size = sizeof(struct pcb);

struct pcb_list {
	int num;
	struct pcb* head;
	struct pcb* end;
};

typedef struct pcb_list pcb_list;
const int pcb_list_size = sizeof(struct pcb_list);


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
 const int freearea_size = sizeof(struct freearea);
 const int  DuLinkList_size = sizeof(struct DuLNode);


struct tape{
	int id;//�Ŵ����� 
	int pcb_id;//�������̱�� 
	int state;
	struct tape* link;
};
typedef struct tape tape;
const int tape_size = sizeof(struct tape);

struct tape_list {
	int num;
	struct tape* head;
	struct tape* end;
};
typedef struct tape_list tape_list;
const int tape_list_size = sizeof(struct tape_list);
