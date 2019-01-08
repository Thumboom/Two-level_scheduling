void start_work();
jcb* job_fcfs( );//先来先服务
//最短作业优先 
jcb* job_sjf();
//高响应比优先 
jcb*  jcb_hrrn();
//为作业分配内存 
Status mem_allocation(jcb* );
//将作业放到后备队列上 
void put_in_jcb_ready(  );
//将进程放到进程创建队列中 
void put_in_pcb_creating();
//尝试分配磁带机, 分配成功返回OK，失败则返回ERROR 
Status try_alloc_tape(pcb*);
//先到先服务 
void pcb_fcfs(pcb* ); 
//最短进程优先 
void pcb_spf(pcb*);
//根据某种策略将放入pcb_ready 中的进程排序 
void put_in_ready(pcb* );
void put_in_pcb_ready();
//释放内存和磁带机 
void release(pcb*);
//时间片轮转 
void RR();
//执行进程 
void run_pcb(); 
//将完成的作业从运行中队列挂到已完成队列，释放相应的pcb 
void put_in_jcb_finish();
 
