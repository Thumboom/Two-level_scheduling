#include"type.h"
#include"view.h"
#include"memAllocation.h"
#include<string.h>


jcb* pre_jcb = NULL;
int isRunning = 0;//�Ƿ��н�������ִ�� 
pcb* running_pcb = NULL;//��ǰ����ִ�еĽ���
int runtime = 0; //��¼������ת����ĳһ����������ʱ�� 

 
//�����ȷ��� 
jcb* job_fcfs( ){
	jcb* p = jcb_ready->head; 
	pre_jcb = p;
	return p;	
}

//�����ҵ���� 
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

//����Ӧ������ 

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
 

//Ϊ��ҵ�����ڴ� 
Status mem_allocation(jcb* pjcb){
	return try_alloc(pjcb->id, pjcb->size);
}


//����ҵ�ŵ��󱸶����� 
void put_in_jcb_ready(  ){
	jcb* cur = jcb_creating->head;
	jcb* next = NULL;
	//��ʱ��˳��ģ����ҵ���ﲢ���뵽�󱸶��й��� 
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

//��ҵ�����㷨ѡ�� 
jcb* job_schedule(){
	jcb* p= NULL,*temp = NULL;
	
	switch(jcb_alg){
		case 1: p = job_fcfs(); break; 
		case 2: p = job_sjf();  break;
		case 3: p = jcb_hrrn(); break;
	}
	return p;
}

//������ҵ�½�������Ϣ 
pcb* new_pcb(jcb* pjcb) {
	pcb* ppcb;
	
	ppcb = (pcb*) malloc(pcb_size);
	ppcb->id = pjcb->id;
	//��ʼ��char���飬��Ȼ��������� 
	for(int i = 0; i < 10; i ++)
		ppcb->name[i] = '\0';
	
	strncpy(ppcb->name, pjcb->name,strlen(pjcb->name));
	ppcb->arrival_time = -1;
	ppcb->need_time = pjcb->need_time;
	ppcb->resource = pjcb->resource;
	ppcb->run_time = 0; 
	ppcb->link = NULL;
	 
}

 
//�����̷ŵ����̴��������� 
void put_in_pcb_creating(){
	/*
	* ����ҵ�󱸶���ѡ��һ����ҵ���ж��ڴ��Ƿ����㹻�ռ䣬
	  ����У��������Ӧ�ռ䲢�������̼��뵽���̴������У��Ӻ󱸶���ɾ������ҵ
	  ���뵽�������ж��� 
	*/
	jcb* pjcb = NULL;
	pjcb = job_schedule();
	pcb* ppcb = NULL;
	 //�ж��Ƿ����㹻�ڴ����----------------- 
	 
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
			 
			 //�Ӻ󱸶�����ɾ������ҵ�����뵽���ж����� 
			if(pjcb != NULL) 
			{
				pjcb->start_time = time;
				
				if( pjcb == jcb_ready->end)
				{
					//�жϾ����������Ƿ�ֻ��һ��Ԫ�� 
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

//���Է���Ŵ��� 
Status try_alloc_tape(pcb* ppcb){
	
	if(ppcb->resource > tapelist->num)
	{
		printf("\n�Ŵ����������㣡\n");
		return ERROR; 
	}
	else
	{
		printf("\n�ɹ�����%d���Ŵ���\n", ppcb->resource);
		
		tapelist->num = tapelist->num - ppcb->resource;
		tape* temp = tapelist->head;
		
		
		//Ѱ�ҿ��еĴŴ��������� 
		for( int i = 0; i < ppcb->resource; )
		{
		 	if( temp->state == 0)
		 	{
		 		temp->pcb_id = ppcb->id;
		 		temp->state = 1;
		 		i ++;
		 		printf("��ҵ%d��������ĴŴ�����Ϊ%d\n",ppcb->id,temp->id);
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

//��̽������� 
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
	//��ӵ�β��� 
	if( cur == NULL )
	{
		pcb_ready->end->link = ppcb;
		pcb_ready->end = ppcb;
		ppcb->link = NULL;
	}
	else if( pre == cur)//��ӵ�ͷ��� 
	{
		ppcb->link = pcb_ready->head;
		pcb_ready->head = ppcb; 
	} 
	else //���뵽�м�
	{
		ppcb->link = cur;
		pre->link = ppcb;	
	}
	
	pcb_ready->num++;
	
}


//����ĳ�ֲ��Խ�����pcb_ready �еĽ������� 
void put_in_ready(pcb* ppcb){
	switch(pcb_alg)
	{
		case 1: pcb_fcfs(ppcb)   ; break;	
		case 2: pcb_spf(ppcb)    ; break;
		case 3: pcb_fcfs(ppcb)	 ; break; 
	} 
}


void put_in_pcb_ready(){
	 //Ϊpcb_creating�еĽ��̷�����Դ
	//��˳����䣬�ȷ���ǰһ���ڷ����һ��
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
			//�ҵ����������У�����creatingɾ��
		
				ppcb->arrival_time = time;
				
				if( ppcb == pcb_creating->end)
				{
					//�жϾ����������Ƿ�ֻ��һ��Ԫ�� 
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
		
		printf("\n\n*********�ͷ�%d������ռ�ڴ�*********\n\n", pcb_id);
			
		show(); 
		
		ptape = tapelist->head;
		while( num < ppcb->resource && ptape != NULL)
		{
			if( ptape->pcb_id == pcb_id)
			{
				
				printf("�ͷ�%d�Ŵ���\n", ptape->id);
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
		printf("\n��ǰִ�еĽ���%d, ����ʱ��%d����ִ��ʱ��%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
		
		if( running_pcb->run_time == running_pcb->need_time)
		{
			isRunning = 0;
			//�ͷŶ�Ӧ��Դ
			running_pcb->finish_time = time + 1;//1����ĩִ���꼴��һ���ӳ�ִ���� 
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
	 	//ʱ��Ƭ�꣬����ǰ���еĽ��̹ҵ���������ĩβ 
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
	  			printf("\n��ǰִ�еĽ���%d, ����ʱ��%d����ִ��ʱ��%d\n", running_pcb->id, 
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
				//�ͷŶ�Ӧ��Դ
				running_pcb->finish_time = time + 1;//1����ĩִ���꼴��һ���ӳ�ִ���� 
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
	
	if(isRunning == 1)//�н�����ִ�� 
	{
		running_pcb->run_time ++;
		printf("\n��ǰִ�еĽ���%d, ����ʱ��%d����ִ��ʱ��%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
		if(running_pcb->run_time == running_pcb->need_time)
		{
			
			isRunning = 0;
			//�ͷŶ�Ӧ��Դ
			running_pcb->finish_time = time + 1;//1����ĩִ���꼴��һ���ӳ�ִ���� 
			release(running_pcb);
			
			
			 //���뵽����ɶ���
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
			printf("\n��ǰִ�еĽ���%d, ����ʱ��%d����ִ��ʱ��%d\n", running_pcb->id, 
									running_pcb->need_time, running_pcb->run_time );
			if(pcb_ready->num == 1)
			{
				pcb_ready->end = NULL;
			}
			
			pcb_ready->num --;
			
			} 
		
	}
	
} 


//����ɵ���ҵ�������ж��йҵ�����ɶ��У��ͷ���Ӧ��pcb 
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
				
				//�����ж�����ɾ�� 
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
		//�ͷŶ�Ӧ���̿ռ�
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

