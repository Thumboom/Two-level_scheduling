
typedef int Status;
Status alloc(int);//�ڴ����
Status free(int); //�ڴ����
Status First_fit(int,int);//�״���Ӧ�㷨
Status Best_fit(int,int); //�����Ӧ�㷨
void show();//�鿴����
Status Initblock();//�����ռ��
Status try_alloc(int, int);

Status free(int);
#define Free 0 //����״̬
#define Busy 1 //����״̬
#define OK 1    //���
#define ERROR 0 //����
#define MAX_length 640
