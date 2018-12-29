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
	printf("      ����������������ȵ�ģ��      \n");
	printf("************************************\n");
	printf("��ѡ����ҵ�����㷨��\n");
	printf("** 1)�����ȷ����㷨  2)��С��ҵ�����㷨 **\n");
	scanf("%d", &jcb_alg);
	
	printf("��ѡ���ڴ�����㷨��\n");
	printf("** 1)�״���Ӧ�㷨    2)��������㷨 **\n");
	scanf("%d", &allc_alg); 
	
	printf("��ѡ����̵����㷨��\n");
	printf("** 1)�����ȷ����㷨  2)��С���������㷨 **\n");
	scanf("%d", &pcb_alg); 
	input_jcb();
}

void input_jcb(){
	int num;
	printf("\n��������ҵ����: ");
	scanf("%d", & num);
	
	
	struct jcb *temp = NULL;
	
	
	printf("\n��ֱ�������ҵ����ʱ�䣬��������ʱ�䣬 ��ҵ�����ڴ�(KB)�� ����Ŵ�����������ҵ����\n");
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
			printf("\n��ҵ��� | ��ҵ�� | ��ҵ����ʱ�� | ��������ʱ�� | ��ҵ�����ڴ�(KB) | ����Ŵ������� | \n");
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


