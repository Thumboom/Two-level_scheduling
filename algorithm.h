void start_work();
jcb* job_fcfs( );//�����ȷ���
//�����ҵ���� 
jcb* job_sjf();
//����Ӧ������ 
jcb*  jcb_hrrn();
//Ϊ��ҵ�����ڴ� 
Status mem_allocation(jcb* );
//����ҵ�ŵ��󱸶����� 
void put_in_jcb_ready(  );
//�����̷ŵ����̴��������� 
void put_in_pcb_creating();
//���Է���Ŵ���, ����ɹ�����OK��ʧ���򷵻�ERROR 
Status try_alloc_tape(pcb*);
//�ȵ��ȷ��� 
void pcb_fcfs(pcb* ); 
//��̽������� 
void pcb_spf(pcb*);
//����ĳ�ֲ��Խ�����pcb_ready �еĽ������� 
void put_in_ready(pcb* );
void put_in_pcb_ready();
//�ͷ��ڴ�ʹŴ��� 
void release(pcb*);
//ʱ��Ƭ��ת 
void RR();
//ִ�н��� 
void run_pcb(); 
//����ɵ���ҵ�������ж��йҵ�����ɶ��У��ͷ���Ӧ��pcb 
void put_in_jcb_finish();
 
