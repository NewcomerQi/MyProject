#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <string.h>
#include "common.h"
#include <unistd.h>
#include "base64.h"
#include "parsemail.h"
#include "mailrecv.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/wait.h>
#include "devicecontrol.h"
#include<dirent.h>


int handleconnection(int sockfd, struct mail * pmail);
/***********************
 *函数名:int handleconnection(int sockfd, struct mail * pmail);
 *功能：SMTP协议通信，接收邮件
 *参数:
 *		sockfd: 套接字
 *		pmail: 结构体指针
 *返回值:
 *		邮件接收成功返回0, 失败返回-1
 * *********************/

int pop3connection(int sfd_new,ctrl_t * subject,mail_t *pmail);
/********************
 *函数名:int pop3connection(int sfd_new, ctrl_t *subject, mail_t *pmail);
 *功能:ＰＯＰ３协议，发送邮件
 *参数:
 *		sfd_new:文件描述符
 *		subject:主题字控制结构指针
 *		pmail:邮件结构体指针
 *返回值：
 *		成功：０，失败-1
 * *******************/

int ftp_init(const char *ip, short port);
/***************
 *函数名:int ftp_init(const char *ip, short port);
 *功能：完成ＴＣＰ协议中创建socket, 绑定socket到本地端口，监听
 *	参数:
 *		ip: ＩＰ地址
 *		port:端口号
 *返回值:成功：文件描述符,失败-1
 *************/

int ftp_accept(int sfd, const char *ip, short port);
/********************
 *函数名：int ftp_accept(int sfd, const char *ip, short port)
 *功能：ＴＣＰ中,服务器等待客户端连接		
 *参数：
 *	sfd:初始化的文件描述符
 *	ip:IP地址
 *	port:端口号
 *成功返回新的描述符，失败-1
 * *********************/

#endif
