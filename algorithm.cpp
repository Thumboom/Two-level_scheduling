#include"type.h"
#include"view.h"
#include<string.h>

jcb* job_fifs( ){
	jcb* p = jcb_ready->head; 

	return p;	
		
} 

//����ҵ�ŵ��󱸶����� 
void put_in_jcb_ready(  ){
	jcb* cur = jcb_creating->head;
	jcb* next = NULL;
	printf("\nput_in_jcb_ready()\n") ;
	//��ʱ��˳��ģ����ҵ���ﲢ���뵽�󱸶��й��� 
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

//��ҵ�����㷨ѡ�� 
jcb* job_schedule(){
	jcb* p= NULL,*temp = NULL;
	
	switch(jcb_alg){
		case 1:p = job_fifs();break; 
		
	}
	
	return p;
}

//������ҵ�½�������Ϣ 
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
	printf("\nput_in_pcb_creating()\n") ;
	 //�ж��Ƿ����㹻�ڴ����----------------- 
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
			//�Ӻ󱸶�����ɾ������ҵ�����뵽���ж����� 
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

