/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/




#ifndef MENU_H
#define MENU_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "common.h"
#include "node.h"
#include "student.h"
#define SYSERR(x,option,y,message,z)	if((x)option(y))	\
{	\
	printf("%s:%s:%d:%s\n",__FILE__,__func__,__LINE__,message);	\
	return z;	\
}

//加载学生链表
int login_menu(node_t *stulist);
//管理员登陆界面
int welcomeadmin_menu(node_t *stulist);
//管理员操作菜单
int admin_menu(node_t *stulist);
//添加学生菜单
int addstu_menu(node_t *stulist);
//管理员修改学生菜单
int updatestu_menu(node_t *stulist);
//删除学生菜单
int delstu_menu(node_t *stulist);
//显示学生菜单
int showstu_menu(node_t *stulist);
//查找学生菜单
int findstu_menu(node_t *stulist);
//学生登陆界面
int welcomestu_menu(node_t *stulist);
//学生操作菜单
int stu_menu(node_t *stulist,stu_t *stup);
//学生修改菜单
int update_stu_menu(node_t *stulist,stu_t *stup);







#endif
