
#include "tcp.h"

int sqlite3_retrieve(sqlite3 *ppDb, const char *username)
{

	char SQL_id[1024] = "";
	char SQL_cmp[1024] = "";

	const char *SQL_find = "select * from %s where %s";

	sprintf(SQL_cmp, "学号=%d", atoi(username));
	sprintf(SQL_id, SQL_find,sqlite3_table_filename, SQL_cmp);

	if(sqlite3_print(ppDb, SQL_id) != 0)
	{
		return -1;
	}
	else 
		return 0;
}

int amend_password(sqlite3 *ppDb, const char *username, const char * password)
{
	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码


	char buf[1024] = "";

	char SQL_cmp[1024] = "";
	char SQL_new[1024] = "";
	char SQL_buf[1024] = "";

	sprintf(SQL_cmp, "学号=%d", atoi(username));

	const char *SQL_update = "update %s set %s where %s";

	sprintf(SQL_new, "密码='%s'", password);
	sprintf(SQL_buf, SQL_update,sqlite3_table_filename, SQL_new, SQL_cmp);
	if(sqlite3_get_table(ppDb, SQL_buf, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
	{
		return -1;
	}
	else
		return 0;
}

Stu_t * sqlite3_find(const int username, const char *password)
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
	char *pzid;		// 错误码


	char xxx[20] = "";
	char SQL_id[1024] = "";
	char SQL_cmp[1024] = "";

	const char *SQL_find = "select * from %s where %s";

	char file[5][20] = {0};

	sprintf(SQL_cmp, "学号=%d", username);
	sprintf(SQL_id, SQL_find,sqlite3_table_filename, SQL_cmp);

	sqlite3_get_table(ppDb, SQL_id, &pazResult, &pnRow, &pnColumn, &pzid);
	if(sqlite3_print(ppDb, SQL_id) == 0)
	{
		strcpy(xxx, pazResult[6]);
	}
	else
	{
		sqlite3_close(ppDb);
		return NULL;
	}

	Stu_t * stu = (Stu_t *)malloc(sizeof(Stu_t));
	if(stu == NULL)
	{
		perror("malloc()");
		return NULL;
	}

	if(strcmp(xxx, password) == 0)
	{
		memset(SQL_cmp, 0, sizeof(SQL_cmp));
		int index = 0;
		for(int i = 0; i <= pnRow; i++)
		{
			for(int j = 0; j < pnColumn; j++)
			{
				strcpy(file[j], pazResult[index]);
				index++;
			}
			putchar(10);
		}
		stu->id = atoi(file[0]);
		strcpy(stu->password, file[1]);
		strcpy(stu->name, file[2]);
		stu->age = atoi(file[3]);
		strcpy(stu->sex, file[4]);
		sqlite3_close(ppDb);
		return stu;
	}
	else
	{
		free(stu);
		sqlite3_close(ppDb);
		return NULL;
	}
}

void sqlite3_update(char * username, sqlite3 * ppDb, int sfd_new)
{	
	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码

	char buf[1024] = "";
	char judge[20] = "";

	char SQL_buf[1024] = "";
	char SQL_cmp[1024] = "";
	char SQL_new[1024] = "";

	sprintf(SQL_cmp, "学号=%d", atoi(username));

	const char *SQL_update = "update %s set %s where %s";

	sprintf(buf, "请选择你要修改的信息:\n1.修改账户\n2.修改密码\n3.修改姓名\n4.修改年龄\n5.修改性别\n6.退出修改\n");
	write(sfd_new, buf, strlen(buf));

	read(sfd_new, judge, sizeof(judge));

	if(atoi(judge) == 6)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf, "退出成功\n");
		write(sfd_new, buf, strlen(buf));
		return;
	}
	memset(buf, 0, sizeof(buf));
	switch (atoi(judge))
	{
	case 1: 
		sprintf(buf, "请输入新账户:");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		sprintf(SQL_new, "学号=%d", atoi(buf));
		sprintf(SQL_buf, SQL_update,sqlite3_table_filename, SQL_new, SQL_cmp);
		break;
	case 2:
		sprintf(buf, "请输入新密码:");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		sprintf(SQL_new, "密码='%s'", buf);
		sprintf(SQL_buf, SQL_update,sqlite3_table_filename, SQL_new, SQL_cmp);
		break;
	case 3:
		sprintf(buf, "请输入新姓名:");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		sprintf(SQL_new, "姓名='%s'", buf);
		sprintf(SQL_buf, SQL_update,sqlite3_table_filename,SQL_new, SQL_cmp);
		break;
	case 4:
		sprintf(buf, "请输入新年龄");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		sprintf(SQL_new, "年龄=%d", atoi(buf));
		sprintf(SQL_buf, SQL_update,sqlite3_table_filename, SQL_new, SQL_cmp);
		break;
	case 5:
		sprintf(buf, "请输入新性别");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		sprintf(SQL_new, "性别='%s'", buf);
		sprintf(SQL_buf, SQL_update,sqlite3_table_filename, SQL_new, SQL_cmp);
		break;
	default:break;
	}
	if(sqlite3_get_table(ppDb, SQL_buf, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf, "有相同用户名,修改失败\n");
		write(sfd_new, buf, strlen(buf));
	//	printf("sqlite3_get_table error:%s\n", pzErrmsg);
		return;
	}
	else
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf, "修改成功\n");
		write(sfd_new, buf, strlen(buf));
	}
}

int sqlite3_print(sqlite3 * ppDb, const char * str)
{
	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码

	if(sqlite3_get_table(ppDb, str, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
	{
		printf("sqlite3_get_table error:%s\n", pzErrmsg);
		return -1;
	}
	else if(pnRow == 0)
	{
		return -1;
	}

	return 0;
}

char *s_gets(char *st, int n)
{
	char *find = NULL;
	char *ret_val = NULL;
	ret_val = fgets(st, n, stdin);
	if(ret_val)
	{
		find = strchr(st, '\n');
		if(find)
			*find = '\0';
		else
			while(getchar() != '\n')
				continue;
	}
	return ret_val;
}

int sqlite3_delete(char * username, sqlite3 * ppDb, int sfd_new)
{
	char buf[30] = "";
	char SQL_buf[1024] = "";
	char SQL_cmp[1024] = "";
	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码

	sprintf(buf, "是否删除?\n1.是\n2.否");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));

	if(atoi(buf) == 1)
	{
		const char *SQL_find = "delete from %s where %s";

		sprintf(SQL_cmp, "学号=%d", atoi(username));

		sprintf(SQL_buf, SQL_find,sqlite3_table_filename, SQL_cmp);

		if(sqlite3_get_table(ppDb, SQL_buf, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
		{

			memset(buf,0,sizeof(buf));
			sprintf(buf, "删除失败");
			write(sfd_new, buf, strlen(buf));
			printf("sqlite3_get_table error:%s\n", pzErrmsg);
			sqlite3_close(ppDb);
			return 1;
		}
		else
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf, "删除成功");
			write(sfd_new, buf, strlen(buf));
			sqlite3_close(ppDb);
			return 0;
		}
	}
	else
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf, "删除取消");
		write(sfd_new, buf, strlen(buf));
		return 1;
	}

}


