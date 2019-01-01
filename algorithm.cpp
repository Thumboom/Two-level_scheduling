#include"type.h"
#include"view.h"
#include<string.h>

jcb* job_fifs( ){
	jcb* p = jcb_ready->head; 

	return p;	
		
} 

//将作业放到后备队列上 
void put_in_jcb_ready(  ){
	jcb* cur = jcb_creating->head;
	jcb* next = NULL;
	printf("\nput_in_jcb_ready()\n") ;
	//按时间顺序模拟作业到达并加入到后备队列过程 
	while( cur != NULL )
	{
		if( cur->arrival_time != time)
			break;
		printf("\nput_in_jcb_ready() while1\n") ;
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
		printf("\nput_in_jcb_ready() while2\n") ;
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
	ppcb->num = pjcb->num;
	strncpy(ppcb->name, pjcb->name,strlen(pjcb->name));
	ppcb->arrival_time = time;
	ppcb->need_time = pjcb->need_time;
	ppcb->resource = pjcb->resource;
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
	printf("\nput_in_pcb_creating()\n") ;
	 //判断是否有足够内存分配----------------- 
	if(pjcb == NULL) return;
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
		
		
		 
		if(pjcb != NULL) 
		{
			//从后备队列中删除该作业，加入到运行队列中 
			if( pjcb == jcb_ready->end)
				jcb_ready->end = pjcb->link; 
			
			jcb_ready->head = pjcb->link;
			
			
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
			pjcb->link = NULL;
		}
		
		
	}
	output_pcb();
	
	 
}


void start_work( ){
	
	put_in_jcb_ready();
	put_in_pcb_creating();	

}

