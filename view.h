extern struct jcb *p_jcb ;
//jcb_creating 用来保存预输入的作业信息 
extern struct jcb_list *jcb_creating , *jcb_ready, *jcb_finish,*jcb_run ;
			 
extern struct pcb *p_pcb ;
extern struct pcb_list *pcb_creating , *pcb_ready,*pcb_finish;
 
extern DuLinkList block_first; //头结点
extern DuLinkList block_last;  //尾结点

extern int jcb_alg;//选择作业调度算法 
extern int pcb_alg;//选择进程调度算法 
extern int allc_alg;//选择内存分配算法 
extern int time;

void output_pcb();
