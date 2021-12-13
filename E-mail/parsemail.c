// 邮件解析－－字符串搜索并提取
// 功能：邮件检索，提取主题字并转存
//
//
#include "parsemail.h"

// 验证用户名
int verusername(const char * username, struct table * usertable)
{
	if(NULL == username || NULL == usertable)
		return -1;

	table_t * temp = NULL;
	temp = sqlite3_find(username);	// 调用数据库查找函数
	if(temp == NULL)
	{
		return -1;
	}

	memcpy(usertable, temp, sizeof(*temp));	// 内存拷贝
	free(temp);
	return 0;
}
//传进来之前去掉/t/n
//验证密码
int verpassword(const char * password, struct table * usertable)
{
	if(NULL == password || NULL == usertable)
		return -1;

	if(strcmp(password, usertable->password) == 0)
		return 0;
	
	return -1;

}
// 解析邮件内容
int parseMail(struct mail * pmail, struct subject_ctrl * subject)
{
	if(NULL == pmail || NULL == subject)
		return -1;

	const char * sub = "Subject:";
	char * start = strstr(pmail->raw, sub)  + 9;
	if(NULL == start)
		return -1;
	char * end = strstr(start, "\r\n") - 1;
	if(NULL == end)
		return -1;

	int len = end - start + 1;

	char buf[128] = "";
	strncpy(buf, start, len);

	const char *command = strtok(buf, " ");
	if(NULL == command)
		return -1;

	strcpy(subject->command, command);
	if(strcmp(command, "8LED") == 0)
	{
		char array[128] = "";
		const char * blub = strtok(NULL, " ");
		subject->bulb = atoi(blub);
		const char * blub_ctrl = strtok(NULL, " ");

		subject->bulb_ctrl = atoi(blub_ctrl);
		sprintf(array, "%s %d %d", command, atoi(blub), atoi(blub_ctrl));
		strcpy(pmail->subject, array);
	}
	else if(strcmp(command, "7SHU") == 0)
	{
		char array[128] = "";
		const char * num = strtok(NULL, " ");
		subject->signal = atoi(num);
		sprintf(array, "%s %d", command, atoi(num));
		strcpy(pmail->subject, array);
	}
	else if(strcmp(command, "MOTO") == 0)
	{
		char array[128] = "";
		const char *revo = strtok(NULL, " ");
		subject->revp = atoi(revo);
		sprintf(array, "%s %d", command, atoi(revo));
		strcpy(pmail->subject, array);
	}
	else if(strcmp(command, "CHANGTABLE") == 0)
	{
		strcpy(pmail->subject, command);
	}
	else 
	{
		strcpy(pmail->subject, command);
		return -1;
	}

	return 0;

}

table_t * sqlite3_find(const char *username)
{
	sqlite3 *ppDb;	//数据库操作句柄
	if(sqlite3_open(sqlite3_database_filename, &ppDb) != 0) // 打开数据库
	{
		printf("sqlite3_open error:%s\n", sqlite3_errmsg(ppDb));
		return NULL;
	}

	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码

	char xxx[20] = "";
	char SQL_id[128] = "";
	char SQL_cmp[128] = "";
	const char *SQL_find = "select * from %s where %s";

	sprintf(SQL_cmp, "用户名='%s'", username);	// 组装
	sprintf(SQL_id, SQL_find,sqlite3_table_filename, SQL_cmp);	// 组装查找

	if(sqlite3_get_table(ppDb, SQL_id, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0 )
	{
		printf("sqlite3_get_table error:%s\n", pzErrmsg);
		sqlite3_close(ppDb);
		return NULL;
	}
	else if(pnRow == 0)
	{
		sqlite3_close(ppDb);
		return NULL;
	}

	strcpy(xxx, pazResult[3]);	// 获取密码
	table_t * stu = (table_t *)malloc(sizeof(table_t));
	if(stu == NULL)
	{
		perror("malloc()");
		return NULL;
	}
	
	strcpy(stu->username, username);	//拷贝用户名至验证结构体
	strcpy(stu->password, xxx);		// 拷贝密码至验证结构体
	sqlite3_close(ppDb);	// 关闭数据库
	return stu;
}


