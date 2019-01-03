
typedef int Status;
Status alloc(int);//内存分配
Status free(int); //内存回收
Status First_fit(int,int);//首次适应算法
Status Best_fit(int,int); //最佳适应算法
void show();//查看分配
Status Initblock();//开创空间表
Status try_alloc(int, int);

Status free(int);
#define Free 0 //空闲状态
#define Busy 1 //已用状态
#define OK 1    //完成
#define ERROR 0 //出错
#define MAX_length 640
