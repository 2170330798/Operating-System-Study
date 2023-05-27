#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED
#include <stdbool.h>
#define FREE 0
#define USED 1

typedef unsigned u16;


typedef struct Thread{
   int id;
   //int start_time;
   //int arrival_time;
   //int need_time;
   //int finish_time;
   int need_size;
   u16 address;//K
   struct Thread * tnext;
}Thread;

Thread *thread;

//系统
typedef struct FreeArea{

   int status;//状态
   int area_size;//分区大小
   u16 address;//空闲分区首地址
   struct FreeArea * pre;//前向指针
   struct FreeArea * fnext;//后向指针

}FreeArea;

int size0;
static int MaxSize;

FreeArea *area,*reco;
//area空闲分区链表
//reco记录上次查询分区链表的下一个节点
void init();//初始化
void create_area();//初始化分区
void read_area();//初始化分区
void read_job();//初始化进程
void allocate_way();//分配方式
void firstfit();//首次适应算法
void nextfit();//循环适应算法
void bubble_sort(const char *s);//链表排序
void bestfit();//最佳适应算法
void worstfit();//最坏适应算法
void retrieve();//回收内存
void display(FreeArea *List1,Thread *List2,const char *s);//显示分区情况

void destroy();//摧毁分区链表，释放资源

#endif // OS_H_INCLUDED
