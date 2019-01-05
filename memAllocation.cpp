#include"type.h"
#include"view.h"
#define Free -1 //����״̬
#define Busy 1 //����״̬
#define OK 1    //���
#define ERROR 0 //����
#define MAX_length 100 //����ڴ�ռ�Ϊ100KB
Status alloc(int);//�ڴ����
Status free(int); //�ڴ����
Status First_fit(int,int);//�״���Ӧ�㷨
Status Best_fit(int,int); //�����Ӧ�㷨
void show();//�鿴����
Status Initblock();//�����ռ��
 
Status Initblock()//������ͷ�����ڴ�ռ�����,β�����Ϊ�ڴ�� 
{
	block_first=(DuLinkList)malloc(sizeof(DuLNode));
	block_last=(DuLinkList)malloc(sizeof(DuLNode));
	block_first->prior=NULL;
	block_first->next=block_last;
	block_last->prior=block_first;
	block_last->next=NULL;
	block_last->data.address=0;
	block_last->data.size=MAX_length;
	block_last->data.ID=Free;
	block_last->data.state=Free;
	return OK;
}


Status try_alloc(int ID, int request){

	if(request<0 ||request==0) 
	{
		printf("�����С�����ʣ������ԣ�\n");
		return ERROR;
	}
 
	if(allc_alg==2) //ѡ�������Ӧ�㷨
	{
		if(Best_fit(ID,request)==OK) 
		{
			printf("����ɹ���\n");
			show();
			return OK;
		}
		else 
		{
			printf("�ڴ治�㣬����ʧ�ܣ�\n");	
			return ERROR;
		}
		
	}
	else //Ĭ���״���Ӧ�㷨
	{
		if(First_fit(ID,request)==OK) {
			printf("����ɹ���\n");
			show();
			return OK;
		}
		else 
		{
			printf("�ڴ治�㣬����ʧ�ܣ�\n");	
			return ERROR;
		}
	}
	
	
}

 
//�� �� �� �� 
Status alloc(int ch)
{
	int ID,request;
	printf("��������ҵ(������)��");
	scanf("%d", &ID);
	printf("��������Ҫ������ڴ��С(��λ:KB)��"); 
	scanf("%d",&request);
	if(request<0 ||request==0) 
	{
		printf("�����С�����ʣ������ԣ�\n");
		return ERROR;
	}
 
	if(ch==2) //ѡ�������Ӧ�㷨
	{
		if(Best_fit(ID,request)==OK) printf("����ɹ���\n");
		else printf("�ڴ治�㣬����ʧ�ܣ�\n");
		return OK;
	}
	else //Ĭ���״���Ӧ�㷨
	{
		if(First_fit(ID,request)==OK) printf("����ɹ���\n");
		else printf("�ڴ治�㣬����ʧ�ܣ�");
		return OK;
	}
}
//�״���Ӧ�㷨
Status First_fit(int ID,int request)//������ҵ����������
{
	//Ϊ������ҵ�����¿ռ��ҳ�ʼ��
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode)); 
	temp->data.ID=ID; 
	temp->data.size=request;
	temp->data.state=Busy;
 
	DuLNode *p=block_first->next;
	while(p)
	{
		if(p->data.state==Free && p->data.size==request)
		{//�д�Сǡ�ú��ʵĿ��п�
			p->data.state=Busy;
			p->data.ID=ID;
			return OK;
			break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//�п��п���������������ʣ��"
			temp->prior=p->prior;
			temp->next=p;      
			temp->data.address=p->data.address;
			p->prior->next=temp; 
			p->prior=temp;
			p->data.address=temp->data.address+temp->data.size;
			p->data.size-=request;
			return OK;
			break;
		}
		p=p->next;
	}
	return ERROR;
}
//�����Ӧ�㷨 
Status Best_fit(int ID,int request)
{
	int ch; //��¼��Сʣ��ռ�
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode)); 
	temp->data.ID=ID; 
	temp->data.size=request;
	temp->data.state=Busy;
	DuLNode *p=block_first->next;
	DuLNode *q=NULL; //��¼��Ѳ���λ��
	while(p) //��ʼ����С�ռ�����λ��
	{
		if(p->data.state==Free &&
			(p->data.size>request || p->data.size==request) )
		{
			q=p;
			ch=p->data.size-request;
			break;
		}
		p=p->next;
	}
	while(p)
	{
		if(p->data.state==Free && p->data.size==request)
		{//���п��Сǡ�ú���
			p->data.ID=ID;
			p->data.state=Busy;
			return OK;
			break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//���п���ڷ�������
			if(p->data.size-request<ch)//ʣ��ռ�ȳ�ֵ��С
			{
				ch=p->data.size-request;//����ʣ����Сֵ
				q=p;//�������λ��ָ��
			}
		}
		p=p->next;
	}
	if(q==NULL) return ERROR;//û���ҵ����п�
	else
	{//�ҵ������λ�ò�ʵ�ַ���
		temp->prior=q->prior;
		temp->next=q;
		temp->data.address=q->data.address;
		q->prior->next=temp;
		q->prior=temp;
		q->data.address+=request;
		q->data.size=ch;
		return OK;
	}
}
 
