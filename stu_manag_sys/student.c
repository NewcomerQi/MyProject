/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/



#include "student.h"

static int cid = 0;

//获取学生学号
int getcid()
{
	return cid > 99999999 ? 1 : ++cid;
}

//显示函数
void showstu(const void *data)
{
	const stu_t *stu1 = (const stu_t *)data;
	printf("%d %s %s %d %d %d %d %d %d\n",stu1->cid,stu1->name,stu1->pass,stu1->age,stu1->classid,stu1->g_math,stu1->g_lang,stu1->g_phil,stu1->order);
}


//加载学生信息
node_t *loadstu()
{
	node_t *head = node_init();
	FILE *fp = fopen(STUFILE,"r");
	SYSERR(NULL,==,fp,"fopen error",head);
	fscanf(fp,"%d\n",&cid);

	stu_t temp = {0};

	while(fscanf(fp,"%d %s %s %d %d %d %d %d %d\n",&temp.cid,temp.name,temp.pass,&temp.age,&temp.classid,&temp.g_math,&temp.g_lang,&temp.g_phil,&temp.order) != EOF)
	{
		node_insert_head(head,&temp,sizeof(stu_t));		//调用链表中的头查函数
		bzero(&temp,sizeof(temp));
	}
	fclose(fp);
	return head;
}

//保存学生函数
int savestu(node_t *stuhead)
{
	SYSERR(stuhead,==,NULL,"stuhead is null",-1);
	FILE *fp = fopen(STUFILE,"w");
	SYSERR(fp,==,NULL,"fopen error",-1);
	fprintf(fp,"%d\n",cid);

	node_t *temp = stuhead->next;
	stu_t *stup = NULL;
	while(temp !=stuhead )
	{
		stup = (stu_t *)temp->data;
		fprintf(fp,"%d %s %s %d %d %d %d %d %d\n",stup->cid,stup->name,stup->pass,stup->age,stup->classid,stup->g_math,stup->g_lang,stup->g_phil,stup->order);
		temp  = temp->next;
	}
	fclose(fp);
	return 0;
}

//比较学生学号函数
int cmp_stu_cid(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->cid - stu2->cid;
}
//比较名字
int cmp_stu_name(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return strcmp(stu1->name,stu2->name);
}
//比较密码
int cmp_stu_pass(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return strcmp(stu1->pass,stu2->pass);
}
//比较年龄
int cmp_stu_age(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->age - stu2->age;
}
//比较班号
int cmp_stu_classid(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->classid - stu2->classid;
}
//比较语文
int cmp_stu_glang(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->g_lang - stu2->g_lang;
}
//比较数学
int cmp_stu_gmath(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->g_math - stu2->g_math;
}

//比较C语言
int cmp_stu_gphil(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->g_phil - stu2->g_phil;
}

//比较总分
int cmp_stu_gsum(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return (stu1->g_lang + stu1->g_math + stu1->g_phil) - (stu2->g_lang + stu2->g_math + stu2->g_phil);
}

//比较序号
int cmp_stu_order(const void *data1,const void *data2)
{
	const stu_t *stu1 = data1;
	const stu_t *stu2 = data2;
	return stu1->order - stu2->order;
}

//添加学生
int addstu(node_t *stuhead,const stu_t *stu)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",-1);
	SYSERR(stu,==,NULL,"stuhead is NULL",-1);
	node_insert_head(stuhead,stu,sizeof(stu_t));
	printf("添加成功\n");
	showstu(stu);
	return 0;
}

//删除学生

