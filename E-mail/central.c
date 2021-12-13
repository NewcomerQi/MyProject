/*****************
 *1.主控模块
 *功能：初始化邮件体参数，套接字生成，进程建立，各模块顺序调用
 * ************/

#include "parsemail.h"
#include "net.h"
#include "devicecontrol.h"
#include<dirent.h>
#include<sys/param.h>


// 存储服务器和客户端连接后的套接字结构体
typedef struct Accept
{
	int sfd_new;
} Accept_t;

void *start_send(void *arg);	// 发送邮件函数
void *start_recv(void *arg);	// 接收邮件函数
void guardprogress(void);		// 将SMPTY和ＰＯＰ３的调用封装在一个函数

int main()
{
	
	//1.创建子进程
    pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork()");
		exit(-1);
	}
	if(pid == 0)
	{
    	guardprogress();
	}
    //2.结束父进程
    if (pid > 0)
    {
     //   printf("01.创建子进程，父进程结束\n");
        exit(0);
    }
    //3.创建会话，并担任会话组组长
    if (setsid() == -1)
    {
        printf("创建会话)失败\n");
    }

    //4.切换工作目录


    chdir("/tmp");


    //6.关闭继承过来的父进程的文件描述符
    for (size_t i = 0; i < getdtablesize(); i++)
    {
        close(i);
   	}
    //5.更改权限,设置掩码
    umask(0);
	return 0;
}

void *start_recv(void *arg)
{
	Accept_t Accept = *(Accept_t *)(arg);	
	int sfd_new = Accept.sfd_new;

	ctrl_t ctrl = {0};	// 主题字结构体
	mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));	// 邮件结构体

	handleconnection(sfd_new,pmail);	// STMP协议通信，接收邮件

	parseMail(pmail, &ctrl);	// 解析邮件
	SubjectControl(pmail,&ctrl);	//主题字控制

	// 截取邮件内容
	char *a = strstr(pmail->raw, ".\r\n");
	char *y = pmail->raw;
	y[strlen(y) - strlen(a)] = '\0';	

	pmail->len = strlen(y);	// 邮件长度
	strcpy(pmail->raw, y);		// 邮件内容
	strcat(pmail->raw, "\n");
	strcat(pmail->raw, pmail->subject);

	char buf[128] = "";
	DIR * dir = NULL;
	if((dir = opendir(pmail->recv)) == NULL)	// 判断邮箱是否存在
	{
		mkdir(pmail->recv, 0666);	// 不存在就创建一个
	}
	else
		closedir(dir);	// 关闭邮箱

	int i = 1;
	sprintf(buf, "%s/%d_%s",pmail->recv,i,pmail->send);	// 创建邮件名
	while(access(buf, F_OK) == 0)	// 判断是否有同名邮件
	{
		i++;	
		sprintf(buf, "%s/%d_%s", pmail->recv, i, pmail->send);	// 有就i++，给邮件区别名
	}

	int fd = open(buf, O_CREAT | O_WRONLY | O_TRUNC, 0666);	// 创建邮件文件
	if(fd == -1)
	{
		perror("open(error)");
		exit(-1);
	}

	write(fd, pmail->raw, strlen(pmail->raw));	// 将邮件内容写入邮件
	close(fd);	// 关闭邮件

	close(sfd_new);	
	free(pmail);	// 释放pamil内存，防止泄露
	pmail = NULL;
	return NULL;
}


void *start_send(void *arg)
{
	Accept_t Accept = *(Accept_t *)(arg);	// 初始化的描述符
	int sfd_new = Accept.sfd_new;	// 服务器等待客户端连接

	ctrl_t ctrl = {0};	//主题字结构体
	mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));	// 邮件结构体

	pop3connection(sfd_new, &ctrl, pmail);	// pop3发邮件锁步流程

	close(sfd_new);
	free(pmail);
	pmail = NULL;
	return NULL;
}

void guardprogress(void)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		return;
	}
	else if(pid == 0)	// 子进程
	{
		if(fork() == 0)
		{
			int sfd = ftp_init(IP, atoi(PORT));	// 创建套接字，绑定，监听

			pthread_t newthread;

			Accept_t Accept1 = {0};

			while(1)
			{
				int sfd_new = ftp_accept(sfd, IP, atoi(PORT));

				Accept1.sfd_new = sfd_new;

				pthread_create(&newthread, NULL, start_recv, (void*)&Accept1);	// 创建线程
				pthread_detach(newthread);	// 线程分离，线程结束自动回收资源

			}
			close(sfd);
		}
		if(fork() == 0)
		{
			int sfdd = ftp_init(IP, MYPORT);	// 创建套接字，绑定，监听
			pthread_t pop;
			Accept_t Accept2 = {0};

			while(1)
			{
				int new_sfd = ftp_accept(sfdd, IP, MYPORT);	// 服务器和客户端等待连接

				Accept2.sfd_new = new_sfd;	// 存储新的描述符，作为pthread_create的参数

				pthread_create(&pop, NULL, start_send, (void*)&Accept2);	// 创建线程
				pthread_detach(pop);	// 线程分离

			}
			close(sfdd);
		}
	}
	else	// 父进程
	{
		;
	}
}
