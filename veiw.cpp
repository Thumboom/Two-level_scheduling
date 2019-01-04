#include"type.h"
#include"algorithm.h"
#include"memAllocation.h"
#include<string.h>
int jcb_alg = 1;//选择作业调度算法 
int pcb_alg = 1;//选择进程调度算法 
int allc_alg = 1;//选择内存分配算法 

int num = 5;//作业数量 
int f_num = 0; //已完成作业数 
int time;
int tape_num = 4;
int piece = 1;

struct jcb *p_jcb = NULL;
//jcb_creating 用来保存预输入的作业信息 
struct jcb_list *jcb_creating = NULL , *jcb_ready = NULL, *jcb_finish = NULL,*jcb_run = NULL;
			 
struct pcb *p_pcb = NULL;
struct pcb_list *pcb_creating = NULL, *pcb_ready, *pcb_finish = NULL;

struct tape_list* tapelist = NULL;


DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点
void input_jcb();

void input_choice(){
	printf("************************************\n");
	printf("      多道批处理两级调度的模拟      \n");
	printf("************************************\n");
	printf("请选择作业调度算法：\n");
	printf("** 1)先来先服务算法  2)最小作业优先算法 **\n");
	printf("** 3)高响应比优先算法                   **\n");
	scanf("%d", &jcb_alg);
	
	printf("请选择内存分配算法：\n");
	printf("** 1)首次适应算法    2)最佳优先算法 **\n");
	scanf("%d", &allc_alg); 
	
	printf("请选择进程调度算法：\n");
	printf("** 1)先来先服务算法  2)最小进程优先算法 **\n");
	printf("** 3)时间片轮转算法                     **\n");
	scanf("%d", &pcb_alg); 
	if(pcb_alg == 3)
	{
		printf("请输入时间片大小： ");
		scanf("%d", &piece);
		
	 } 
	
	
	input_jcb();
}

void init(){
	jcb_creating = (jcb_list*)malloc(jcb_list_size);
	jcb_creating->head = NULL;
	jcb_creating->num = 0; 
	
	jcb_ready = (jcb_list*)malloc(jcb_list_size);
	jcb_ready->head = NULL;
	jcb_ready->num = 0;
	
	jcb_finish = (jcb_list*)malloc(jcb_list_size);
	jcb_finish->head = NULL;
	jcb_finish->num = 0;
	
	
	jcb_run = (jcb_list*)malloc(jcb_list_size);
	jcb_run->head = NULL;
	jcb_run->num = 0;
	
	
	pcb_creating = (pcb_list*)malloc(pcb_list_size);
	pcb_creating->head = NULL;
	pcb_creating->num = 0;
	
	
	pcb_finish = (pcb_list*)malloc(pcb_list_size);
	pcb_finish->head = NULL;
	pcb_finish->num = 0;
	
	pcb_ready = (pcb_list*)malloc(pcb_list_size);
	pcb_ready->head = NULL;
	pcb_ready->num = 0;
	
	block_first = (DuLinkList)malloc(DuLinkList_size);
//	block_first->data = NULL;
	block_first->prior = NULL;
	block_first->next = NULL;
	Initblock();
	
	
	tapelist = (tape_list*) malloc(tape_list_size);
	tapelist->head = NULL;
	tapelist->end = NULL;
	tapelist->num = 0;
	
	
	tape* ptape = NULL;
	for(int i = 0; i < tape_num; i ++)
	{
		ptape = (tape*)malloc(tape_size);
		ptape->id = i;
		ptape->state = 0;
		ptape->link = NULL;
		if(tapelist->head == NULL)
		{
			tapelist->num ++;
			tapelist->head = ptape;
			tapelist->end = ptape;
		}
		else
		{
			tapelist->num ++; 
			tapelist->end->link = ptape;
			tapelist->end = ptape;
		}
		
	}
	
	
//	printf("jcb_creating ->head%d", &jcb_creating);
}


