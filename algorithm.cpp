#include"type.h"
#include"view.h"
#include"memAllocation.h"
#include<string.h>


jcb* pre_jcb = NULL;
int isRunning = 0;//是否有进程正在执行 
pcb* running_pcb = NULL;//当前正在执行的进程
 

jcb* job_fifs( ){
	jcb* p = jcb_ready->head; 
	pre_jcb = p;
	return p;	
		
} 

Status mem_allocation(jcb* pjcb){
	return try_alloc(pjcb->id, pjcb->size);
}


//将作业放到后备队列上 
void put_in_jcb_ready(  ){
	jcb* cur = jcb_creating->head;
	jcb* next = NULL;
	//按时间顺序模拟作业到达并加入到后备队列过程 
	while( cur != NULL )
	{
		if( cur->arrival_time != time)
			break;
		if( jcb_ready->head == NULL)
		{
			jcb_ready->head = cur;
			jcb_ready->end = cur;
		} 
		else 
		{
			jcb_ready->end->link = cur;
			jcb_ready->end = cur;
		}
		
		next = cur->link;
		cur = cur->link;
		jcb_creating->head = cur;
		if(jcb_ready->end != NULL)
			jcb_ready->end->link = NULL;
			
		jcb_creating->num --;
		
		jcb_ready->num ++;
	 }
} 

//作业调度算法选择 
jcb* job_schedule(){
	jcb* p= NULL,*temp = NULL;
	
	switch(jcb_alg){
		case 1:p = job_fifs();break; 
		
	}
	return p;
}

//根据作业新建进程信息 
pcb* new_pcb(jcb* pjcb) {
	pcb* ppcb;
	
	ppcb = (pcb*) malloc(pcb_size);
	ppcb->id = pjcb->id;
	strncpy(ppcb->name, pjcb->name,strlen(pjcb->name));
	ppcb->arrival_time = -1;
	ppcb->need_time = pjcb->need_time;
	ppcb->resource = pjcb->resource;
	ppcb->run_time = 0; 
	ppcb->link = NULL;
	 
}

 
//将进程放到进程创建队列中 
void put_in_pcb_creating(){
	/*
	* 从作业后备队列选择一个作业，判断内存是否有足够空间，
	  如果有，则分配相应空间并创建进程加入到进程创建队列，从后备队列删除该作业
	  加入到正在运行队列 
	*/
	jcb* pjcb = NULL;
	pjcb = job_schedule();
	pcb* ppcb = NULL;
	 //判断是否有足够内存分配----------------- 
	if(pjcb == NULL || mem_allocation(pjcb) == ERROR) return;
	else 
	{
		ppcb = new_pcb(pjcb);
		
		if( pcb_creating->head == NULL)
		{
			pcb_creating->head = ppcb;
			pcb_creating->end = ppcb;
		}
		else
		{
			pcb_creating->end->link = ppcb;
			pcb_creating->end = ppcb;
		}
		 
		pcb_creating->num ++; 
		 
		 //从后备队列中删除该作业，加入到运行队列中 
		if(pjcb != NULL) 
		{
			pjcb->start_time = time;
			
			if( pjcb == jcb_ready->end)
			{
				//判断就绪队列中是否只有一个元素 
				if( pre_jcb == pjcb)
					jcb_ready->end = NULL;
				else 	
					jcb_ready->end = pre_jcb; 
			}
				
			
			if(jcb_ready->head == pjcb)
				jcb_ready->head = pjcb->link;
			else
			{
				pre_jcb->link = pjcb->link;
				
			}
			
				
			jcb_ready->num--;	
			
			if(jcb_run->head == NULL)
			{
				jcb_run->head = pjcb;
				jcb_run->end = pjcb;
			}	
			else
			{
				jcb_run->end->link = pjcb;
				jcb_run->end = pjcb;
				
			}
			jcb_run->num++;
			pjcb->link = NULL;
		}
		
	}
//	output_pcb();
	
}

//尝试分配磁带机 
Status try_alloc_tape(pcb* ppcb){
	
	if(ppcb->resource > tapelist->num)
	{
		printf("\n磁带机数量不足！\n");
		return ERROR; 
	}
	else
	{
		printf("\n成功分配%d道磁带机\n", ppcb->resource);
		
		tapelist->num = tapelist->num - ppcb->resource;
		tape* temp = tapelist->head;
		
		
		//寻找空闲的磁带机并分配 
		for( int i = 0; i < ppcb->resource; )
		{
		 	if( temp->state == 0)
		 	{
		 		temp->pcb_id = ppcb->id;
		 		temp->state = 1;
		 		i ++;
		 		printf("作业%d所分配道的磁带机号为%d\n",ppcb->id,temp->id);
			 }
			temp = temp->link;
		}
		return OK;
		
	}
	
}


