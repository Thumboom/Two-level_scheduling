extern struct jcb *p_jcb ;
//jcb_creating ��������Ԥ�������ҵ��Ϣ 
extern struct jcb_list *jcb_creating , *jcb_ready, *jcb_finish,*jcb_run ;
			 
extern struct pcb *p_pcb ;
extern struct pcb_list *pcb_creating , *pcb_ready,*pcb_finish;
 
extern DuLinkList block_first; //ͷ���
extern DuLinkList block_last;  //β���

extern int jcb_alg;//ѡ����ҵ�����㷨 
extern int pcb_alg;//ѡ����̵����㷨 
extern int allc_alg;//ѡ���ڴ�����㷨 
extern int time;

void output_pcb();
