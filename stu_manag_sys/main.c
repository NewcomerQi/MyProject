/**************
 		版本号：v1.0
		作者：黄运棋
		日期：2021/2/8
**************/





#include "node.h"
#include "student.h"
#include "menu.h"
#include "common.h"

int main()
{
	int issave;
	node_t *stulist = loadstu();	//加载学生信息函数

	login_menu(stulist);		//程序主界面

	printf("是否保存？\n1.保存\n2.不保存\n");
	scanf("%d",&issave);
	if(issave == 1)
	{
		savestu(stulist);		//保存函数
		printf("保存成功!\n");
	}
		node_destroy(&stulist);		//销毁
	return 0;

}
