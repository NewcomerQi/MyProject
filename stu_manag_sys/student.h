/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/




#ifndef STUDENT_H
#define STUDENT_H

#include "node.h"
#include "common.h"

typedef struct student
{
	int cid;	//学生学号
	char name[32];	//学生姓名
	char pass[16];	//学生密码
	int age;	//学生年龄
	int classid;	//班号
	int g_math;	//学生数学成绩
	int g_lang;	//C语言成绩
	int g_phil;	//语文成绩
	int order;	//排名
}stu_t;

/*************
功能：获取学生学号
参数：无
返回值：成功获取返回学号
***************/
int getcid();

/*************
功能：加载链表中学生的信息
参数：无
返回值：成功返回学生信息存放地址
*************/

node_t *loadstu();

/*************
功能：保存学生信息到学生链表
参数：
返回值：保存成功返回0
*************/
int savestu(node_t *stuhead);

/*************
功能：将学生数据添加到学生链表
参数：stuhead表示学生链表，stu表示将要插入的学生
返回值：插入成功返回0.失败返回-1
*************/
int addstu(node_t *stuhead,const stu_t *stu);

/*************
功能：在学生链表中查找学生信息
参数：stuhead表示学生链表，mode表示选择用什么方式查找，stu表示要查找的学生
返回值：成功返回要查找学生信息的地址，失败返回NULL
*************/
node_t *find_stu(node_t *stuhead,int mode,const stu_t *stu);

/************
功能：从学生链表中删除指定学生
参数：stuhead表示学生链表，mode表示选择用什么方式删除，stu表示要删除的学生
返回值：成功返回0,失败返回-1
************/
int deletestu(node_t *stuhead,int mode,const stu_t *stu);

/*************
功能：修改链表中指定学生的信息
参数：stuhead表示学生链表，olddata表示学生本来的信息，mode表示选择要修改的信息，newdata表示要修改成的信息
返回值：成功返回0,失败返回-1
**************/
int updatestu(node_t *stuhead,const stu_t *olddata,int mode,const stu_t *newdata);

/*************
功能：排序展示
参数：stuhead表示学生链表，mode表示用什么方式排序
返回值：空
*************/
void show_stu(node_t *stuhead,int mode);

/*************
功能：比较学生输入的姓名查找学生信息
参数：stuhead表示学生链表，data表示学生的姓名
返回值：成功返回0,失败返回-1
**************/
int find_vdlist(node_t *stuhead,const stu_t *data);

/**************
功能：检查学生用户名和密码是否正确函数
参数：stuhead表示学生链表，data表示学生的姓名和密码
返回值：成功返回0,失败返回-1
***************/
int check(node_t *stuhead,const stu_t *data);

/**************
功能:显示单个学生所有信息
参数：data学生信息存储的地址
返回值：空
************/
void showstu(const void *data);

//比较学生学号函数
int cmp_stu_cid(const void *data1,const void *data2);

//比较学生姓名函数
int cmp_stu_name(const void *data1,const void *data2);

//比较学生年龄函数
int cmp_stu_age(const void *data1,const void *data2);

//比较学生班号函数
int cmp_stu_classid(const void *data1,const void *data2);

//比较学生语文成绩函数
int cmp_stu_glang(const void *data1,const void *data2);

//比较学生数学成绩函数
int cmp_stu_gmath(const void *data1,const void *data2);

//比较学生C语言成绩函数
int cmp_stu_gphil(const void *data1,const void *data2);

//比较学生总分函数
int cmp_stu_gsum(const void *data1,const void *data2);

//比较学生序号函数
int cmp_stu_order(const void *data1,const void *data2);
#endif