//�ڴ� �� �� 
Status free(int ID)
{
	DuLNode *p=block_first->next;
	while(p)
	{
		if(p->data.ID==ID)
		{
			p->data.state=Free;
			p->data.ID=Free;
			
			if(p->prior->data.state==Free 
							&& p->prior != block_first)//��ǰ��Ŀ��п�����
			{
				p->prior->data.size+=p->data.size;
				p->prior->next=p->next;
				if(p == block_last)//�ͷŵĽ����β������� 
				{	
					block_last = p;
					break;
				} 
				p->next->prior=p->prior;
				p = p->prior;//������滹�п��������Ժϲ��� 
			}
			if(p == block_last)//�ͷŵĽ����β�������
				break; 
			
			
			if(p->next->data.state==Free)//�����Ŀ��п�����
			{
				if(p->next == block_last)//������β��� 
				{
					p->data.size+=p->next->data.size;
					block_last = p;
					p->next = NULL; 
					break;
				}
				
	 			p->data.size+=p->next->data.size;
				p->next->next->prior=p;
				p->next=p->next->next;				
			}
				
				break;	
		}
		p=p->next;
	}
	return OK;
}
 
//��ʾ�ڴ�������
void show()
{
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	printf("+++        �� �� �� �� �� ��        +++\n");
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	DuLNode *p=block_first->next;
	while(p)
	{
		printf("�� �� �ţ�");
		if(p->data.ID==Free) printf("Free\n");
		else printf("%d\n",p->data.ID);
		printf("��ʼ��ַ��%ld\n",p->data.address);
		printf("������С��%ld KB\n",p->data.size);
		printf("״    ̬��");
		if(p->data.state==Free) printf("��  ��\n");
		else printf("�ѷ���\n");
		printf("--------------\n");
		p=p->next;
	}
}
 

int testAlloc()
{

	printf("       ��̬�������䷽ʽ��ģ��       \n");
	printf("************************************\n");
	printf("** 1)�״���Ӧ�㷨  2)�����Ӧ�㷨 **\n");
	printf("************************************\n");
	printf("��ѡ������㷨��");
	Initblock(); //�����ռ��
	int choice;  //����ѡ����
	
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("++    1: �����ڴ�        2: �����ڴ�      ++\n");
	printf("++    3: �鿴�ڴ�        0: ��    ��      ++\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	while(1)
	{
    
    	printf("���������Ĳ��� ��");
    	scanf("%d",&choice);
		if(choice==1)
		{ 
			alloc(allc_alg); // �����ڴ�			
			show(); 
		} 
		else if(choice==2)  // �ڴ����
		{
			int ID;
			printf("��������Ҫ�ͷŵķ����ţ�");
			scanf("%d",&ID);
			free(ID);
			show();
		}
		else if(choice==3) show();//��ʾ�ڴ�
		else if(choice==0) break; //�˳�
		else //�����������
		{
			printf("�������������ԣ�\n");
			continue;
		}
	}
}


