#include "Os.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define Size (sizeof(FreeArea))

void init()
{
    area = (FreeArea*)malloc(sizeof(FreeArea));
    area->area_size = 0;//KB
    area->status    = FREE;
    area->address   = 0x0000;//K
    area->fnext = area->pre = NULL;
    size0 = 100;
    MaxSize = 512;//K
    thread = (Thread*)malloc(sizeof(Thread));
    //thread->arrival_time = 0;
    //thread->finish_time  = 0;
    thread->id           = 1;
    thread->need_size    = sizeof(Thread)*2;
    //thread->need_time    = 0;
    //thread->start_time   = 0;
    thread->address = 0x0000;//k
    thread->tnext = NULL;
    reco = area->fnext;
    printf("Initialized Finished!\n");
}

void display(FreeArea *List1,Thread *List2,const char *s)
{
    if(List1 != NULL && List2 == NULL)
    {
        FreeArea *p1 = List1->fnext;
        printf("+-----------------%s-------------------+\n",s);
        while(p1)
        {
            printf("|size: %2d    address: %0X     status: %2d  |\n",p1->area_size,p1->address,p1->status);
            p1 = p1->fnext;
        }
        printf("+--------------------------------------------+\n");
    }
    if(List2 != NULL && List1 == NULL)
    {
        Thread *p2 = List2;
        printf("+-------------------%s-------------------+\n",s);
        while(p2)
        {
            printf("|id: %2d    size: %2d    address: %0X       |\n",p2->id,p2->need_size,p2->address);
            p2 = p2->tnext;
        }
        printf("+--------------------------------------------+\n");

    }
}

/*
void read_job()
{
    Thread *p = thread, *p2 = NULL;
    FILE *fp=fopen("task.txt","w+");
    if(fp == NULL)
    {
        printf("Can't open!\n");
    }
    int index = 0,i=0;
    fscanf(fp,"%d",&index);
    while(i<index)
    {
        p2 = (Thread*)malloc(sizeof(Thread));
        p2->address = &(p->tnext);
        fscanf(fp,"%d%d",p2->id,p2->need_size);
        p->tnext = p2;
        p = p2;
        i++;
    }
    p->tnext = NULL;

    fclose(fp);
}
*/
void create_area()
{
    FreeArea *p = area,*p2 = NULL;
    FILE *fp=fopen("area.txt","w+");
    if(fp == NULL)
    {
        printf("Can't open!\n");
    }
    int index = 10,i=0;
    fprintf(fp,"%s","size:  address:  status:\n");
    while(i<index)
    {
        p2 = (FreeArea*)malloc(Size);
        p2->address = &(p->fnext);
        p2->area_size = Size+rand()%Size+1;
        p2->status = FREE;
        fprintf(fp,"%d %X %d\n",p->area_size,p->address,p->status);
        p->fnext = p2;
        p2->pre = p;
        p = p2;
        i++;
    }
    p->fnext = NULL;
    printf("Create Finished!\n");
    fclose(fp);
}

void read_area()
{
    FreeArea *p = area;
    FILE *fp=fopen("area.txt","r+");
    if(fp == NULL || p == NULL)
    {
        printf("Can't open!\n");
    }

    while(p)
    {
        fscanf(fp,"%d%X%d",&p->area_size,&p->address,&p->status);
        p = p->fnext;
    }
    printf("Read Finished!\n");
    fclose(fp);
}

void firstfit()
{
    FreeArea *p = area->fnext;
    while(p)
    {
        if(p->area_size > thread->need_size)
        {
            if(p->area_size - thread->need_size <= size0)
            {
                thread->address = p->address;
                p->area_size = (p->area_size - thread->need_size);//分区大小减去使用的大小
                p->status = USED;//使用了
                break;
            }
            else
            {
                thread->address = p->address;
                p->status = USED;
                break;
            }
        }
        p = p->fnext;
    }
}

void swap_node(FreeArea *node1,FreeArea *node2,FreeArea *node3)
{
    //交换节点
    node1->fnext = node3;
    node3->pre = node1;
    node2->fnext = node3->fnext;
    node3->pre  =  node2->pre;
    node3->fnext = node2;
    node2->pre = node3;
}

