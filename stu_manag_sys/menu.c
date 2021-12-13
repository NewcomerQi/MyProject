/**************
  版本号：v1.0
  作者：黄运棋
  日期：2021/2/8
 **************/

#include "menu.h"

int login_menu(node_t *stulist)
{
	system("clear");	//清屏
	while(1)
	{

		printf("管理员菜单\n");
		printf("1.管理员登陆\n");
		printf("2.学生登陆\n");
		printf("3.退出\n");

		char buf[128];
		printf("请输入你的选择：\n");
		scanf("%s",buf);
		system("clear");

		switch(atoi(buf))
		{
		case 1:welcomeadmin_menu(stulist);break;	//调用管理员登陆函数
		case 2:welcomestu_menu(stulist);break;		//调用学生登陆函数
		case 3:break;
		default:printf("你输入的有误，请重新输入!\n");break;
		}
		if(atoi(buf) == LOOPCOUNT)		//退出
			break;
	}

	return 0;
}

//管理员登陆函数
int welcomeadmin_menu(node_t *stulist)
{
	system("clear");
	int num=0;
	int i=3;
	char name[16];
	char pass[16];
	do	//允许输错三次
	{
		printf("你还有%d次机会\n",i);
		printf("请输入管理员用户名和密码:\n");
		scanf("%s%s",name,pass);
		num++;
		i--;

		if(strcmp(ADMINNAME,name)==0 && strcmp(ADMINPASS,pass)==0)
		{
			printf("登陆成功\n");
			admin_menu(stulist);		//调用管理员操作菜单
			return 0;
		}
		else
		{
			printf("账户或密码错误\n");
		}
	}
	while(num<=2);		
	system("clear");
	return 0;

}

//管理员操作菜单
int admin_menu(node_t *stulist)
{
	system("clear");
	while(1)
	{
		printf("欢迎你管理员!\n");
		printf("管理员菜单:\n");
		printf("1.添加学生\n");
		printf("2.删除学生\n");
		printf("3.修改学生\n");
		printf("4.查看学生\n");
		printf("5.查找学生\n");
		printf("6.退出\n");

		char buf[128];
		printf("请输入你的选择：\n");
		scanf("%s",buf);
		system("clear");
		switch(atoi(buf))
		{
		case 1:addstu_menu(stulist);break;		//调用添加学生菜单
		case 2:delstu_menu(stulist);break;		//调用删除学生菜单
		case 3:updatestu_menu(stulist);break;	//调用修改学生菜单
		case 4:showstu_menu(stulist);break;		//调用显示菜单
		case 5:findstu_menu(stulist);break;		//调用查找菜单
		case 6:break;
		default:printf("你输入的有误，请重新输入!\n");break;
		}
		if(atoi(buf) == 6)		//退出
			break;
	}
	return 0;
}

