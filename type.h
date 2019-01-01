#include<stdlib.h>
#include<stdio.h>

struct jcb{
	int num;
    char name[10];    //作业名 
    int arrival_time; //作业到达时间
    int start_time;   //作业开始调入内存时间 
    int need_time;      //作业运行所需时间 
    int finish_time;  //运行结束时间 
    int size;       //所需内存时间  
    int resource;		//所需磁带机数量 
    char state;		//作业状态 
    struct jcb * link;   //链接下一个jcb的指针 
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
	int num;
    char name[10];    //进程名 
    int arrival_time; //进程到达时间
    int start_time;   //进程开始时间 
    int need_time;      //进程运行所需时间 
    int finish_time;  //运行结束时间 
    int resource;   //所分配磁带数
	char state; 
    struct pcb * link;   //链接下一个pcb的指针 
};

typedef struct pcb PCB;    //定义结构体变量 

const int pcb_size = sizeof(struct pcb);

struct pcb_list {
	int num;
	struct pcb* head;
	struct pcb* end;
};

typedef struct pcb_list pcb_list;
const int pcb_list_size = sizeof(struct pcb_list);


typedef int Status;
 
typedef struct freearea//定义一个空闲区说明表结构
{
	int ID;   //分区号
	long size;   //分区大小
	long address; //分区地址
	int state;   //状态
}ElemType;
 
//----------  线性表的双向链表存储结构  ------------
typedef struct DuLNode //double linked list
{
	ElemType data; 
	struct DuLNode *prior; //前趋指针
	struct DuLNode *next;  //后继指针
}DuLNode,*DuLinkList;
 