void bubble_sort(const char *s)
{

    FreeArea *pre, *p, *q;
    FreeArea *tail = NULL;
    while (area->fnext != tail)//传入的是带头节点的链表,为了简便操作
    {
        //头节点未赋值,所以从head->next开始
        pre = area;
        p = area->fnext;
        q = p->fnext;
        while (p->fnext != tail)
        {
            if ((strcmp("worstfit",s)==0)&&(p->area_size < q->area_size))//降序排列
            {   //交换节点
                swap_node(pre,p,q);
            }else
            if((strcmp("bestfit",s)==0)&&(p->area_size > q->area_size))//升序排列
            {   //交换节点
                swap_node(pre,p,q);
            }
            else{
                p = p->fnext;
            }
            q = p->fnext;//这里，p,q如果交换了节点，p已经在if里后移了
            //不然p就在else里后移，所以q赋值p->next就行了
            pre = pre->fnext;
        }
        tail = p;//一次循环后，最后一个数已最小，tail前移
    }
}

void bestfit()
{
    bubble_sort("bestfit");
    FreeArea *p = area->fnext;
    while(p)
    {
        if(p->area_size > thread->need_size)
        {
            if(p->area_size - thread->need_size <= size0)
            {
                thread->address = p->address;
                p->area_size = (p->area_size - thread->need_size);//分区大小减去使用的大小
                p->status = USED;//使用了
                break;
            }
            else
            {
                thread->address = p->address;
                p->status = USED;
                break;
            }
        }
        p = p->fnext;
    }


}

void worstfit()
{
    bubble_sort("worstfit");//分区从大到小构成分区链表
    FreeArea *p = area->fnext;
    while(p)
    {
        if(p->area_size > thread->need_size)
        {
            if(p->area_size - thread->need_size <= size0)
            {
                thread->address = p->address;
                p->area_size = (p->area_size - thread->need_size);//分区大小减去使用的大小
                p->status = USED;//使用了
                break;
            }
            else
            {
                thread->address = p->address;
                p->status = USED;
                break;
            }
        }
        p = p->fnext;
    }
}

void nextfit()
{
    FreeArea *first = reco,*reco2 = NULL;
    reco2 = first;
    while(first)
    {
        if(first->area_size > thread->need_size)
        {
            if(first->area_size - thread->need_size <= size0)
            {
                thread->address = first->address;
                first->area_size = (first->area_size - thread->need_size);//分区大小减去使用的大小
                first->status = USED;//使用了
                reco = first->fnext;
                break;
            }
            else
            {
                thread->address = first->address;
                first->status = USED;
                reco = first->fnext;
                break;
            }
        }
        first = first->fnext;
    }
    reco = first = reco2;
}

void retrieve()//回收机制
{
    FreeArea *p = area->fnext,*p2 = NULL,*p3 = NULL,*p4 = NULL,*next = NULL;
    //1
    while(p)
    {
        p2 = p;
        next = p2->fnext;
        if(p2->status == USED)
        {
            if(p2->pre != NULL)
            {
                p3 = p2;
                p2->pre->area_size += p2->area_size;
                p2->status = FREE;
                p2->pre->fnext = p2->fnext;
                p2->fnext->pre = p2->pre;
                free(p3);
            }
            else if(p2->fnext != NULL)
            {
                p3 = p2->fnext;
                p2 = p2->fnext->fnext;
                p2->fnext->fnext->pre = p2;
                free(p3);
            }
            else if(p2->fnext != NULL && p2->pre != NULL && p2->status == USED)
            {
                p3 = p2;
                p4 = p2->fnext;
                p2->pre->area_size += p2->area_size+p2->fnext->area_size;
                p2->pre->fnext = p2->fnext;
                p2->fnext->pre = p2->pre;
                free(p3);
                free(p4);
            }
            /*
            else
            {
                p2 = p;
                while(p2)
                {
                    if(p2->area_size == 0 )
                    {
                        if(p2->address == 100 && p2->pre->status == USED)
                        {
                            p3 = p2;
                            p3->pre->area_size += p3->area_size;
                            p3->pre->fnext = p3->fnext;
                            p3->fnext->pre = p3->pre;
                            free(p3);
                        }
                    }
                    p2 = p2->fnext;
                }

            }
            */
        }
        p = next;
    }
    printf("Recall Finished!\n");
}

void destroy()
{
    FreeArea *p = area, *pre = NULL;

    while(p)
    {
        pre = p->fnext;
        free(p);
        p = pre;
    }
    free(thread);
    printf("Destroy Finished!\n");
}





