//添加学生菜单
int addstu_menu(node_t *stulist)
{
	while(1)
	{
		system("clear");
		char name[16];
		char pass[16];
		char age[128],classid[128],math[128],glang[128],gphil[128],order[128];
		int stu_id,choose,check;
		stu_id = getcid();

		printf("请输入学生的用户名和密码：\n");
		scanf("%s%s",name,pass);
		getchar();
		printf("请输入学生的年龄，班号和序号：\n");
		scanf("%s%s%s",age,classid,order);
		getchar();
		printf("请输入学生数学，语文，C语言成绩：\n");
		scanf("%s%s%s",math,glang,gphil);
		getchar();
		system("clear");
		if((age[0]!='0' && atoi(age)==0) || (classid[0]!=0 && atoi(classid)==0) || (order[0]!=0 && atoi(order)==0) || (math[0]!=0 && atoi(math)==0) || (glang[0]!=0 && atoi(glang)==0) || (gphil[0]!=0 && atoi(gphil)==0))
		{
			printf("输入有误,请输入正确的信息!\n");
		}
		printf("请核对你输入的信息:\n");
		printf("学号：%d\n姓名：%s\n密码：%s\n年龄：%d\n班号：%d\n数学成绩：%d\n语文成绩：%d\nC语言成绩：%d\n序号：%d\n",stu_id,name,pass,atoi(age),atoi(classid),atoi(math),atoi(glang),atoi(gphil),atoi(order));
		printf("1.确认 2.取消\n");
		scanf("%d",&check);
		system("clear");
		if(check == 1)
		{
			stu_t stu = {stu_id,"0","0",atoi(age),atoi(classid),atoi(math),atoi(glang),atoi(gphil),atoi(order)};
			strcpy(stu.name,name);
			strcpy(stu.pass,pass);
			addstu(stulist,&stu);
		}

		printf("是否继续添加?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		getchar();
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;
}

//删除学生菜单
int delstu_menu(node_t *stulist)
{
	while(1)
	{
		system("clear");
		char name[16];
		char mode[128];
		int cid,age,classid,math,glang,gphil;
		int choose;
		stu_t stu = {0};
		printf("所有学生信息：\n");
		node_show(stulist,showstu);		//调用双向循环链表中查找函数
		puts("");
		printf("请选择你要删除的模式\n");
		printf("1.按学号删除\n");
		printf("2.按名字删除\n");
		printf("3.按班号删除\n");
		printf("4.按年龄删除\n");
		printf("5.按数学成绩删除\n");
		printf("6.按语文成绩删除\n");
		printf("7.按C语言成绩删除\n");

		scanf("%s",mode);
		getchar();
		//	system("clear");

		switch(atoi(mode))
		{
		case 1:printf("请输入你要删除的学号：\n");
			   scanf("%d",&cid);
			   stu.cid = cid;
			   break;
		case 2:printf("请输入你要删除的名字：\n");
			   scanf("%s",name);
			   strcpy(stu.name,name);
			   break;
		case 3:printf("请输入你要删除的班号：\n");
			   scanf("%d",&classid);
			   stu.classid = classid;
			   break;
		case 4:printf("请输入你要删除的年龄：\n");
			   scanf("%d",&age);
			   stu.age = age;
			   break;
		case 5:printf("请输入你要删除的数学成绩：\n");
			   scanf("%d",&math);
			   stu.g_lang = math;
			   break;
		case 6:printf("请输入你要删除的语文成绩：\n");
			   scanf("%d",&glang);
			   stu.g_lang = glang;
			   break;
		case 7:printf("请输入你要删除的C语言成绩：\n");
			   scanf("%d",&gphil);
			   stu.g_phil = gphil;
			   break;
		default:printf("你输入的有误！\n");break;
		}
		deletestu(stulist,atoi(mode),&stu);		//调用删除函数
		printf("是否继续删除?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;

}

//修改学生菜单函数
int updatestu_menu(node_t *stulist)
{
	while(1)
	{
		system("clear");
		int newcid,newclassid,newmath,newglang,newgphil;
		int choose;
		char mode[128];
		stu_t stu1 = {0};
		stu_t stu2 = {0};

		printf("所有学生信息：\n");
		node_show(stulist,showstu);
		printf("请输入你要修改学生信息的名字\n");
		scanf("%s",stu1.name);
		getchar();
		while(find_vdlist(stulist,&stu1)==-1)
		{
			printf("没有找到，请重新输入\n");
			scanf("%s",stu1.name);
			getchar();
		}
		printf("请选择你要修改的选项：\n");
		printf("4.修改班号\n");
		printf("5.修改学号\n");
		printf("6.修改语文成绩\n");
		printf("7.修改数学成绩\n");
		printf("8.修改C语言成绩\n");
		scanf("%s",mode);
		getchar();
		while(atoi(mode)<4 || atoi(mode)>8)
		{
			printf("输入有误,请输入正确的选项\n");
			scanf("%s",mode);
			getchar();

		}

		system("clear");
		switch(atoi(mode))
		{
		case 4:printf("请输入新班号\n");
			   scanf("%d",&newclassid);
			   stu2.classid = newclassid;		//赋值新班号
			   break;
		case 5:printf("请输入新学号\n");
			   scanf("%d",&newcid);
			   stu2.cid = newcid;
			   break;
		case 6:printf("请输入新的语文成绩\n");
			   scanf("%d",&newglang);
			   stu2.g_lang = newglang;
			   break;
		case 7:printf("请输入新的数学成绩\n");
			   scanf("%d",&newmath);
			   stu2.g_math = newmath;
			   break;
		case 8:printf("请输入新的C语言成绩\n");
			   scanf("%d",&newgphil);
			   stu2.g_phil = newgphil;
			   break;
		default:printf("你输入的有误\n");break;
		}
		updatestu(stulist,&stu1,atoi(mode),&stu2);		//调用修改函数
		printf("是否继续修改?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;

}

//排序显示菜单
int showstu_menu(node_t *stulist)
{
	int choose;
	char mode[128];
	while(1)
	{
		system("clear");

		printf("请选择你要排序的方式：\n");
		printf("1.按名字字母排序显示\n");
		printf("2.按学号排序显示\n");
		printf("3.按年龄排序显示\n");
		printf("4.按班号排序显示\n");
		printf("5.按语文成绩排序显示\n");
		printf("6.按数学成绩排序显示\n");
		printf("7.按C语言成绩排序显示\n");
		printf("8.按总成绩排序显示\n");
		scanf("%s",mode);
		system("clear");
		if(atoi(mode)>=1 && atoi(mode)<=8)
		{
			switch(atoi(mode))
			{
			case 1:show_stu(stulist,atoi(mode));break;
			case 2:show_stu(stulist,atoi(mode));break;
			case 3:show_stu(stulist,atoi(mode));break;
			case 4:show_stu(stulist,atoi(mode));break;
			case 5:show_stu(stulist,atoi(mode));break;
			case 6:show_stu(stulist,atoi(mode));break;
			case 7:show_stu(stulist,atoi(mode));break;
			case 8:show_stu(stulist,atoi(mode));break;
			default:printf("输入错误，请重新输入！\n");break;
			}
			printf("排序结果：\n");
			node_show(stulist,showstu);		//调用双向循环链表中的显示函数
		}
		else
		{
			printf("输入有误\n");
		}
		printf("是否继续查看?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;

}

//查找学生菜单
int findstu_menu(node_t *stulist)
{
	while(1)
	{
		system("clear");
		char name[16];
		char mode[128];
		int cid,age,classid,math,glang,gphil,order;
		int choose;
		stu_t stu = {0};
		printf("请选择你要查找的选项：\n");
		printf("1.查找名字\n");
		printf("2.查找学号\n");
		printf("3.查找年龄\n");
		printf("4.查找班号\n");
		printf("5.查找语文成绩\n");
		printf("6.查找数学成绩\n");
		printf("7.查找C语言成绩\n");
		printf("8.查找序号\n");
		scanf("%s",mode);
		getchar();
		system("clear");

		switch(atoi(mode))
		{
		case 1:printf("请输入你要查找的名字：\n");
			   scanf("%s",name);
			   strcpy(stu.name,name);
			   break;
		case 2:printf("请输入你要查找的学号\n");
			   scanf("%d",&cid);
			   stu.cid = cid;
			   break;
		case 3:printf("请输入你要查找的年龄\n");
			   scanf("%d",&age);
			   stu.age = age;
			   break;
		case 4:printf("请输入你要查找的班号\n");
			   scanf("%d",&classid);
			   stu.classid = classid;
			   break;
		case 5:printf("请输入你要查找的数学成绩\n");
			   scanf("%d",&math);
			   stu.g_math = math;
			   break;
		case 6:printf("请输入你要查找的语文成绩\n");
			   scanf("%d",&glang);
			   stu.g_lang = glang;
			   break;
		case 7:printf("请输入你要查找的C语言成绩\n");
			   scanf("%d",&gphil);
			   stu.g_phil = gphil;
			   break;
		case 8:printf("请输入你要查找的序号\n");
			   scanf("%d",&order);
			   stu.order = order;
			   break;
		default:printf("你输入的有误！\n");
		}

		find_stu(stulist,atoi(mode),&stu);		//c查找函数
		printf("是否继续查找?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;

}

//学生登陆菜单
int welcomestu_menu(node_t *stulist)
{
	system("clear");
	int num=0;
	int i=3;
	char name[16];
	char pass[16];
	do		//三次输错菜单
	{
		printf("你还有%d次机会\n",i);
		printf("请输入的你用户名和密码:\n");
		scanf("%s%s",name,pass);
		num++;
		i--;

		stu_t stu = {0};
		strcpy(stu.name,name);
		strcpy(stu.pass,pass);

		if(check(stulist,&stu) == 0)
		{
			stu_menu(stulist,&stu);
			return 0;
		}
		else
		{
			printf("用户名或密码错误\n");
		}
	}
	while(num<=2);
	system("clear");
	return 0;
}

//学生操作菜单
int stu_menu(node_t *stulist,stu_t *stup)
{
	system("clear");
	printf("欢迎你，%s\n",stup->name);
	while(1)
	{
		printf("菜单\n");
		printf("1.修改你的信息\n");
		printf("2.查看你的信息\n");
		printf("3.退出\n");

		char buf[128];
		printf("请输入你的选择：\n");
		scanf("%s",buf);
		system("clear");
		if(atoi(buf) == 1)
		{
			update_stu_menu(stulist,stup);		//修改学生信息函数
			break;
		}
		switch(atoi(buf))
		{
		case 1:break;;		//修改学生信息函数
		case 2:find_vdlist(stulist,stup);break;			//查看学生信息函数
		case 3:break;
		default:printf("你输入的有误，请重新输入!\n");break;
		}
		if(atoi(buf) == LOOPCOUNT)
			break;
	}
	return 0;
}

//学生修改信息菜单息函数
int update_stu_menu(node_t *stulist,stu_t *stup)
{
	system("clear");
	char newname[16];
	char newpass[16];
	char mode[128];
	int choose;
	int newcid,newage,newclassid;
	while(1)
	{
		stu_t stu2 = {0};

		find_vdlist(stulist,stup);
		printf("请选择你要修改的选项：\n");
		printf("1.修改名字\n");
		printf("2.修改密码\n");
		printf("3.修改年龄\n");
		printf("4.修改班号\n");
		printf("5.修改学号\n");
		scanf("%s",mode);
		getchar();
		while(atoi(mode)<1 || atoi(mode)>5)
		{
			printf("输入有误,请输入正确的选项\n");
			scanf("%s",mode);
			getchar();

		}
		switch(atoi(mode))
		{
		case 1:	printf("请输入新名字:\n");
				scanf("%s",newname);
				strcpy(stu2.name,newname);
				updatestu(stulist,stup,atoi(mode),&stu2);			//修改学生信息函数
				strcpy(stup->name,stu2.name);
				break;
		case 2:printf("请输入新密码:\n");
			   scanf("%s",newpass);
			   strcpy(stu2.pass,newpass);
			   updatestu(stulist,stup,atoi(mode),&stu2);
			   break;
		case 3:printf("请输入新年龄\n");
			   scanf("%d",&newage);
			   stu2.age = newage;
			   updatestu(stulist,stup,atoi(mode),&stu2);
			   break;
		case 4:printf("请输入新班号\n");
			   scanf("%d",&newclassid);
			   stu2.classid = newclassid;
			   updatestu(stulist,stup,atoi(mode),&stu2);
			   break;
		case 5:printf("请输入新学号\n");
			   scanf("%d",&newcid);
			   stu2.cid = newcid;
			   updatestu(stulist,stup,atoi(mode),&stu2);
			   break;
		default:printf("你输入的有误!\n");break;
		}
		printf("是否继续修改?\n");
		printf("1.YES\n2.NO\n");
		scanf("%d",&choose);
		system("clear");
		if(choose != 1)
			break;
	}
	return 0;

}



