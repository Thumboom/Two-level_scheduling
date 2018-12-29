#include<stdlib.h>
#include<stdio.h>
struct jcb{
	int num;
    char name[10];    //作业名 
    int arrival_time; //作业到达时间
    int start_time;   //作业开始时间 
    int need_time;      //作业运行所需时间 
    int finish_time;  //运行结束时间 
    int size;       //所需内存时间  
    int resource;		//所需磁带机数量 
    char state;		//作业状态 
    struct jcb * link;   //链接下一个jcb的指针 
};
typedef struct jcb jcb; 
const int jcb_size = sizeof(struct jcb);

struct pcb{
    char name[10];    //进程名 
    int arrival_time; //进程到达时间
    int start_time;   //进程开始时间 
    int need_time;      //进程运行所需时间 
    int finish_time;  //运行结束时间 
    struct pcb * link;   //链接下一个pcb的指针 
};

typedef struct pcb PCB;    //定义结构体变量 

const int pcb_size = sizeof(struct pcb);
