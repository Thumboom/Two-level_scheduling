#include"type.h"
#include"view.h"
#include"memAllocation.h"
#include<string.h>


jcb* pre_jcb = NULL;
int isRunning = 0;//是否有进程正在执行 
pcb* running_pcb = NULL;//当前正在执行的进程
int runtime = 0; //记录进程轮转法中某一进程已运行时间 

 
//先来先服务 
jcb* job_fcfs( ){
	jcb* p = jcb_ready->head; 
	pre_jcb = p;
	return p;	
}

//最短作业优先 
jcb* job_sjf(){
	jcb*p = jcb_ready->head;
	
	jcb* temp = jcb_ready->head;
	jcb* pre = NULL;
	pre_jcb = temp;
	
	while(temp != NULL)
	{
		if(temp->need_time < p->need_time)
		{
			p = temp;
			pre_jcb = pre;
		 } 
		 
		 pre = temp;
		 temp = temp->link;
	}
	
	return p; 
	
	
}

//高响应比优先 

jcb*  jcb_hrrn(){
	double higest = -1;
	double temp = 0;
	jcb* target = jcb_ready->head;
	jcb* cur = jcb_ready->head;
	jcb* pre; 
	
	pre_jcb = cur;
	pre = cur;
	if( cur == NULL )
		return NULL;
	higest = (time - target->arrival_time + target->need_time) * 1.0 
				/ target->need_time;
				
				
	while( cur != NULL)
	{
		temp = (time - cur->arrival_time + cur->need_time) * 1.0 
				/ cur->need_time;
				
		if( temp > higest)
		{
			pre_jcb = pre;
			target = cur;
			higest = temp;
		}
		pre = cur;
		cur = cur->link;
		
	}
	
	return target;	
	 
}
 

//为作业分配内存 
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
		case 1: p = job_fcfs(); break; 
		case 2: p = job_sjf();  break;
		case 3: p = jcb_hrrn(); break;
	}
	return p;
}

