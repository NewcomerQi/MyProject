#ifndef PARSEMAIL_H
#define PARSEMAIL_H

#include<string.h>
#include <stdio.h>
#include "common.h"
#include "base64.h"
#include<sqlite3.h>
#include <unistd.h>


#define sqlite3_database_filename "mysqlite.db"	// 数据库名称
#define sqlite3_table_filename "user"		// 表名称

int verusername(const char * username, struct table * usertable);
/***********
 *功能：验证用户名
 *参数：usernam:用户名
 *		usertable:验证结构体
 *返回值:
 		成功：０，　失败-1
 * ********/

int verpassword(const char * password, struct table * usertable);
/**************
 *功能：验证密码
 *
 *
 * *************/

int parseMail(struct mail * pmail, struct subject_ctrl * subject);
/*******************
 *功能：解析邮件
 *参数:
 *		pmail：邮件结构体指针
 *		subject:主题字结构指针
 *返回值:
 		成功：０失败-1
 * *****************/

table_t * sqlite3_find(const char * username);
/****************
 *在数据库中查找用户名和密码
 *参数:
 *		username:用户名
 *返回值:
 		成功返回验证结构体
		失败：ＮＵＬＬ
 * *************/

#endif


