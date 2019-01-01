#include"type.h"
#include"algorithm.h"
int jcb_alg = 1;//选择作业调度算法 
int pcb_alg = 1;//选择进程调度算法 
int allc_alg = 1;//选择内存分配算法 

int num;//作业数量 
int f_num = 0; //已完成作业数 
int time;
struct jcb *p_jcb = NULL;
//jcb_creating 用来保存预输入的作业信息 
struct jcb_list *jcb_creating = NULL , *jcb_ready = NULL, *jcb_finish = NULL,*jcb_run = NULL;
			 
struct pcb *p_pcb = NULL;
struct pcb_list *pcb_creating = NULL, *pcb_ready, *pcb_finish = NULL;

DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点
void input_jcb();

void input_choice(){
	printf("************************************\n");
	printf("      多道批处理两级调度的模拟      \n");
	printf("************************************\n");
	printf("请选择作业调度算法：\n");
	printf("** 1)先来先服务算法  2)最小作业优先算法 **\n");
	scanf("%d", &jcb_alg);
	
	printf("请选择内存分配算法：\n");
	printf("** 1)首次适应算法    2)最佳优先算法 **\n");
	scanf("%d", &allc_alg); 
	
	printf("请选择进程调度算法：\n");
	printf("** 1)先来先服务算法  2)最小进程优先算法 **\n");
	scanf("%d", &pcb_alg); 
	input_jcb();
}

void init(){
	jcb_creating = (jcb_list*)malloc(jcb_list_size);
	jcb_creating->head = NULL;
	
	jcb_ready = (jcb_list*)malloc(jcb_list_size);
	jcb_ready->head = NULL;
	
	jcb_finish = (jcb_list*)malloc(jcb_list_size);
	jcb_finish->head = NULL;
	
	jcb_run = (jcb_list*)malloc(jcb_list_size);
	jcb_run->head = NULL;
	
	pcb_creating = (pcb_list*)malloc(pcb_list_size);
	pcb_creating->head = NULL;
	
	pcb_finish = (pcb_list*)malloc(pcb_list_size);
	pcb_finish->head = NULL;
	
	pcb_ready = (pcb_list*)malloc(pcb_list_size);
	pcb_ready->head = NULL;
	
	
	
//	printf("jcb_creating ->head%d", &jcb_creating);
}

void input_jcb(){
	
	printf("\n请输入作业数量: ");
	scanf("%d", & num);
	
	
	jcb *temp = NULL;
	
	
	printf("\n请分别输入作业到达时间，所需运行时间， 作业所需内存(KB)， 所需磁带机数量，作业名，\n");
	for(int i = 0; i < num; i ++)
	{
		
		p_jcb = (struct jcb*)malloc(jcb_size);
	//	scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,
	//							&p_jcb->resource, &p_jcb->name );
		scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,&p_jcb->size,
								&p_jcb->resource,&p_jcb->name);
		
		p_jcb->num = i;
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
			
		
	}
	
	 
}

void output_pcb(){
	p_pcb = pcb_creating->head;
			printf("\n进程编号 | 进程名 | 进程到达时间 | 所需运行时间 | 所需磁带机数量 | \n");
	while(p_pcb != NULL)
	{
		printf("\n  %d        %s         %d            %d              %d \n", 
								p_pcb->num, p_pcb->name,  p_pcb->arrival_time,p_pcb->need_time, 
							p_pcb->resource );
		p_pcb = p_pcb->link;
	 } 
	
	
}




void outputAll(){	
	
	p_jcb = jcb_ready->head;
	printf("\n作业编号 | 作业名 | 作业到达时间 | 所需运行时间 | 作业所需内存(KB) | 所需磁带机数量 | \n");
	if(jcb_ready->head != NULL) 
	{
		while(p_jcb != NULL)
		{
			printf("\n  %d        %s         %d            %d              %d               %d \n", 
									p_jcb->num, p_jcb->name,  p_jcb->arrival_time,p_jcb->need_time, p_jcb->size,
								p_jcb->resource );
			p_jcb = p_jcb->link;
		 } 
	} 
}


void timer(){
	
	
	
	for(time = jcb_creating->head->arrival_time; f_num < num ; time++)
	{
		//转换单位 
		if( time % 100 == 60 )
		{
			time = time / 100 * 100 + 100;
		}
		
		printf("time == %d\n", time); 
		
		start_work();
		 
		if(jcb_creating->head == NULL)
			break;
		
	
		
	}
	
	
}

int main(){
	//outputAll();
	init();
	input_choice();

	timer();
	outputAll();
	return 0;
}