void pcb_fcfs(pcb* ppcb){
	 
	if(pcb_ready->head == NULL)
	{
		pcb_ready->head = ppcb;
		pcb_ready->end = ppcb;
	}	
	else
	{
		
		pcb_ready->end->link = ppcb;
		pcb_ready->end = ppcb;
		
	}
	pcb_ready->num++;
	ppcb->link = NULL;
	
}

void pcb_spf(pcb* ppcb){
	
	
}


//根据某种策略将放入pcb_ready 中的进程排序 
void put_in_ready(pcb* ppcb){
	switch(pcb_alg)
	{
		case 1: pcb_fcfs(ppcb)   ; break;	
		case 2: pcb_spf(ppcb)    ; break;
	} 
}


void put_in_pcb_ready(){
	 //为pcb_creating中的进程分配资源
	//按顺序分配，先分配前一个在分配后一个
	pcb* ppcb = pcb_creating->head;
	pcb* pre = ppcb;
	pcb* next_pcb = NULL;
	int t;
	if(ppcb == NULL)
		return;
	
	t = ppcb->arrival_time;
	
	while( ppcb != NULL && ppcb->arrival_time <= t)
	{
		if(try_alloc_tape(ppcb) == OK)
		{
			//挂到就绪队列中，并从creating删除
		
				ppcb->start_time = time;
				
				if( ppcb == pcb_creating->end)
				{
					//判断就绪队列中是否只有一个元素 
					if( pre == ppcb)
						pcb_creating->end = NULL;
					else 	
						pcb_creating->end = pre; 
				}
					
				
				if(pcb_creating->head == ppcb)
				{
					pcb_creating->head = ppcb->link;
					pre = pcb_creating->head;					
				}
				else
				{
					pre->link = ppcb->link;
					
				}
					
				pcb_creating->num--;	
				
				next_pcb = ppcb->link;
//				ppcb->link = NULL;
				put_in_ready(ppcb);
				ppcb = next_pcb;
				
		} 
		else
		{
			pre = ppcb;
			ppcb = ppcb->link;
		}
	}
	
}


void release(pcb* ppcb){
	
	int pcb_id = ppcb->id;
	tape* ptape = NULL;
	int num = 0;
	if(free(pcb_id) == OK)
	{
		ptape = tapelist->head;
		while( num < ppcb->resource && ptape != NULL)
		{
			if( ptape->pcb_id == pcb_id)
			{
				
				printf("释放%d磁带机\n", ptape->id);
				ptape->pcb_id = -1;
				ptape->state = 0;
				tapelist->num++;
				num ++;
			}
			
			ptape = ptape->link;
			
		}
		
	}
	
}

void run_pcb(){
	
	if(isRunning == 1)//有进程在执行 
	{
		running_pcb->run_time ++;
		printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
		if(running_pcb->run_time == running_pcb->need_time)
		{
			
			isRunning = 0;
			//释放对应资源
			
			release(running_pcb);
			
			
			 //加入到已完成队列
			 if( pcb_finish->head == NULL)
			 {
			 	pcb_finish->head = running_pcb;
			 	pcb_finish->end = running_pcb;
			 	
			  } 
			  else
			  {
			  	pcb_finish->end->link = running_pcb;
			  	pcb_finish->end = running_pcb;
			  }
			  
			  pcb_finish->num++;
			  
			  running_pcb->link = NULL;
			  running_pcb = NULL;
			  
			  //-------
			if( pcb_ready->head != NULL)
			{
				running_pcb = pcb_ready->head;
				pcb_ready->head = running_pcb->link;
				
				if(pcb_ready->num == 1)
				{
					pcb_ready->end = NULL;
				}
				
				pcb_ready->num --;
				isRunning = 1;
			 }
			  
		}
			
			
			 
			
		
	}
	else
	{
		if( pcb_ready->head != NULL)
		{
			running_pcb = pcb_ready->head;
			pcb_ready->head = running_pcb->link;
			
			isRunning = 1;
			printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
			if(pcb_ready->num == 1)
			{
				pcb_ready->end = NULL;
			}
			
			pcb_ready->num --;
			
			running_pcb->run_time ++;
			
		 } 
		
	}
	
} 


void start_work( ){
	
	put_in_jcb_ready();
	put_in_pcb_creating();	
	put_in_pcb_ready();
//	output_pcb();
	run_pcb();
}

