#ifndef _TCP_H_
#define _TCP_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<sqlite3.h>
#define sqlite3_database_filename "mysqlite.db"	// 数据库名称
#define sqlite3_table_filename "user"		// 表名称
#define SMTP_IP ""
#define PORT "10000"	// 固定端口
typedef struct Accept
{
	struct sockaddr_in my_addr;
	int sfd_new;
} Accept_t;

typedef struct student
{
	int id;			// 学号
	char password[20];	//密码
	char name[20];		//姓名
	int age;			//年龄
	char sex[10];		//性别
}Stu_t;


int ftp_init(const char *ip, short port);
int ftp_accept(int sfd, const char *ip, short port, struct sockaddr_in *cid_addr);
void *start_rountine(void *arg);
void print_quit(struct sockaddr_in client_addr);
void menu(char *username, sqlite3 * ppDb, int sfd_new);
void my_register(int sfd_new);
void retrieve_password(int sfd_new);
int tcp_connect(char *ip, short port);
void chating(int sfd_new);
void file_send(int sfd_new);

char *s_gets(char *st, int n);
Stu_t * sqlite3_find(const int username, const char *password);
void sqlite3_update(char * username, sqlite3 * ppDb, int sfd_new);
int sqlite3_delete(char * username, sqlite3 * ppDb, int sfd_new);
int sqlite3_print(sqlite3 * ppDb, const char * str);
int sqlite3_retrieve(sqlite3 *ppDb, const char *username);
int amend_password(sqlite3 *ppDb, const char *username, const char * password);


#endif
