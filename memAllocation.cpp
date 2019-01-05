#include"type.h"
#include"view.h"
#define Free -1 //空闲状态
#define Busy 1 //已用状态
#define OK 1    //完成
#define ERROR 0 //出错
#define MAX_length 100 //最大内存空间为100KB
Status alloc(int);//内存分配
Status free(int); //内存回收
Status First_fit(int,int);//首次适应算法
Status Best_fit(int,int); //最佳适应算法
void show();//查看分配
Status Initblock();//开创空间表
 
Status Initblock()//开创带头结点的内存空间链表,尾结点作为内存块 
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
		printf("分配大小不合适，请重试！\n");
		return ERROR;
	}
 
	if(allc_alg==2) //选择最佳适应算法
	{
		if(Best_fit(ID,request)==OK) 
		{
			printf("分配成功！\n");
			show();
			return OK;
		}
		else 
		{
			printf("内存不足，分配失败！\n");	
			return ERROR;
		}
		
	}
	else //默认首次适应算法
	{
		if(First_fit(ID,request)==OK) {
			printf("分配成功！\n");
			show();
			return OK;
		}
		else 
		{
			printf("内存不足，分配失败！\n");	
			return ERROR;
		}
	}
	
	
}

 
//分 配 主 存 
Status alloc(int ch)
{
	int ID,request;
	printf("请输入作业(分区号)：");
	scanf("%d", &ID);
	printf("请输入需要分配的内存大小(单位:KB)："); 
	scanf("%d",&request);
	if(request<0 ||request==0) 
	{
		printf("分配大小不合适，请重试！\n");
		return ERROR;
	}
 
	if(ch==2) //选择最佳适应算法
	{
		if(Best_fit(ID,request)==OK) printf("分配成功！\n");
		else printf("内存不足，分配失败！\n");
		return OK;
	}
	else //默认首次适应算法
	{
		if(First_fit(ID,request)==OK) printf("分配成功！\n");
		else printf("内存不足，分配失败！");
		return OK;
	}
}
//首次适应算法
Status First_fit(int ID,int request)//传入作业名及申请量
{
	//为申请作业开辟新空间且初始化
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode)); 
	temp->data.ID=ID; 
	temp->data.size=request;
	temp->data.state=Busy;
 
	DuLNode *p=block_first->next;
	while(p)
	{
		if(p->data.state==Free && p->data.size==request)
		{//有大小恰好合适的空闲块
			p->data.state=Busy;
			p->data.ID=ID;
			return OK;
			break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//有空闲块能满足需求且有剩余"
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
//最佳适应算法 
Status Best_fit(int ID,int request)
{
	int ch; //记录最小剩余空间
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode)); 
	temp->data.ID=ID; 
	temp->data.size=request;
	temp->data.state=Busy;
	DuLNode *p=block_first->next;
	DuLNode *q=NULL; //记录最佳插入位置
	while(p) //初始化最小空间和最佳位置
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
		{//空闲块大小恰好合适
			p->data.ID=ID;
			p->data.state=Busy;
			return OK;
			break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//空闲块大于分配需求
			if(p->data.size-request<ch)//剩余空间比初值还小
			{
				ch=p->data.size-request;//更新剩余最小值
				q=p;//更新最佳位置指向
			}
		}
		p=p->next;
	}
	if(q==NULL) return ERROR;//没有找到空闲块
	else
	{//找到了最佳位置并实现分配
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
 
//内存 回 收 
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
							&& p->prior != block_first)//与前面的空闲块相连
			{
				p->prior->data.size+=p->data.size;
				p->prior->next=p->next;
				if(p == block_last)//释放的结点是尾结点的情况 
				{	
					block_last = p;
					break;
				} 
				p->next->prior=p->prior;
				p = p->prior;//方便后面还有空闲区可以合并用 
			}
			if(p == block_last)//释放的结点是尾结点的情况
				break; 
			
			
			if(p->next->data.state==Free)//与后面的空闲块相连
			{
				if(p->next == block_last)//后面是尾结点 
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
 
//显示内存分配情况
void show()
{
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	printf("+++        内 存 分 配 情 况        +++\n");
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	DuLNode *p=block_first->next;
	while(p)
	{
		printf("分 区 号：");
		if(p->data.ID==Free) printf("Free\n");
		else printf("%d\n",p->data.ID);
		printf("起始地址：%ld\n",p->data.address);
		printf("分区大小：%ld KB\n",p->data.size);
		printf("状    态：");
		if(p->data.state==Free) printf("空  闲\n");
		else printf("已分配\n");
		printf("--------------\n");
		p=p->next;
	}
}
 

int testAlloc()
{

	printf("       动态分区分配方式的模拟       \n");
	printf("************************************\n");
	printf("** 1)首次适应算法  2)最佳适应算法 **\n");
	printf("************************************\n");
	printf("请选择分配算法：");
	Initblock(); //开创空间表
	int choice;  //操作选择标记
	
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("++    1: 分配内存        2: 回收内存      ++\n");
	printf("++    3: 查看内存        0: 退    出      ++\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	while(1)
	{
    
    	printf("请输入您的操作 ：");
    	scanf("%d",&choice);
		if(choice==1)
		{ 
			alloc(allc_alg); // 分配内存			
			show(); 
		} 
		else if(choice==2)  // 内存回收
		{
			int ID;
			printf("请输入您要释放的分区号：");
			scanf("%d",&ID);
			free(ID);
			show();
		}
		else if(choice==3) show();//显示内存
		else if(choice==0) break; //退出
		else //输入操作有误
		{
			printf("输入有误，请重试！\n");
			continue;
		}
	}
}


