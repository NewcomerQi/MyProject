/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/



#ifndef NODE_H
#define NODE_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SYSERR(x,option,y,message,z)	if((x)option(y))	\
{	\
	printf("%s:%s:%d:%s\n",__FILE__,__func__,__LINE__,message);	\
	return z;	\
}
typedef struct node
{
	void *data;
	struct node *next,*prev;
}node_t;

typedef void (*showfunc_t)(const void *);
typedef int (*cmpfunc_t)(const void *,const void *);
//初始化
node_t *node_init(void);
//显示函数
void node_show(node_t *head,showfunc_t showfunc);
//头增
int node_insert_head(node_t *head,const void *data,int size);
//尾增
int node_insert_tail(node_t *head,const void *data,int size);
//头删
int node_head_del(node_t *head);
//尾删
int node_tail_del(node_t *head);
//差
node_t *node_find(node_t *head,const void *data,cmpfunc_t cmpfunc);
//查找删
int node_find_del(node_t *head,const void *data,cmpfunc_t cmpfunc);
//冒泡排序
int bubble_node(node_t *head,cmpfunc_t cmpfunc);
//选择排序
int select_node(node_t *head,cmpfunc_t cmpfunc);
//销毁
void node_destroy(node_t **head);



#endif
