/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/

#include "node.h"
node_t *node_init(void)
{
	node_t *head = malloc(sizeof(node_t));
	SYSERR(head,==,NULL,"malloc error",NULL);
	head->next = head->prev = head;
	head->data = NULL;
	return head;
}

int node_insert_head(node_t *head,const void *data,int size)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(data,==,NULL,"data is NULL",-1);
	SYSERR(size,<,1,"size must >=1",-1);

	node_t *p = malloc(sizeof(node_t));
	SYSERR(p,==,NULL,"malloc error",-1);

	p->data = malloc(size);
	if(p->data == NULL)
	{
		free(p);
		printf("malloc data error\n");
		return -1;
	}

	memcpy(p->data,data,size);

	p->next = head->next;
	head->next = p;
	p->prev = head;
	p->next->prev = p;
	return 0;

}
void node_show(node_t *head,showfunc_t showfunc)
{
	node_t *p = head->next;
	while(p != head)
	{
		showfunc(p->data);
		p=p->next;
	}
}

int node_insert_tail(node_t *head,const void *data,int size)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(data,==,NULL,"data is NULL",-1);
	SYSERR(size,<,1,"size must >=1",-1);

	node_t *p = malloc(sizeof(node_t));
	SYSERR(p,==,NULL,"malloc error",-1);

	p->data = malloc(size);
	if(p->data == NULL)
	{
		free(p);
		printf("malloc data error\n");
		return -1;
	}

	memcpy(p->data,data,size);

	p->next = head;
	head->prev->next = p;
	p->prev = head->prev;
	head->prev = p;
	return 0;
}
int node_head_del(node_t *head)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(head->next,==,NULL,"head is empty",-1);
	node_t *p = head->next;
	head->next = p->next;
	p->next->prev = head;

	free(p->data);
	free(p);
	return 0;
}
int node_tail_del(node_t *head)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(head->next,==,NULL,"head is empty",-1);
	node_t *p = head->prev;
	head->prev = p->prev;
	head->prev->next = head;

	free(p->data);
	free(p);
	return 0;
}

node_t *node_find(node_t *head,const void *data,cmpfunc_t cmpfunc)
{
	SYSERR(head,==,NULL,"head is NULL",NULL);
	SYSERR(head->next,==,NULL,"head is empty",NULL);
	SYSERR(data,==,NULL,"data is NULL",NULL);
	node_t *p = head->next;
	while(p != head && p->data != NULL)
	{
		if(cmpfunc(data,p->data) == 0)
		{
			return p;
		}
		p=p->next;
	}
	return NULL;

}
int node_find_del(node_t *head,const void *data,cmpfunc_t cmpfunc)
{
	/*
	   SYSERR(head,==,NULL,"head is NULL",-1);
	   SYSERR(head->next,==,NULL,"head is empty",-1);
	   SYSERR(data,==,NULL,"data is NULL",-1);
	   node_t *p = head->next;
	   while(p != head && p->data != NULL)
	   {
	   if(cmpfunc(data,p->data) == 0)
	   {
	   p->prev->next = p->next;
	   p->next->prev = p->prev;
	   free(p->data);
	   free(p);
	   return 0;
	   }
	   p = p->next;
	   }
	   printf("not found\n");
	   return -1;
	   */
	node_t *p = node_find(head,data,cmpfunc);
	if(p == NULL)
	{
		return -1;
		
	}
//	SYSERR(p,==,NULL,"not found",-1);
	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p->data);
	free(p);
	return 0;
}
int bubble_node(node_t *head,cmpfunc_t cmpfunc)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(head->next,==,NULL,"head is empty",-1);
	node_t *i,*j,*end;
	void *temp;
	int flag;
	for(i = head->next,end=head; i->next != head;i=i->next,end=end->prev)
	{
		flag = 0;
		for(j=head->next; j->next != end; j=j->next)
		{
			if(cmpfunc(j->data,j->next->data)<0)
			{
				flag = 1;
				temp = j->data;
				j->data = j->next->data;
				j->next->data = temp;
			}
		}
		if(flag==0)
			break;
	}
	return 0;
}
int select_node(node_t *head,cmpfunc_t cmpfunc)
{
	SYSERR(head,==,NULL,"head is NULL",-1);
	SYSERR(head->next,==,NULL,"head is empty",-1);
	node_t *i,*j,*min;
	void *temp;
	for(i=head->next; i->next != head; i=i->next)
	{
		min = i;
		for(j=i->next; j != head; j=j->next)
		{
			if(cmpfunc(min->data,j->data)>0)
				min = j;
			
		}
		if(min != i)
		{
			temp = min->data;
			min->data = i->data;
			i->data = temp;
		}

	}
	return 0;


}

void node_destroy(node_t **head)
{
	node_t *p = (*head)->next;//*head没有数据位
	node_t *q;
	while(p != *head)
	{
		q = p->next;
		free(p->data);
		free(p);
		p=q;

	}
	free(*head);
	*head = NULL;

}