int deletestu(node_t *stuhead,int mode,const stu_t *stu)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",-1);
	SYSERR(stu,==,NULL,"stu is NULL",-1);
	if(mode<1 && mode>8)
		return -1;
	if(mode == 1)
	{
		while(node_find_del(stuhead,stu,cmp_stu_cid) == 0)
			;
			printf("删除成功\n");
			node_show(stuhead,showstu);
	}
	if(mode == 2)
	{
		while(node_find_del(stuhead,stu,cmp_stu_name) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	if(mode == 3)
	{
		while(node_find_del(stuhead,stu,cmp_stu_classid) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	if(mode == 4)
	{
		while(node_find_del(stuhead,stu,cmp_stu_age) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	if(mode == 5)
	{
		while(node_find_del(stuhead,stu,cmp_stu_gmath) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	if(mode == 6)
	{
		while(node_find_del(stuhead,stu,cmp_stu_glang) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	if(mode == 7)
	{
		while(node_find_del(stuhead,stu,cmp_stu_gphil) == 0)
			;
		printf("删除成功\n");
		node_show(stuhead,showstu);
	}
	return 0;

}

node_t *find_stu(node_t *stuhead,int mode,const stu_t *data)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",NULL);
	SYSERR(data,==,NULL,"stu is NULL",NULL);
	node_t *p = stuhead;
	if(mode<1 && mode>8)
		return NULL;
	if(mode == 1)
	{
		while((p = node_find(p,data,cmp_stu_name)) != NULL)
			showstu(p->data);
	}
	if(mode == 2)
	{
		while((p = node_find(p,data,cmp_stu_cid)) != NULL)
			showstu(p->data);
	}
	if(mode == 3)
	{
		while((p = node_find(p,data,cmp_stu_age)) != NULL)
			showstu(p->data);
	}
	if(mode == 4)
	{
		while((p = node_find(p,data,cmp_stu_classid)) != NULL)
			showstu(p->data);
	}
	if(mode == 5)
	{
		while((p = node_find(p,data,cmp_stu_glang)) != NULL)
			showstu(p->data);
	}
	if(mode == 6)
	{
		while((p = node_find(p,data,cmp_stu_gmath)) != NULL)
			showstu(p->data);
	}
	if(mode == 7)
	{
		while((p = node_find(p,data,cmp_stu_gphil)) != NULL)
			showstu(p->data);
	}
	if(mode == 8)
	{
		while((p = node_find(p,data,cmp_stu_order)) != NULL)
			showstu(p->data);
	}
	return p;
}



int updatestu(node_t *stuhead,const stu_t *olddata,int mode,const stu_t *newdata)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",-1);
	SYSERR(olddata,==,NULL,"olddata is NULL",-1);
	SYSERR(newdata,==,NULL,"newdata is NULL",-1);

	if(mode<1 && mode>8)
		return -1;

	if(mode == 1)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		strcpy(stup->name,newdata->name);
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}
	if(mode == 2)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		strcpy(stup->pass,newdata->pass);
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}

	if(mode == 3)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->age = newdata->age;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}

	
	if(mode == 4)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->classid = newdata->classid;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}
	if(mode == 5)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->cid = newdata->cid;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}

	if(mode == 6)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->g_lang = newdata->g_lang;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}

	if(mode == 7)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->g_math = newdata->g_math;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}

	if(mode == 8)
	{
		node_t *result  = node_find(stuhead,olddata,cmp_stu_name);
		stu_t *stup = (stu_t *)result->data;
		if(result == NULL)
			return -1;
		stup->g_phil = newdata->g_phil;
		printf("修改成功\n");
		find_vdlist(stuhead,stup);
	}


	return 0;
}

void show_stu(node_t *stuhead,int mode)
{
	int choose;
	if(mode == 1)
	{
		bubble_node(stuhead,cmp_stu_name);
	}
	if(mode == 2)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_cid);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_cid);
	}
	if(mode == 3)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_age);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_age);
	}
	if(mode == 4)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_classid);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_classid);
	}
	if(mode == 5)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_glang);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_glang);
	}
	if(mode == 6)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_gmath);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_gmath);
	}
	if(mode == 7)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_gphil);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_gphil);
	}
	if(mode == 8)
	{
		printf("请选择：1.顺序 2.逆序\n");
		scanf("%d",&choose);
		if(choose == 1)
			select_node(stuhead,cmp_stu_gsum);
		if(choose == 2)
			bubble_node(stuhead,cmp_stu_gsum);
	}
	return;

}
int find_vdlist(node_t *stuhead,const stu_t *data)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",-1);
	SYSERR(data,==,NULL,"stu is NULL",-1);

	node_t *p = node_find(stuhead,data,cmp_stu_name);
	if(p != NULL)
	{
		printf("学生信息：\n");
		showstu(p->data);
		return 0;
	}
	else
	{
		return -1;
	}

}

//检查学生用户名和密码是否正确
int check(node_t *stuhead,const stu_t *data)
{
	SYSERR(stuhead,==,NULL,"stuhead is NULL",-1);
	SYSERR(data,==,NULL,"stu is NULL",-1);

	node_t *p = node_find(stuhead,data,cmp_stu_name);
	if(p != NULL)
	{
		if(cmp_stu_pass(p->data,data) == 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}

	}
	else
	{
		return -1;
	}
	return 0;

}



