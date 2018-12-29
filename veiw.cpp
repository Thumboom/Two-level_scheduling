#include"type.h"

int i = 100;
int jcb_alg = 1;
int pcb_alg = 1;
int allc_alg = 1;

struct jcb *p_jcb = NULL, *jcb_ready = NULL, *jcb_finish = NULL,*jcb_run = NULL;
struct pcb *p_pcb = NULL, *pcb_creating = NULL, *pcb_finish = NULL;



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

void input_jcb(){
	int num;
	printf("\n请输入作业数量: ");
	scanf("%d", & num);
	
	
	struct jcb *temp = NULL;
	
	
	printf("\n请分别输入作业到达时间，所需运行时间， 作业所需内存(KB)， 所需磁带机数量，作业名，\n");
	for(int i = 0; i < num; i ++)
	{
		
		p_jcb = (struct jcb*)malloc(jcb_size);
	//	scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,
	//							&p_jcb->resource, &p_jcb->name );
		scanf("%d%d%d%d%s",&p_jcb->arrival_time,&p_jcb->need_time,&p_jcb->size,
								&p_jcb->resource,&p_jcb->name);
		
		p_jcb->num = i;
		
		if(jcb_ready == NULL)
		{
			jcb_ready = p_jcb;
			temp = p_jcb;
		} else
		{
			temp->link = p_jcb;
			temp = temp->link;
		}
			
		
	}
	
	
	 
}


void outputAll(){	
	
	p_jcb = jcb_ready;
			printf("\n作业编号 | 作业名 | 作业到达时间 | 所需运行时间 | 作业所需内存(KB) | 所需磁带机数量 | \n");
	if(jcb_ready != NULL) 
	{
		while(p_jcb != NULL)
		{
			printf("\n  %d        %s         %d            %d              %d               %d \n", 
									p_jcb->num,p_jcb->name,  p_jcb->arrival_time,p_jcb->need_time,&p_jcb->size,
								p_jcb->resource );
			p_jcb = p_jcb->link;
		 } 
	} 
}

int main(){
	//outputAll();
	input_choice();
	outputAll();
	return 0;
}