void initData(){
	
	jcb *temp = NULL;
	for(int i = 0; i < num; i ++)
	{
		
		p_jcb = (struct jcb*)malloc(jcb_size);
	
		if( i == 0 )
		{
			
			p_jcb->arrival_time = 1000;
			p_jcb->need_time = 25;
			p_jcb->size = 15;
			p_jcb->resource = 2;
			strcpy(p_jcb->name, "job1");
		}
		else if( i == 1 )
		{
			
			p_jcb->arrival_time = 1020;
			p_jcb->need_time = 30;
			p_jcb->size = 60;
			p_jcb->resource = 1;
			strcpy(p_jcb->name, "job2");
		}
		else if( i == 2 )
		{
			
			p_jcb->arrival_time = 1030;
			p_jcb->need_time = 10;
			p_jcb->size = 50;
			p_jcb->resource = 3;
			strcpy(p_jcb->name, "job3");
		}
		else if( i == 3 )
		{
			
			p_jcb->arrival_time = 1035;
			p_jcb->need_time = 20;
			p_jcb->size = 10;
			p_jcb->resource = 2;
			strcpy(p_jcb->name, "job4");
		}
		else if ( i == 4 )
		{
			
			p_jcb->arrival_time = 1040;
			p_jcb->need_time = 15;
			p_jcb->size = 30;
			p_jcb->resource = 2;
			strcpy(p_jcb->name, "job5");
		}
		p_jcb->id = i;
		p_jcb->link = NULL;
		
		if(jcb_creating->head == NULL)
		{
			jcb_creating->head = p_jcb;
			temp = p_jcb;
			
		} 
		else
		{
			temp->link = p_jcb;
			temp = temp->link;
		}
		
		jcb_creating->num ++;
			
		
	}
	
}

void input_jcb(){
	
	printf("\n请输入作业数量: ");
	scanf("%d", & num);
	
	
	jcb *temp = NULL;
	
	initData();
	return;
	
	
	printf("\n请分别输入作业到达时间，所需运行时间， 作业所需内存(KB)， 所需磁带机数量，作业名，\n");
	for(int i = 0; i < num; i ++)
	{
		
		p_jcb = (struct jcb*)malloc(jcb_size);
	//	scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,
	//							&p_jcb->resource, &p_jcb->name );
		scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,&p_jcb->size,
								&p_jcb->resource,&p_jcb->name);
		
		p_jcb->id = i;
		p_jcb->link = NULL;
		
		if(jcb_creating->head == NULL)
		{
			jcb_creating->head = p_jcb;
			temp = p_jcb;
			
		} 
		else
		{
			temp->link = p_jcb;
			temp = temp->link;
		}
		
		jcb_creating->num ++;
			
	}
	 
}

void output_pcb(){
	p_pcb = pcb_ready->head;
			printf("\n进程编号 | 进程名 | 进程到达时间 | 所需运行时间 | 所需磁带机数量 | \n");
	while(p_pcb != NULL)
	{
		printf("\n  %d        %s         %d            %d              %d \n", 
								p_pcb->id, p_pcb->name,  p_pcb->arrival_time,p_pcb->need_time, 
							p_pcb->resource );
		p_pcb = p_pcb->link;
	 } 
	
	
}

void outputJcb(){
	p_jcb = jcb_creating->head;
	printf("\n作业编号 | 作业名 | 作业到达时间 | 所需运行时间 | 作业所需内存(KB) | 所需磁带机数量 | \n");
	while(p_jcb != NULL)
	{
		printf("\n  %d        %s         %d            %d              %d               %d \n", 
								p_jcb->id, p_jcb->name,  p_jcb->arrival_time,p_jcb->need_time, p_jcb->size,
							p_jcb->resource );
		p_jcb = p_jcb->link;
	 } 
}


void outputAll(){	
	
	p_jcb = jcb_finish->head;
	int sum = 0;
	double avg = 0;
	
	
		printf("\n作业编号 | 作业名 | 作业到达时间 | 所需运行时间 | 调入内存时间 | 结束时间 | 作业所需内存(KB) | 所需磁带机数量 | \n");
	while(p_jcb != NULL)
	{
		printf("\n  %d        %s         %d            %d            %d            %d         %d            %d \n", 
								p_jcb->id, p_jcb->name,  p_jcb->arrival_time,p_jcb->need_time, p_jcb->start_time, 
								p_jcb->finish_time, p_jcb->size,p_jcb->resource );
								
		sum = sum + (p_jcb->finish_time - p_jcb->arrival_time);
		p_jcb = p_jcb->link;
		
	 } 
	 
	 avg = (sum * 1.0 )/ num;
	 printf("平均周转时间%f", avg);
}


void timer(){
	
	for(time = jcb_creating->head->arrival_time; jcb_finish->num < num ; time++)
	{
		//转换单位 
		if( time % 100 == 60 )
		{
			time = time / 100 * 100 + 100;
		}
		
		printf("-----------------%d-----------------\n", time); 
		
		start_work();
		 
	}
	
}

int main(){
	//outputAll();
	init();
	input_choice();

	timer();
//	output_pcb();
	outputAll();
	return 0;
}