//根据作业新建进程信息 
pcb* new_pcb(jcb* pjcb) {
	pcb* ppcb;
	
	ppcb = (pcb*) malloc(pcb_size);
	ppcb->id = pjcb->id;
	//初始化char数组，不然会出现乱码 
	for(int i = 0; i < 10; i ++)
		ppcb->name[i] = '\0';
	
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
	 
	while( pjcb != NULL)
	{
		if( mem_allocation(pjcb) == ERROR) break;
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
		pjcb = job_schedule();
	}

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

//最短进程优先 
void pcb_spf(pcb* ppcb){
	pcb* pre = NULL;
	pcb* cur = NULL;
	
	ppcb->arrival_time = time; 
	
	if( pcb_ready->head == NULL )
	{
		
		pcb_ready->head = ppcb;
		pcb_ready->end = ppcb;
		ppcb->link = NULL;
		pcb_ready->num++;
		return;
	} 
	
	cur = pcb_ready->head;
	pre = cur;
	
	while(cur != NULL && cur->need_time <= ppcb->need_time)
	{
		pre = cur;
		cur = cur->link;
	}
	//添加到尾结点 
	if( cur == NULL )
	{
		pcb_ready->end->link = ppcb;
		pcb_ready->end = ppcb;
		ppcb->link = NULL;
	}
	else if( pre == cur)//添加到头结点 
	{
		ppcb->link = pcb_ready->head;
		pcb_ready->head = ppcb; 
	} 
	else //插入到中间
	{
		ppcb->link = cur;
		pre->link = ppcb;	
	}
	
	pcb_ready->num++;
	
}


//根据某种策略将放入pcb_ready 中的进程排序 
void put_in_ready(pcb* ppcb){
	switch(pcb_alg)
	{
		case 1: pcb_fcfs(ppcb)   ; break;	
		case 2: pcb_spf(ppcb)    ; break;
		case 3: pcb_fcfs(ppcb)	 ; break; 
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
		
				ppcb->arrival_time = time;
				
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
				output_pcb();
				
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
		
		printf("\n\n*********释放%d进程所占内存*********\n\n", pcb_id);
			
		show(); 
		
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



void RR(){
	
	if(isRunning == 1 && runtime < piece)
	{
		running_pcb->run_time ++;
		runtime ++;
		printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
		
		if( running_pcb->run_time == running_pcb->need_time)
		{
			isRunning = 0;
			//释放对应资源
			running_pcb->finish_time = time + 1;//1分钟末执行完即下一分钟初执行完 
			release(running_pcb);
			runtime = 0;
		
		
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
	 else if( isRunning == 1 && runtime == piece)
	 {
	 	//时间片完，将当前运行的进程挂到就绪队列末尾 
	 	runtime = 0;
	 	
	 	if(pcb_ready->end != NULL)
	 	{
	 		pcb_ready->end->link = running_pcb;
	 		pcb_ready->end = running_pcb;
	 		
		 }
		 else
		 {
		 	pcb_ready->head = running_pcb;
		 	pcb_ready->end = running_pcb;
		 	
		 }
		 
		 running_pcb->link = NULL;
		 running_pcb = NULL;
		 
		 isRunning = 0;
		 pcb_ready->num ++;
	 	
	  }
	  
	  if( isRunning == 0)
	  {
	  		
	  		running_pcb = pcb_ready->head; 
	  		if(running_pcb != NULL)
	  		{
	  		
			  	if(running_pcb->arrival_time == -1)
				  running_pcb->arrival_time = time;	
	  			pcb_ready->head = running_pcb->link;
	  			
	  			isRunning = 1;
	  			running_pcb->run_time ++;
	  			printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
				if(pcb_ready->num == 1)
				{
					pcb_ready->end = NULL;
				}
				
				pcb_ready->num --;
		  		runtime ++;	
		  		
		  		
		  	if( running_pcb->run_time == running_pcb->need_time)
			{
				isRunning = 0;
				//释放对应资源
				running_pcb->finish_time = time + 1;//1分钟末执行完即下一分钟初执行完 
				release(running_pcb);
				runtime = 0;
			
			
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
	  	
	   } 
 
	
	
	
}





void run_pcb(){
	
	if( pcb_alg == 3)
	{
		RR();
		return;
	}
	
	if(isRunning == 1)//有进程在执行 
	{
		running_pcb->run_time ++;
		printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
		if(running_pcb->run_time == running_pcb->need_time)
		{
			
			isRunning = 0;
			//释放对应资源
			running_pcb->finish_time = time + 1;//1分钟末执行完即下一分钟初执行完 
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
			running_pcb->run_time ++;
			printf("\n当前执行的进程%d, 所需时间%d，已执行时间%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
			if(pcb_ready->num == 1)
			{
				pcb_ready->end = NULL;
			}
			
			pcb_ready->num --;
			
			} 
		
	}
	
} 


//将完成的作业从运行中队列挂到已完成队列，释放相应的pcb 
void put_in_jcb_finish(){
	
	pcb* ppcb = pcb_finish->head;
	pcb* next_pcb = NULL;
	int id;
	jcb* pjcb = NULL;
	jcb* pre = NULL;
	jcb* next = NULL; 
	
	while(ppcb != NULL)
	{
		id = ppcb->id;
		pjcb = jcb_run->head;
		pre = pjcb;
		while( pjcb != NULL)
		{
			
			next = pjcb->link;
			if( pjcb->id == id)
			{
				pjcb->finish_time = ppcb->finish_time;
				
				if( jcb_finish->head == NULL)
				{
					jcb_finish->head = pjcb;
					jcb_finish->end = pjcb;
					
				}
				else
				{
					jcb_finish->end->link = pjcb;
					jcb_finish->end = pjcb;
				}
				
				jcb_finish->num ++;
				
				//从运行队列中删除 
				if( pjcb == jcb_run->end)
				{
					if(pre == pjcb)
						jcb_run->end = NULL;
					else
						jcb_run->end = pre;
						
				}
				
				if( jcb_run->head == pjcb)
				{
					jcb_run->head = pjcb->link;
					pre = jcb_run->head;
				}
				else
				{
					pre->link = pjcb->link;
				}
				
				jcb_run->num--;
				pjcb->link = NULL;
				pjcb = next;
				
			
				
				
			}
			else
			{
				pre = pjcb;
				pjcb = next;
			}
		
		}
		
		next_pcb = ppcb->link;
		
		pcb_finish->head = next_pcb;
		if(next_pcb == pcb_finish->end)
		{
			pcb_finish->end = next_pcb;
		} 
		else if(next_pcb == NULL)
		{
			pcb_finish->end = NULL;
		}
		
		pcb_finish->num --;
		//释放对应进程空间
		free(ppcb);
		ppcb = next_pcb;
		
	}
	 
}


void start_work( ){
	
	put_in_jcb_ready();
	put_in_pcb_creating();	
	put_in_pcb_ready();
//	output_pcb();
	run_pcb();
	put_in_jcb_finish();
}

